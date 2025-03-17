#include "DEV_Config.h"
#include "EPD_7in3f.h"
#include <stdlib.h> 
#include "GUI_Paint.h"
#include "read_com.h"

#define IMAGE_WIDTH  EPD_7IN3F_WIDTH
#define IMAGE_HEIGHT EPD_7IN3F_HEIGHT


bool read_image_data_from_com() {
    printf("Reading bytes and updating display...\n");

    int x = 0, y = 0;

    while(true) {
        int ch = getchar_timeout_us(1000);  // Read with timeout

        if (ch == PICO_ERROR_TIMEOUT) {
            printf("Timeout waiting for pixel data.\n");
            if (y < 100) {
                printf("Partial image received; ending early.\n");
                return READ_COM_DATA_FAIL;
            } else {
                return 0;
            }
        }

        unsigned char color = (unsigned char)ch;  // Convert byte to color

        UDOUBLE Addr = x + y * Paint.WidthByte;
        Paint.Image[Addr] = (unsigned char)color;

        x++;
        if (x >= Paint.WidthByte) {
            x = 0;
            y++;
            if (y >= Paint.HeightByte) {
                break;  // Stop if we reach display height
            }
        }
    }

    printf("Finished updating display.\n");
    return 0;
}
