# 路径
file(GLOB_RECURSE source "book_init/*.c" "audio/*.cpp")

# 文件夹
set(include_dirs
    book_init
    audio)

# 配置组件
idf_component_register( SRCS ${source}
                        INCLUDE_DIRS ${include_dirs}
                        REQUIRES "driver")
