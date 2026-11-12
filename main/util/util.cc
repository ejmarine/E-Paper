#include "util.h"

#include <stdlib.h>
#include <string.h>

#include "esp_log.h"

SemaphoreHandle_t epd_mutex = NULL;

static const char *TAG = "epd";

void epd_lock(void)   { 
    xSemaphoreTake(epd_mutex, portMAX_DELAY); 
}
void epd_unlock(void) { 
    xSemaphoreGive(epd_mutex); 
}

void display_partial_paint(UWORD x0, UWORD y0, UWORD x1, UWORD y1)
{
    UWORD W = Paint.WidthMemory, H = Paint.HeightMemory;
    UWORD px0, py0, px1, py1;

    switch (Paint.Rotate) {
    case ROTATE_90:  px0 = W - y1; px1 = W - y0; py0 = x0;     py1 = x1;     break;
    case ROTATE_180: px0 = W - x1; px1 = W - x0; py0 = H - y1; py1 = H - y0; break;
    case ROTATE_270: px0 = y0;     px1 = y1;     py0 = H - x1; py1 = H - x0; break;
    default:         px0 = x0;     px1 = x1;     py0 = y0;     py1 = y1;     break;
    }

    px0 &= ~7;
    px1 = (px1 + 7) & ~7;

    UWORD bytes = (px1 - px0) / 8;
    UWORD rows = py1 - py0;
    UBYTE *window = (UBYTE *)malloc((size_t)bytes * rows);
    if (window == NULL) {
        ESP_LOGE(TAG, "Partial window allocation failed");
        return;
    }
    for (UWORD r = 0; r < rows; r++) {
        memcpy(window + (size_t)r * bytes,
               Paint.Image + (size_t)(py0 + r) * Paint.WidthByte + px0 / 8,
               bytes);
    }
    EPD_Display_Partial(window, px0, py0, px1, py1);
    free(window);
}

int next_space(uint8_t* buf, int start, int max) {
    int ret = 0;
    for (int i = start+1; i < max + start; i++) {
        if ((char)buf[i] != ' ' && (char)buf[i] != '-') {
            ret++;
        }
        else {
            return ret;
        }
    }
    return ret;
}