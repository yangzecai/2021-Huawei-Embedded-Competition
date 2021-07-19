# cmake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project("Embedded-2021")

# 编译参数
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++17")
add_definitions(-std=c++17)
# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

# 设置头文件路径列表
set(INC_LIST
    ${CMAKE_SOURCE_DIR}/include
)

# 包含头文件
include_directories(${INC_LIST})

# 添加cmake子目录
add_subdirectory(source)

# 添加执行文件
add_executable(Embedded-2021 ${PROJECT_SOURCE_DIR}/main.cpp)

# 添加链接
target_link_libraries(Embedded-2021
    source-objects
)