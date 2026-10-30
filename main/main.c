#include <stdlib.h>
#include "epaper_bsp.h"
#include "epaper_port.h"
#include "esp_log.h"

static const char *TAG = "main";

uint8_t *Image_Mono;

void app_main(void) {
    epaper_port_init();
    EPD_Init_Fast();

    if ((Image_Mono = malloc(EPD_SIZE_MONO)) == NULL) {
        ESP_LOGE(TAG, "Failed to allocate image buffer");
        return;
    }

    Paint_NewImage(Image_Mono, EPD_WIDTH, EPD_HEIGHT, 0, WHITE);
    Paint_Clear(WHITE);
    Paint_ReadBmp(gImage_mine, 0, 0, GIMAGE_MINE_WIDTH, GIMAGE_MINE_HEIGHT);
    EPD_Display_Fast(Image_Mono);


}
