#include "menu.h"

#include "epaper_port.h"

void page_wipe(){
    Paint_DrawRectangle(0, 31, 480, 800, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
}

void select_wipe(int page){
    if (page == 1) {
        Paint_DrawRectangle(0, 140, 50, 800, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    }

}

void menu_draw(UBYTE *image, int page) {
    page_wipe();
    if (page == 0) {
        Paint_DrawString_EN(30, 60, "Welcome Back!", &Font36, WHITE, BLACK);
        Paint_DrawString_EN(120, 400, "Books", &Font28, WHITE, BLACK);
        Paint_DrawString_EN(120, 475, "Download", &Font28, WHITE, BLACK);
        Paint_DrawString_EN(120, 550, "Settings", &Font28, WHITE, BLACK);
    }
    else if (page == 1) {
        Paint_DrawString_EN(30, 60, "Select a Book!", &Font36, WHITE, BLACK);
    }
}

void select_draw(UBYTE *image, int page, int select) {
    if (page == 0) {
        Paint_DrawLine(85, 415, 100, 432, select == 1 ? BLACK : WHITE, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
        Paint_DrawLine(85, 450, 100, 432, select == 1 ? BLACK : WHITE, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
        Paint_DrawLine(85, 490, 100, 507, select == 2 ? BLACK : WHITE, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
        Paint_DrawLine(85, 525, 100, 507, select == 2 ? BLACK : WHITE, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
        Paint_DrawLine(85, 550, 100, 567, select == 3 ? BLACK : WHITE, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
        Paint_DrawLine(85, 585, 100, 567, select == 3 ? BLACK : WHITE, DOT_PIXEL_3X3, LINE_STYLE_SOLID);
    }
    else if (page == 1) {
        select_wipe(page);
        Paint_DrawLine(15, select * 40 + 150, 30, select * 40 + 155, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
        Paint_DrawLine(15, select * 40 + 160, 30, select * 40 + 155, BLACK, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
    }
}