project(scl011)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -Wextra -Wpedantic -std=c11")

find_package(LIBUSB REQUIRED)

include_directories(
${LIBUSB_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIBUSB_LIBRARIES})
