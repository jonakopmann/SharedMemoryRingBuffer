#include <chrono>
#include <iostream>
#include <random>
#include <shmrb/ring-buffer.h>

int main()
{
    RingBuffer::Destroy("/test_ring_buffer");
    RingBuffer* rb = new RingBuffer("/test_ring_buffer", TRUE, 10, 4);
    auto start = std::chrono::system_clock::now();
    gint valuesWritten = 0;
    while (TRUE)
    {
        rb->LockWrite();

        gpointer write = rb->GetWritePointer();

        *(gint*)write = rand() % 100;

        //std::cout << "wrote to buffer" << std::endl;

        rb->UnlockWrite();

        valuesWritten++;
        std::cout << "\r" << valuesWritten;

        auto current = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(current - start).count() >= 60)
        {
            break;
        }
    }

    delete rb;

    return 0;
}