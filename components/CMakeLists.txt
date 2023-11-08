# 源文件路径
set(scrs
        "."
        "book/book_config.c"
        "disk/disk.c"
        "peripheral/peripheral.cpp"
)

# 头文件夹
set(include_dirs
        "."
        "book"
        "disk"
        "peripheral"
)

# 前置组件
set(requires
        freertos
        driver
        fatfs
)

# 配置组件
idf_component_register( SRCS            ${scrs}
                        include_dirs    ${include_dirs}
                        REQUIRES        ${requires}
)
