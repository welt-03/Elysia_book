
set(scrs
        "."
        "book/book_config.c"
        "audio/audio.cpp"
)

set(include_dirs
        "."
        "book"
        "audio"
)

# 配置组件
idf_component_register( SRCS "${scrs}"
                        INCLUDE_DIRS "${include_dirs}"
                        REQUIRES "driver"
)
