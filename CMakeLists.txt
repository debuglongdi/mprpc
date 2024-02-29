cmake_minimum_required(VERSION 3.20)
project(mprpc CXX)
# 设置调试信息 以及启动c++11语言标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 设置Debug、Release构建类型的编译选项,如果将项目编译成动态库，并且项目又使用了静态库，那就需要在编译选项中加-fPIC
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -std=c++11")

set(CMAKE_BUILD_TYPE Debug)

#可执行文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#项目库路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


#设置编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
#编译库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

#项目框架代码
add_subdirectory(src)
#项目框架使用代码
add_subdirectory(example)