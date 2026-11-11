#include "books.h"

#include "epaper_port.h"
#include "button_bsp.h"
#include "menu.h"
#include "util.h"

#include <iostream>
#include <string>


static const char *TAG = "main";

file_entry_t* books_draw() {
    
    
    if (file_entry_t *entries = (file_entry_t*)malloc(16 * sizeof(file_entry_t))) {
        int n = list_dir_once("/sdcard", entries, 16);

        for (int i = 0; i < n; i++) {
            Paint_DrawString_EN(50, i*40+140, entries[i].name, &Font16, WHITE, BLACK);
        }

        return entries;
    }

    return NULL;

}


int book_load(uint8_t* buf, size_t len, file_entry_t file) {

    std::string filename = "/sdcard/";
    filename += file.name;

    if (sd_read_file_to_buffer(filename.c_str(), buf, len)) {
        ESP_LOGE(TAG, "Successfuly Read File %s", filename.c_str());
        return 1;
    }
    else {
        ESP_LOGE(TAG, "Could not read %s %d", filename.c_str(), len);
        return 0;
    }
}

void book_draw(uint8_t* buf, size_t len) {
    char glyph[2] = {'\0', '\0'};
    for (size_t i = 0; i < len && buf[i] != 0 && buf[i] != 0xFF; i++) {
        glyph[0] = (char)buf[i];
        Paint_DrawString_EN((UWORD)(i * 17 + 10), 40, glyph, &Font16, WHITE, BLACK);
    }
}

void books_loop(UBYTE *Image_Mono){
    int input;
    int selection = 0;
    int page_num = 0;

    ESP_LOGE(TAG, "Entered Books Loop");

    uint8_t page_buf[256] = {};

    page_wipe();
    menu_draw(Image_Mono, 1);
    select_draw(Image_Mono, 1, selection);
    file_entry_t *entries = books_draw();
    epd_lock();
    EPD_Display_Partial(Image_Mono, 0, 0, 800, 480);
    epd_unlock();

    if (!entries) {
        ESP_LOGE(TAG, "Failed to read files");
        return;
    }

    while (1) {
        input = wait_key_event_and_return_code(pdMS_TO_TICKS(1000));

        ESP_LOGE(TAG, "%d", input);

        if (input == 14) {
            if (!page_buf[0]) {
                selection++;
                select_draw(Image_Mono, 1, selection);
                epd_lock();
                EPD_Display_Partial(Image_Mono, 0, 0, 800, 480);
                epd_unlock();
            }
        }
        else if (input == 0) {
            if (!page_buf[0]) {
                selection--;
                select_draw(Image_Mono, 1, selection);
                epd_lock();
                EPD_Display_Partial(Image_Mono, 0, 0, 800, 480);
                epd_unlock();
            }
        }
        else if (input == 7) {
            
            if (book_load(page_buf, sizeof(page_buf), entries[selection])) {
                page_wipe();
                book_draw(page_buf, sizeof(page_buf));
                epd_lock();
                EPD_Display_Base(Image_Mono);
                epd_unlock();
            }
        }
        else if (input == 21) {
            return;
        }
    }
}