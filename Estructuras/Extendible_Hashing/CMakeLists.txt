cmake_minimum_required(VERSION 3.27)
project(EXTENDIBLE_HASHING)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(readData test/readData.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include src utils test)

target_sources(
        ${PROJECT_NAME}
        PRIVATE
        include/bucket/Bucket.h
        include/bucket/Register_Index.h
        include/directory/Single_Entry.h
        include/directory/Directory.h
        include/Extendible_Hashing.h
        src/homesRecord.h
        utils/macros.h
)