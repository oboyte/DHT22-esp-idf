idf_component_register(SRCS "DHT22.cpp"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_driver_gpio
                    )

                  #target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++23)
