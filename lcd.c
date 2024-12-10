#include "lcd.h"

LCD_SCAN_DIR lcd_scan_dir = D2U_L2R;

const char *keyboard_layout[5][10] = {
    {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
    {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
    {"A", "S", "D", "F", "G", "H", "J", "K", "L", ";"},
    {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"},
    {" ", " ", " ", " ", " ", " ", " ", " ", " ", " "}};
int x_sep = (LCD_3_5_WIDTH / 5);
int y_sep = (LCD_3_5_HEIGHT / 3);

void draw_key(uint16_t x, uint16_t y, uint16_t color);

int lcd_test(void)
{
    uint8_t counter = 0;

    System_Init();
    SD_Init();
    LCD_Init(lcd_scan_dir, 800);
    TP_Init(lcd_scan_dir);

    // GUI_Show();
    // Driver_Delay_ms(10000);

    // LCD_Show_bmp(lcd_scan_dir);
    // Driver_Delay_ms(2000);

    TP_GetAdFac();
    // TP_Dialog(lcd_scan_dir);
    draw_keyboard();

    return 0;
}

void draw_keyboard(void)
{
    GUI_Clear(BLACK);
    for (size_t x = 0; x < 10; x++)
    {
        for (size_t y = 0; y < 5; y++)
        {
            draw_key(x, y, BLACK);
        }
    }
}

void draw_key(uint16_t x, uint16_t y, uint16_t color)
{
    if (x_sep == 100 || y_sep == 100)
    {
        return;
    }

    GUI_DrawRectangle(x * x_sep + 1,
                      y * y_sep + 1,
                      (x * x_sep) + x_sep,
                      (y * y_sep) + y_sep,
                      color, DRAW_FULL, DOT_PIXEL_1X1);
    GUI_DrawRectangle(x * x_sep + 1,
                      y * y_sep + 1,
                      (x * x_sep) + x_sep,
                      (y * y_sep) + y_sep,
                      YELLOW, DRAW_EMPTY, DOT_PIXEL_1X1);
    GUI_DisString_EN(x * x_sep + (x_sep / 2) - 12, y * y_sep + (y_sep / 2) - 7, keyboard_layout[y][x], &Font24, LCD_BACKGROUND, BLUE);
}

static uint16_t prev_x = 100;
static uint16_t prev_y = 100;
static bool prev_status = false;
#define UP false
#define DOWN true

void read_keyboard(uint32_t blink_interval_ms)
{
    uint16_t screen_x, screen_y;
    int status = Touch_Scan(&screen_x, &screen_y);
    uint16_t x = screen_x / x_sep;
    uint16_t y = screen_y / y_sep;

    if (status == DOWN)
    {
        if ((x != prev_x) || (y != prev_y))
        {
            draw_key(prev_x, prev_y, BLACK);
            draw_key(x, y, WHITE);
        }
    }
    else if ((status == UP) && (prev_status == DOWN))
    {
        draw_key(prev_x, prev_y, BLACK);
        draw_key(x, y, BLACK);
        x = 100;
        y = 100;
    }
    else
    {
        x = 100;
        y = 100;
    }

    prev_x = x;
    prev_y = y;
    prev_status = status;

    // if (status != prev_status)
    // {

    //     GUI_DrawRectangle(0, 0, LCD_3_5_WIDTH, 24, BLACK, DRAW_FULL, DOT_PIXEL_1X1);
    //     char buf[400];
    //     sprintf(buf, "x: %d, y: %d - %s - %d", x, y, (status) ? "DOWN" : "UP", blink_interval_ms);
    //     GUI_DisString_EN(0, 0, buf, &Font24, LCD_BACKGROUND, WHITE);

    //     if (status == DOWN)
    //     {
    //         GUI_DrawRectangle(x * x_sep, y * y_sep, (x * x_sep) + x_sep, (y * y_sep) + y_sep, WHITE, DRAW_FULL, DOT_PIXEL_1X1);
    //     }

    //     if (status == UP)
    //     {
    //         GUI_DrawRectangle(x * x_sep, y * y_sep, (x * x_sep) + x_sep, (y * y_sep) + y_sep, BLACK, DRAW_FULL, DOT_PIXEL_1X1);
    //     }
    //     prev_status = status;
    // }

    // TP_Scan(0);
    // if (sTP_DEV.chStatus & TP_PRESS_DOWN)
    // { // Press the button
    //     int x = sTP_Draw.Xpoint;
    //     int y = sTP_Draw.Ypoint;
    //     GUI_DrawCircle(x, y, 3, GREEN, DRAW_FULL, DOT_FILL_RIGHTUP);
    // }
}