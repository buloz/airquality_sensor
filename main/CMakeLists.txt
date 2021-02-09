set(COMPONENT_SRCS "include_bme/bme680.c" "include_bme/bsec_integration.c" "include_custom/http_custom_client.c" "include_custom/bme_dependency.c" "include_custom/i2c_custom.c" "include_custom/wifi_sensor.c" "main.c" )
set(COMPONENT_ADD_INCLUDEDIRS "include_bme" "include_custom")

# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES)
set(COMPONENT_PRIV_REQUIRES )
register_component()
target_link_libraries(${COMPONENT_TARGET} "-L${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(${COMPONENT_TARGET} algobsec)
