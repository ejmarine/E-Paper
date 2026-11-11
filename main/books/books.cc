#include "books.h"

#include "epaper_port.h"
#include "button_bsp.h"
#include "menu.h"
#include "util.h"

#include <iostream>
#include <string>


static const char *TAG = "main";
const int WIDTH_CHARS = 27; // number of chararcters displayed on one line
const int HEIGHT_CHARS = 24; // number of lines displayed on one page
const int PAGE_LEN = WIDTH_CHARS * HEIGHT_CHARS;

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


int book_page_load(uint8_t* buf, int page_num, file_entry_t file) {

    std::string filename = "/sdcard/";
    filename += file.name;

    if (s_example_read_from_offset(filename.c_str(), (char*)buf, PAGE_LEN, page_num * PAGE_LEN)) {
        ESP_LOGE(TAG, "Successfuly Read File %s", filename.c_str());
        return 1;
    }
    else {
        ESP_LOGE(TAG, "Could not read %s %d", filename.c_str(), PAGE_LEN * page_num);
        return 0;
    }
}

void book_draw(uint8_t* buf, size_t len) {
    char glyph[2] = {'\0', '\0'};
    for (size_t i = 0; i < HEIGHT_CHARS; i++) {
        for (size_t j = 0; j < WIDTH_CHARS && buf[j*i] != 0 && buf[j*i] != 0xFF; j++) {
        glyph[0] = (char)buf[j + (i * WIDTH_CHARS)];
        Paint_DrawString_EN((UWORD)(j * 17 + 10), i * 30 + 40, glyph, &Font16, WHITE, BLACK);
        }
    }
}

void books_loop(UBYTE *Image_Mono){
    int input;
    int selection = 0;
    int page_num = 0;

    ESP_LOGE(TAG, "Entered Books Loop");

    uint8_t page_buf[PAGE_LEN] = {};

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

            read_loop(Image_Mono, entries[selection]);
            
            selection = 0;
            page_wipe();
            menu_draw(Image_Mono, 1);
            select_draw(Image_Mono, 1, selection);
            file_entry_t *entries = books_draw();
            epd_lock();
            EPD_Display_Partial(Image_Mono, 0, 0, 800, 480);
            epd_unlock();
        }
        else if (input == 21) {
            return;
        }
    }
}

void read_loop(UBYTE *Image_Mono, file_entry_t file) {
    int input;
    int page_num = 0;

    ESP_LOGE(TAG, "Entered Reading Loop");

    uint8_t page_buf[PAGE_LEN] = {};

    if (!book_page_load(page_buf, page_num, file)) {
        return;
    }

    page_wipe();
    book_draw(page_buf, page_num);
    epd_lock();
    EPD_Display_Base(Image_Mono);
    epd_unlock();

    while (1) {
        input = wait_key_event_and_return_code(pdMS_TO_TICKS(1000));

        ESP_LOGE(TAG, "%d", input);

        if (input == 14) {
            page_num++;
            if (!book_page_load(page_buf, page_num, file)) {
                return;
            }
            book_draw(page_buf, PAGE_LEN);
            epd_lock();
            EPD_Display_Base(Image_Mono);
            epd_unlock();

        }
        else if (input == 0) {
            page_num--;
            if (!book_page_load(page_buf, page_num, file)) {
                return;
            }
            book_draw(page_buf, PAGE_LEN);
            epd_lock();
            EPD_Display_Base(Image_Mono);
            epd_unlock();
        }
        else if (input == 21) {
            return;
        }
    }
}