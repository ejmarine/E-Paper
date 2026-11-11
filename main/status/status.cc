#include <iostream>
#include <string>

#include "status.h"
#include "axp_prot.h"
#include "epaper_port.h"
#include "util.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

void status_draw(int hr, int min, int bat) {
    
    std::string time = (hr < 10 ? "0" + std::to_string(hr) : std::to_string(hr)) + ":" + (min < 10 ? "0" + std::to_string(min) : std::to_string(min));
    std::string batp = std::to_string(bat) + "%";

    ESP_LOGE(TAG, "time: %s", time.c_str());
    ESP_LOGE(TAG, "batp: %s", batp.c_str());


    Paint_DrawLine(0, 30, 480, 30, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    Paint_DrawString_EN(200, 0, time.c_str(), &Font16, WHITE, BLACK);
    Paint_DrawString_EN(410, 0, batp.c_str(), &Font16, WHITE, BLACK);
}

void status_wipe() {
    Paint_DrawRectangle(0, 0, 480, 29, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
}

void status_Task(void *arg) {
    while (Paint.Image == NULL) {
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    int time = 0;
    while (1) {
        epd_lock();
        status_wipe();
        status_draw(18, time, get_battery_power());
        time = (time + 1) % 60;
        display_partial_paint(0, 0, 480, 32);
        epd_unlock();
        vTaskDelay(pdMS_TO_TICKS(60 * 1000));
    }
}