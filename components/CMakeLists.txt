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
        console
        esp_tinyusb
)

# 配置组件
idf_component_register(
    SRCS                ${scrs}         # list the source files of this component
    INCLUDE_DIRS        ${include_dirs} # optional, add here public include directories
    PRIV_INCLUDE_DIRS                   # optional, add here private include directories
    REQUIRES            ${requires}     # optional, list the public requirements (component names)
    PRIV_REQUIRES                       # optional, list the private requirements
)