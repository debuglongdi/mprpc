cmake_minimum_required(VERSION 3.0)
project(mprpc)

#可执行文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#项目库路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/src)


#设置编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
#编译库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

# add_subdirectory(src)
add_subdirectory(example)