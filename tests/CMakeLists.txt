project(SharedMemoryRingBuffer)

add_executable(TestConsumer test-consumer.cpp)
add_executable(TestProducer test-producer.cpp)

target_link_libraries(TestConsumer ${LIBRARY_NAME})
target_link_libraries(TestProducer ${LIBRARY_NAME})
