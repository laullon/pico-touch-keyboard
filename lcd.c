#include "lcd.h"
#include "main.h"

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
                      (x * x_sep) + x_sep - 3,
                      (y * y_sep) + y_sep - 3,
                      color, DRAW_FULL, DOT_PIXEL_1X1);
    GUI_DrawRectangle(x * x_sep + 1,
                      y * y_sep + 1,
                      (x * x_sep) + x_sep,
                      (y * y_sep) + y_sep,
                      YELLOW, DRAW_EMPTY, DOT_PIXEL_1X1);
    GUI_DisString_EN(x * x_sep + (x_sep / 2) - 12, y * y_sep + (y_sep / 2) - 7, keyboard_layout[y][x], &Font24, LCD_BACKGROUND, WHITE ^ color);
}

static uint16_t prev_x = 100;
static uint16_t prev_y = 100;

static uint64_t total_screen_x = 0;
static uint64_t total_screen_y = 0;
static uint64_t total = 0;

static bool prev_status = false;
#define UP false
#define DOWN true

void read_keyboard(uint32_t blink_interval_ms)
{

    // if (blink_interval_ms == BLINK_NOT_MOUNTED)
    // {
    //     GUI_DrawCircle(6, 6, 3, RED, DRAW_FULL, DOT_FILL_RIGHTUP);
    // }
    // else if (blink_interval_ms == BLINK_SUSPENDED)
    // {
    //     GUI_DrawCircle(6, 6, 3, YELLOW, DRAW_FULL, DOT_FILL_RIGHTUP);
    // }
    // else
    // {
    //     GUI_DrawCircle(6, 6, 3, GREEN, DRAW_FULL, DOT_FILL_RIGHTUP);
    // }

    // GUI_DrawCircle(6, blink_interval_ms / 10, 3, BLUE, DRAW_FULL, DOT_FILL_RIGHTUP);
    GUI_DrawCircle(16, BLINK_NOT_MOUNTED / 10, 3, WHITE, DRAW_FULL, DOT_FILL_RIGHTUP);
    GUI_DrawCircle(16, BLINK_MOUNTED / 10, 3, WHITE, DRAW_FULL, DOT_FILL_RIGHTUP);
    GUI_DrawCircle(16, BLINK_SUSPENDED / 10, 3, WHITE, DRAW_FULL, DOT_FILL_RIGHTUP);

    uint16_t screen_x, screen_y;
    int status = Touch_Scan(&screen_x, &screen_y);

    if (status == DOWN)
    {
        total_screen_x += screen_x;
        total_screen_y += screen_y;
        total++;

        screen_x = total_screen_x / total;
        screen_y = total_screen_y / total;

        uint16_t x = screen_x / x_sep;
        uint16_t y = screen_y / y_sep;

        // GUI_DrawCircle(screen_x, screen_y, 3, GREEN, DRAW_FULL, DOT_FILL_RIGHTUP);
        if (total > 20)
        {
            if ((x != prev_x) || (y != prev_y))
            {
                draw_key(prev_x, prev_y, BLACK);
                draw_key(x, y, WHITE);
            }

            prev_x = x;
            prev_y = y;
        }
    }
    else if ((status == UP) && (prev_status == DOWN))
    {
        draw_key(prev_x, prev_y, BLACK);
        prev_x = 100;
        prev_y = 100;

        total_screen_x = 0;
        total_screen_y = 0;
        total = 0;
    }

    prev_status = status;

    // GUI_DrawRectangle(0, 0, LCD_3_5_WIDTH, 24, BLACK, DRAW_FULL, DOT_PIXEL_1X1);
    // char buf[400];
    // sprintf(buf, "%f - %f - %f", blink_interval_ms, BLINK_SUSPENDED, (blink_interval_ms == BLINK_SUSPENDED));
    // GUI_DisString_EN(0, 0, buf, &Font24, LCD_BACKGROUND, WHITE);

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