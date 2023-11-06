# 源文件路径
set(scrs
        "."
        "book/book_config.c"
        "peripheral/peripheral.cpp"
)

# 头文件夹
set(include_dirs
        "."
        "book"
        "peripheral"
)

# 配置组件
idf_component_register( SRCS "${scrs}"
                        INCLUDE_DIRS "${include_dirs}"
                        REQUIRES "driver"
)
