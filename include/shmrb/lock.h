#pragma once

#include <glib-2.0/glib.h>
#include <pthread.h>

class Lock
{
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_read;
    pthread_cond_t m_write;

    guint m_reader, m_waitReader;
    guint m_writer, m_waitWriter;
public:
    void Init();
    ~Lock();

    void LockReader();
    void UnlockReader();

    void LockWriter();
    void UnlockWriter();
};