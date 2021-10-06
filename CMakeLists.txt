cmake_minimum_required(VERSION 3.15)

project(linger VERSION 1.0.0 LANGUAGES C)

add_executable(client utils.h client.c)
add_executable(server utils.h server.c)
