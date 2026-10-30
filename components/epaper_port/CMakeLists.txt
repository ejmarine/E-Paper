idf_component_register(
  SRCS "epaper_bsp.c" "epaper_port.c" "ImageData.c" "my_image.c"
  PRIV_REQUIRES esp_driver_gpio esp_driver_spi epaper_lib
  INCLUDE_DIRS "./")
