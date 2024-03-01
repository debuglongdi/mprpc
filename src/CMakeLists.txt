# aux新增加文件后，不会将其加入到makefile中，需要cmake“清理所有项目的重新配置”
aux_source_directory(. SRC_LIST)
#编译项目为静态库
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base  pthread zookeeper_mt)

