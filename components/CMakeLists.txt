
set(scrs
        "."
        "book_init/book_init.c"
        "audio/audio.cpp"
)

set(include_dirs
        "."
        "book_init"
        "audio"
)

# 配置组件
idf_component_register( SRCS "${scrs}"
                        INCLUDE_DIRS "${include_dirs}"
                        REQUIRES "driver"
)
