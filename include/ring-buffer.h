#pragma once

#include <glib-2.0/glib.h>
#include <pthread.h>

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

    gboolean m_isProducer;
    gsize m_itemSize;
    gsize m_size;

    void pthread_rwlock_init_pshared(pthread_rwlock_t* rwlock);

public:
    RingBuffer(const gchar* name, gboolean isProducer, gsize capacity, gsize itemSize);
    ~RingBuffer();

    gboolean CanRead();

    void AddItem(gpointer data);
    gpointer GetItem(gint& outCurrentIndex, gint desiredIndex = -1);

    gpointer GetWritePointer();
    gpointer GetReadPointer(gint& outCurrentIndex, gint desiredIndex = -1);

    inline void LockRead() { pthread_rwlock_rdlock(m_lock); }
    inline void UnlockRead() { pthread_rwlock_unlock(m_lock); }
    inline void LockWrite() { pthread_rwlock_wrlock(m_lock); }
    inline void UnlockWrite() { pthread_rwlock_unlock(m_lock); }

    static void Destroy(const gchar* name);
};
