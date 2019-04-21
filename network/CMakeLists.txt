find_package(Threads)
find_package(Curses)

add_executable(server server.c)
add_executable(client client.c)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
