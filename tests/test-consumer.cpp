#include <chrono>
#include <iostream>
#include <shmrb/ring-buffer.h>

int main()
{
    RingBuffer* rb = new RingBuffer("/test_ring_buffer", FALSE, 10, 4);

    gpointer read;
    gint idx = -1;
    auto start = std::chrono::system_clock::now();

    gint valuesRead = 0;

    while (TRUE)
    {
        if (!rb->CanRead())
        {
            continue;
        }
        rb->LockRead();

        //std::cout << "desired index: " << idx;

        gint currentIdx;
        read = rb->GetReadPointer(currentIdx, idx);

        while (read == NULL)
        {
            rb->UnlockRead();
            rb->LockRead();
            read = rb->GetReadPointer(currentIdx, idx);
        }
        idx = currentIdx;

        //std::cout << " value: " << *(gint*)read << std::endl;
        valuesRead++;
        std::cout << "\r" << valuesRead;

        rb->UnlockRead();

        auto current = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(current - start).count() >= 60)
        {
            break;
        }
    }

    delete rb;

    return 0;
}