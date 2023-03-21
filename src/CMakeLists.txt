project(SharedMemoryRingBuffer)

set(SHMRB_SOURCES ring-buffer.cpp lock.cpp)

#Add source files to target
target_sources(${LIBRARY_NAME} PRIVATE ${SHMRB_SOURCES})
