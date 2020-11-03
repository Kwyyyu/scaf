set(COMPONENT_SRCS "main.c"
                   "iot.c"
                   "motor.c"
                   "weight.c"
                   "pir.c"
                   "ili9340.c"
                   "fontx.c"
                   "lcd.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()

target_add_binary_data(${COMPONENT_TARGET} "certs/private_key.pem" TEXT)

spiffs_create_partition_image(storage ../fonts FLASH_IN_PROJECT)