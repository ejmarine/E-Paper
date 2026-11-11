#pragma once

#include "epaper_port.h"
#include "sdcard_bsp.h"

file_entry_t* books_draw();
void books_loop(UBYTE *image);
void book_draw(uint8_t *buf, size_t len);
int book_page_load(uint8_t *buf, int page, file_entry_t file);
void read_loop(uint8_t *buf, file_entry_t file);
