set(COMPONENT_SRCS "test.c")
set(COMPONENT_ADD_INCLUDEDIRS "."
	"iot-device-sdk-embedded-c/include")
register_component()
target_add_binary_data(${COMPONENT_TARGET} "certs/private_key.pem" TEXT)
