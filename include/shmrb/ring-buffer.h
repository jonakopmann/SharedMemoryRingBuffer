#pragma once

#include <glib-2.0/glib.h>
#include <pthread.h>

#define EVENT_BUFFER_SHM "/shm_ring_buffer"

class RingBuffer
{
private:
    struct Header
    {
        gsize capacity;
        gint begin;
        gint end;
    };
    Header* m_header;
    pthread_rwlock_t* m_lock;
    gpointer m_data;

    gsize m_itemSize;
    gsize m_size;
    gboolean m_isProducer;


    void pthread_rwlock_init_pshared(pthread_rwlock_t* rwlock);

public:
    RingBuffer(const gchar* name, gboolean isProducer, gsize capacity, gsize itemSize);
    ~RingBuffer();

    gboolean CanRead();

    void AddItem(gpointer data);
    gpointer GetItem(gint& outCurrentIndex, gint desiredIndex = -1);

    gpointer GetWritePointer();
    gpointer GetReadPointer(gint& outCurrentIndex, gint desiredIndex = -1);

    void LockRead() { pthread_rwlock_rdlock(m_lock); }
    void UnlockRead() { pthread_rwlock_unlock(m_lock); }
    void LockWrite() { pthread_rwlock_wrlock(m_lock); }
    void UnlockWrite() { pthread_rwlock_unlock(m_lock); }

    static void Destroy(const gchar* name);
};
