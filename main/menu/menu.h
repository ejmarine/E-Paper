#pragma once

#include "epaper_port.h"

void page_wipe();

void select_wipe(int page);

void menu_draw(UBYTE *image, int page);

void select_draw(UBYTE *image, int page, int state);