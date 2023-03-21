project(SharedMemoryRingBuffer)

set(SHMRB_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/lock.h
    ${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}/ring-buffer.h)
    

#Add header files to target
target_sources(${LIBRARY_NAME} PRIVATE ${SHMRB_HEADERS})

#Install header files
install(FILES ${SHMRB_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_NAME})

install(TARGETS ${LIBRARY_NAME} EXPORT libshmrb
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${LIBRARY_NAME})