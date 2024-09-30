cmake_minimum_required (VERSION 3.8)

project (simulador-eleicao C)

file(GLOB_RECURSE MY_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*c")

add_executable(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE "${MY_SRC}")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
