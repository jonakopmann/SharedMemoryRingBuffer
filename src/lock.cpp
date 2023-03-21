#include <iostream>

#include "shmrb/lock.h"

void pthread_mutex_init_pshared(pthread_mutex_t* mutex)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

void pthread_cond_init_pshared(pthread_cond_t* cond)
{
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    pthread_cond_init(cond, &attr);
    pthread_condattr_destroy(&attr);
}

Lock::~Lock()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_read);
    pthread_cond_destroy(&m_write);
}

void Lock::Init()
{
    pthread_mutex_init_pshared(&m_mutex);

    pthread_cond_init_pshared(&m_read);
    pthread_cond_init_pshared(&m_write);

    m_reader = m_waitReader = 0;
    m_writer = m_waitWriter = 0;
}

void Lock::LockReader()
{
    pthread_mutex_lock(&m_mutex);

    if (m_writer || m_waitWriter)
    {
        m_waitReader++;
        do
        {
            pthread_cond_wait(&m_read, &m_mutex);
            if (m_writer || m_waitWriter)
            {
                std::cout << "waiting ... " << std::endl;
            }
        } while (m_writer || m_waitWriter);
        m_waitReader--;
    }

    m_reader++;

    pthread_mutex_unlock(&m_mutex);
}

void Lock::UnlockReader()
{
    pthread_mutex_lock(&m_mutex);

    m_reader--;

    if (m_waitWriter)
    {
        pthread_cond_broadcast(&m_write);
    }

    pthread_mutex_unlock(&m_mutex);
}

void Lock::LockWriter()
{
    pthread_mutex_lock(&m_mutex);

    if (m_reader)
    {
        m_waitWriter++;
        do
        {
            pthread_cond_wait(&m_write, &m_mutex);
        } while (m_reader);
        m_waitWriter--;
    }
    m_writer++;

    pthread_mutex_unlock(&m_mutex);
}

void Lock::UnlockWriter()
{
    pthread_mutex_lock(&m_mutex);

    m_writer--;

    if (m_waitReader)
    {
        pthread_cond_broadcast(&m_read);
    }

    pthread_mutex_unlock(&m_mutex);
}