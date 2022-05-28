idf_component_register(
    SRCS
    "db.c"
    "fs.c"

    INCLUDE_DIRS
    "include"

    REQUIRES nvs_flash spiffs
)
