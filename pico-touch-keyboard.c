#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lib/lcd/LCD_Driver.h"
#include "LCD_Touch.h"
#include "LCD_BMP.h"

#include "tusb.h"

int lcd_test(void);

int main()
{
    stdio_init_all();
    tusb_init();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Example to turn on the Pico W LED
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    while (1)
    {
        tud_task();
        lcd_test();
    }
    return 0;
}

int lcd_test(void)
{
    uint8_t counter = 0;

    System_Init();
    SD_Init();
    LCD_SCAN_DIR lcd_scan_dir = SCAN_DIR_DFT;
    LCD_Init(lcd_scan_dir, 800);
    TP_Init(lcd_scan_dir);

    GUI_Show();
    Driver_Delay_ms(10000);

    LCD_Show_bmp(lcd_scan_dir);
    Driver_Delay_ms(2000);

    TP_GetAdFac();
    TP_Dialog(lcd_scan_dir);
    while (1)
    {
        TP_DrawBoard(lcd_scan_dir);
    }

    return 0;
}
