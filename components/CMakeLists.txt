# 源文件路径
set(scrs
        "."
        "book/book_config.c"
        "disk/disk.c"
        "peripheral/peripheral.cpp"
        "lvgl/src/."
        "esp_lvgl_port/."
        "display_driver/."
        "display_driver/lvgl_tft/."
        "display_driver/lvgl_touch/."
        "display_driver/lvgl_i2c/."
)

# 头文件夹
set(include_dirs
        "."
        "book"
        "disk"
        "peripheral"
        "lvgl"
        "esp_lvgl_port"
        "display_driver"
)

# 前置组件
set(requires
        freertos
        driver
        fatfs
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