set(COMPONENT_SRCS "bme680.c" "bsec_integration.c" "http_custom_client.c" "bme_dependency.c" "i2c_custom.c" "wifi_sensor.c" "main.c" )
set(COMPONENT_ADD_INCLUDEDIRS ".")

# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES)
set(COMPONENT_PRIV_REQUIRES )
register_component()
target_link_libraries(${COMPONENT_TARGET} "-L${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${COMPONENT_TARGET} algobsec)
