idf_component_register(SRCS "spiffs_example_main.c" "ili9340.c" "fontx.c"
                    INCLUDE_DIRS ".")
                    
spiffs_create_partition_image(storage ../fonts FLASH_IN_PROJECT)