#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "epaper_port.h"

extern SemaphoreHandle_t epd_mutex;

void epd_lock(void);
void epd_unlock(void);

/* Refresh a rectangle given in Paint coordinates, end exclusive. */
void display_partial_paint(UWORD x0, UWORD y0, UWORD x1, UWORD y1);
int next_space(uint8_t* buf, int start, int max);