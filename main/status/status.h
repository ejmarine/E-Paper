#pragma once

#include "epaper_port.h"

void status_draw(int hr, int min, int bat);

void status_Task(void *arg);