#include <fcntl.h>
#include <sys/mman.h>   
#include <sys/stat.h>

#include "ring-buffer.h"

void RingBuffer::pthread_rwlock_init_pshared(pthread_rwlock_t* rwlock)
{
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    pthread_rwlock_init(rwlock, &attr);
    pthread_rwlockattr_destroy(&attr);
}


RingBuffer::RingBuffer(const gchar* name, gboolean isProducer, gsize capacity, gsize itemSize)
    : m_isProducer(isProducer), m_itemSize(itemSize)
{
    gint shm_fd = shm_open(name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG); // TODO: O_TRUNC?
    if (shm_fd < 0)
    {
        g_error("shm_open failed");
        return;
    }

    m_size = sizeof(Header) + sizeof(pthread_rwlock_t) + capacity * m_itemSize;

    if (isProducer && (ftruncate(shm_fd, m_size) < 0))
    {
        g_error("ftruncate failed");
    }

    gpointer p_buf = NULL;
    p_buf = mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (p_buf == (void*)-1)
    {
        g_error("mmap failed");
        return;
    }

    // Layout of RingBuffer in shared memory
    // | Header | Lock |    Data    |

    m_header = reinterpret_cast<Header*>(p_buf);
    m_lock = reinterpret_cast<pthread_rwlock_t*>((gchar*)p_buf + sizeof(Header));
    m_data = (gpointer)((gchar*)m_lock + sizeof(pthread_rwlock_t));

    if (m_isProducer)
    {
        m_header->capacity = capacity;
        m_header->begin = 0;
        m_header->end = 0;
        pthread_rwlock_init_pshared(m_lock);
    }
}

RingBuffer::~RingBuffer()
{
    if (m_header)
    {
        munmap((gpointer)m_header, m_size);
    }
    m_header = NULL;
    m_lock = NULL;
    m_data = NULL;
}

gboolean RingBuffer::CanRead()
{
    return TRUE;
}

void RingBuffer::AddItem(gpointer data)
{
    LockWrite();

    // copy over to m_data
    memmove((gpointer)((gchar*)m_data + m_header->end * m_itemSize), data, m_itemSize);

    m_header->end = (m_header->end + 1) % m_header->capacity;
    if (m_header->end == m_header->begin)
    {
        // buffer is full move begin
        m_header->begin = (m_header->begin + 1) % m_header->capacity;
    }

    UnlockWrite();
}

gpointer RingBuffer::GetItem(gint& outCurrentIndex, gint desiredIndex /*= -1*/)
{
    LockRead();

    if (m_header->begin == m_header->end || desiredIndex == m_header->end)
    {
        UnlockRead();
        return NULL;
    }

    outCurrentIndex = desiredIndex;
    if (desiredIndex == -1 ||
        (m_header->begin < m_header->end && (desiredIndex < m_header->begin || desiredIndex > m_header->end)) ||
        (m_header->begin > m_header->end && (desiredIndex < m_header->begin && desiredIndex > m_header->end)))
    {
        outCurrentIndex = m_header->begin;
    }

    gpointer data = (gpointer)((gchar*)m_data + outCurrentIndex * m_itemSize);
    outCurrentIndex = (outCurrentIndex + 1) % m_header->capacity;

    UnlockRead();

    return data;
}

gpointer RingBuffer::GetWritePointer()
{
    gpointer retVal = (gchar*)m_data + m_header->end * m_itemSize;

    m_header->end++;
    m_header->end %= m_header->capacity;
    if (m_header->end == m_header->begin)
    {
        // buffer is full move begin
        m_header->begin++;
        m_header->begin %= m_header->capacity;
    }

    return retVal;
}

gpointer RingBuffer::GetReadPointer(gint& outCurrentIndex, gint desiredIndex /*= -1*/)
{
    if (m_header->begin == m_header->end || desiredIndex == m_header->end)
    {
        return NULL;
    }

    outCurrentIndex = desiredIndex;
    if (desiredIndex == -1 ||
        (m_header->begin < m_header->end && (desiredIndex < m_header->begin || desiredIndex > m_header->end)) ||
        (m_header->begin > m_header->end && (desiredIndex < m_header->begin && desiredIndex > m_header->end)))
    {
        outCurrentIndex = m_header->begin;
    }

    gpointer data = (gchar*)m_data + outCurrentIndex * m_itemSize;
    outCurrentIndex++;
    outCurrentIndex %= m_header->capacity;

    return data;
}

void RingBuffer::Destroy(const gchar* name)
{
    shm_unlink(name);
}
