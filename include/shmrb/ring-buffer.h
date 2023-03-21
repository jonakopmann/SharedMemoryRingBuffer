#pragma once

#include "lock.h"

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
    Lock* m_lock;
    gpointer m_data;

    gsize m_itemSize;
    gsize m_size;
    gboolean m_isProducer;
public:
    RingBuffer(const gchar* name, gboolean isProducer, gsize capacity, gsize itemSize);
    ~RingBuffer();

    gboolean CanRead();

    void AddItem(gpointer data);
    gpointer GetItem(gint& outCurrentIndex, gint desiredIndex = -1);

    gpointer GetWritePointer();
    gpointer GetReadPointer(gint& outCurrentIndex, gint desiredIndex = -1);

    void LockRead() { m_lock->LockReader(); }
    void UnlockRead() { m_lock->UnlockReader(); }
    void LockWrite() { m_lock->LockWriter(); }
    void UnlockWrite() { m_lock->UnlockWriter(); }

    static void Destroy(const gchar* name);
};
