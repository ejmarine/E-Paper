#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "axp_prot.h"
#include "button_bsp.h"
#include "epaper_port.h"
#include "i2c_bsp.h"
#include "pcf85063_bsp.h"
#include "qmi8658_bsp.h"
#include "sdcard_bsp.h"
#include "shtc3_bsp.h"

#include "menu.h"
#include "status.h"
#include "books.h"
#include "util.h"


// Create a mutex lock to protect the device from interference when reading
SemaphoreHandle_t alarm_mutex = NULL; // protect alarms
SemaphoreHandle_t rtc_mutex = NULL;   // protect RTC
SemaphoreHandle_t nvs_mutex = NULL;   // protect NVS
SemaphoreHandle_t qmi8658_mutex = NULL;   // protect qmi8658

// The wifi is on indicator
bool wifi_enable;      


// The priority of creating thread tasks
#define STATUS_TASK_PRIO   5
#define USER_TASK_PRIO    3

// Quantity of individual main courses
#define HOME_PAGE_NUM   8

// E-ink screen sleep time (S)
#define EPD_Sleep_Time   60
// Equipment shutdown time (minutes)
#define Unattended_Time  10

// Icon location
#define Icon_X_1   77
#define Icon_X_2   307
#define Icon_Y_1   78
#define Icon_Y_2   264
#define Icon_Y_3   450
#define Icon_Y_4   636

#define Text_X_1   Icon_X_1 + 48
#define Text_X_2   Icon_X_2 + 48
#define Text_Y_1   Icon_Y_1 + 100
#define Text_Y_2   Icon_Y_2 + 100
#define Text_Y_3   Icon_Y_3 + 100
#define Text_Y_4   Icon_Y_4 + 100


// Define the data cache area of the e-ink screen
uint8_t *Image_Mono;

// Log tag
static const char *TAG = "main";

// The main page displays
void esp_home(int selection, int Refresh_mode)
{
    
}

// Main menu task
void user_Task(void *arg)
{

    int input;
    int selection = 1;

    ESP_LOGE(TAG, "Attempting to update display...");
    if (Image_Mono == NULL) {
        ESP_LOGE(TAG, "No framebuffer, skipping display update");
        vTaskDelete(NULL);
    }

    ESP_LOGE(TAG, "Attempting to add debug file...");
    uint8_t buf[] = {116, 101, 115, 116};
    sd_write_buffer_to_file("/sdcard/atest", buf, sizeof(buf));

    epd_lock();
    Paint_NewImage(Image_Mono, EPD_WIDTH, EPD_HEIGHT, ROTATE_270, WHITE);
    Paint_SetScale(2);
    Paint_SelectImage(Image_Mono);
    Paint_Clear(WHITE);

    menu_draw(Image_Mono, 0);
    select_draw(Image_Mono, 0, selection);

    EPD_Display_Base(Image_Mono);
    epd_unlock();


    while (1) {
        input = wait_key_event_and_return_code(pdMS_TO_TICKS(1000));

        ESP_LOGE(TAG, "%d", input);

        if (input == 14) {

            selection++;
            select_draw(Image_Mono, 0, selection);
            epd_lock();
            EPD_Display_Partial(Image_Mono, 0, 0, 800, 480);
            epd_unlock();
        }
        else if (input == 0) {
            selection--;
            select_draw(Image_Mono, 0, selection);
            epd_lock();
            EPD_Display_Partial(Image_Mono, 0, 0, 800, 480);
            epd_unlock();
        }
        else if (input == 7) {
            if (selection == 1) {
                books_loop(Image_Mono);
                selection = 1;
                page_wipe();
                select_wipe(1);
                menu_draw(Image_Mono, 0);
                select_draw(Image_Mono, 0, selection);
                epd_lock();
                EPD_Display_Base(Image_Mono);
                epd_unlock();
            }
        }
    }
}


extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_LOGE(TAG,"Hello world!\n");

    // Initialize the SD card
    _sdcard_init();
    i2c_master_init();   // Initialize the I2C bus
    vTaskDelay(pdMS_TO_TICKS(50)); 
    i2c_devices_init();  // Initialize all I2C devices
    vTaskDelay(pdMS_TO_TICKS(50));

    // Initialize the RTC
    PCF85063_init();
    // PCF85063_alarm_Time_Disable();
    // PCF85063_clear_alarm_flag();

    // Initialize the power supply and perform mode judgment simultaneously
    axp_init();
    // The program officially begins to execute
    // ESP_LOGE(TAG,"Hello world!\n");
    // Initialize key
    button_Init();

    // Initialize the temperature and humidity sensor
    i2c_shtc3_init();

    // E-ink screen pin initialization
    epaper_port_init();
    // EPD_display_BMP(NULL);
    // vTaskDelay(pdMS_TO_TICKS(500));

    // EPD_Init
    EPD_Init();
    // Create a data cache area for the e-paper
    Image_Mono = (UBYTE *)malloc(EPD_SIZE_MONO);
    if (Image_Mono == NULL) {
        ESP_LOGE(TAG, "Failed to apply for black memory...");
    }

    // Clear the alarm clock
    PCF85063_clear_alarm_flag();    
    PCF85063_alarm_Time_Disable();
    PCF85063_clear_alarm_flag();

    // Initialize the six-axis sensor
    QMI8658_init();

    // Initialize the mutex lock
    alarm_mutex = xSemaphoreCreateMutex();
    rtc_mutex = xSemaphoreCreateMutex();
    qmi8658_mutex = xSemaphoreCreateMutex();
    epd_mutex = xSemaphoreCreateMutex();

    // Check whether the creation was successful
    assert(alarm_mutex != NULL);
    assert(rtc_mutex != NULL);
    assert(epd_mutex != NULL);

    // Create the main menu task
    xTaskCreate(user_Task, "user_Task", 64 * 1024, NULL, USER_TASK_PRIO, NULL);
    xTaskCreate(status_Task, "status_Task", 64 * 1024, NULL, STATUS_TASK_PRIO, NULL);

    ESP_LOGE(TAG, "Initialization Successful!");

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }

    heap_caps_free(Image_Mono);
    fflush(stdout);
    // esp_restart();
}
