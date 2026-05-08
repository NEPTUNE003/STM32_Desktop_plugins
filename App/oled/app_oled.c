#include "oled/app_oled.h" 
#include "oled/bsp_i2c_oled.h"
#include "fonts/bsp_fonts.h"
#include "dwt/bsp_dwt.h" 
#include "key/bsp_gpio_key.h"
#include "led/bsp_gpio_led.h"
#include "i2c/bsp_i2c.h"
#include "dht11/app_dht11.h"
#include <stdio.h>
#include <string.h>
#include "beep/bsp_gpio_beep.h"

#define BEEP_ON()   GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_OFF()  GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN)

uint8_t menu = 0;									// menu高4位: 0x00=HOME, 0x10=BUZZER, 0x20=TEMP, 0x30=LED, 0x40=COUNT
																	// menu低4位: 0=菜单列表, 1=通用子界面（除LED）, LED子界面:0x31=ON,0x32=OFF
																	//                        0x11=BUZZER子界面, 0x41=COUNT子界面,
uint8_t menu_show_flag = 0;
uint8_t content_show_flag = 0;
uint8_t led_state_flag = 0;              //1：关
uint32_t Count = 0;
uint8_t left_shift_flag = 0;
uint8_t right_shift_flag = 0;

static uint8_t morse_input_mode = 0;      // 1=输入模式
static char current_morse_seq[6] = "";    // 最多5个符号 + 结束符
static int last_digit = -1;               // 保存最近一次识别的数字，-1表示无效
static const char* morse_digits[] = {"-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

static void PlayMorseSymbol(char symbol) 
{
    if (symbol == '.') 
		{ 
			BEEP_ON(); 
			DWT_DelayMs(150); 
			BEEP_OFF(); 
			DWT_DelayMs(150); 
		}
    else if (symbol == '-') 
		{ 
			BEEP_ON(); 
			DWT_DelayMs(450); 
			BEEP_OFF(); 
			DWT_DelayMs(150); 
		}
}

static volatile uint8_t hello_playing = 0;
static volatile uint8_t hello_stop_request = 0;

// 播放 HELLO 摩斯码 (.... . .-.. .-.. ---)
static void PlayHelloMorse(void) {
    hello_playing = 1;
    hello_stop_request = 0;
    
    OLED_CLS();
    OLED_ShowString(1, 0, (uint8_t*)"HELLO:  .... .", TEXTSIZE_F8X16);
    OLED_ShowString(3, 0, (uint8_t*)".-.. .-.. ---", TEXTSIZE_F8X16);

    // H
    if (hello_stop_request) goto stop;
    PlayMorseSymbol('.'); PlayMorseSymbol('.'); PlayMorseSymbol('.'); PlayMorseSymbol('.'); DWT_DelayMs(300);
    // E
    if (hello_stop_request) goto stop;
    PlayMorseSymbol('.'); DWT_DelayMs(300);
    // L
    if (hello_stop_request) goto stop;
    PlayMorseSymbol('.'); PlayMorseSymbol('-'); PlayMorseSymbol('.'); PlayMorseSymbol('.'); DWT_DelayMs(300);
    // L
    if (hello_stop_request) goto stop;
    PlayMorseSymbol('.'); PlayMorseSymbol('-'); PlayMorseSymbol('.'); PlayMorseSymbol('.'); DWT_DelayMs(300);
    // O
    if (hello_stop_request) goto stop;
    PlayMorseSymbol('-'); PlayMorseSymbol('-'); PlayMorseSymbol('-');
    
stop:
    hello_playing = 0;
    BEEP_OFF();
		//重新显示提示文字，menu值未改变
    OLED_CLS();
    OLED_ShowString(1, 0, (uint8_t*)"K1:HELLO", TEXTSIZE_F8X16);
    OLED_ShowString(3, 0, (uint8_t*)"K2:NUM", TEXTSIZE_F8X16);
    
    content_show_flag = 1;  
}


static void StopHelloMorse(void) {
    if (hello_playing) {
        hello_stop_request = 1;
        while (hello_playing); // 等待播放完成退出
    }
}

// 将摩斯序列转换为数字
static int MorseSeqToDigit(const char* seq) 
{
    if (strlen(seq) != 5) return -1;
    for (int i = 0; i < 10; i++) {
        if (strcmp(seq, morse_digits[i]) == 0) return i;      //compare
    }
    return -1;
}

/* ================== 质数 ================== */
static uint8_t isPrime(uint32_t n) {
    if (n < 2) return 0;
    for (uint32_t i = 2; i * i <= n; i++) if (n % i == 0) return 0;
    return 1;
}
static uint32_t nextPrime(uint32_t n) {
    uint32_t num = n + 1;
    while (!isPrime(num)) num++;
    return num;
}

/* ================== 外设 ================== */
static void SetLedAndBeep(uint8_t state) {
    if (state) 
		{ 
			LED_OFF(LED2_GPIO_PORT, LED2_GPIO_PIN, LED_LOW_TRIGGER); 
			BEEP_OFF(); 
			led_state_flag = 1; 
		}
    else 
		{ 
			LED_ON(LED2_GPIO_PORT, LED2_GPIO_PIN, LED_LOW_TRIGGER); 
			BEEP_ON(); 
			led_state_flag = 0; 
		}
}

/* ================== 开机 ================== */
void Boot_Task(void) {
    OLED_CLS();
    OLED_ShowString(3, 3, (uint8_t*)"WeiJiKeShe", TEXTSIZE_F8X16);
    IIC_DELAY_US(3000000);
}

/* ================== 菜单 ================== */
void Menu_Task(void) {
    uint8_t key1 = KEY_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN, KEY_HIGH_TRIGGER);
    uint8_t key2 = KEY_Scan(KEY2_GPIO_PORT, KEY2_GPIO_PIN, KEY_HIGH_TRIGGER);
    uint8_t key3 = KEY_Scan(KEY3_GPIO_PORT, KEY3_GPIO_PIN, KEY_HIGH_TRIGGER);

      	// 如果不在摩斯输入模式        0X10：BUZZER
    if (  !  ((menu & 0xf0) == 0x10 && (menu & 0x0f) != 0 && morse_input_mode)) {
        if (key3 == KEY_DOWN) {
            OLED_CLS();
            if ((menu & 0x0f) == 0) {          //在菜单界面
                content_show_flag = 1;
                if (menu == 0x30) menu = led_state_flag ? 0x32 : 0x31;
                else menu = (menu & 0xf0) | 0x01;    //进入子界面
            } else {
                menu &= 0xf0;
                menu_show_flag = 1;
            }
        }
        if (key1 == KEY_DOWN) {
            if ((menu & 0x0f) == 0) {
                menu = (menu == 0x00) ? 0x40 : menu - 0x10;
                menu_show_flag = 1;
            } else {
                if (menu == 0x31 || menu == 0x32) menu = (menu == 0x31) ? 0x32 : 0x31;
                if (menu == 0x41) Count = nextPrime(Count);
                content_show_flag = 1;
            }
        }
        if (key2 == KEY_DOWN) {
            if ((menu & 0x0f) == 0) {
                menu = (menu == 0x40) ? 0x00 : menu + 0x10;
                menu_show_flag = 1;
            } else {
                if (menu == 0x31 || menu == 0x32) menu = (menu == 0x31) ? 0x32 : 0x31;
                if (menu == 0x41) Count = 0;
                content_show_flag = 1;
            }
        }
    }

    // 主菜单显示
    if ((menu & 0x0f) == 0 && menu_show_flag) {
        OLED_CLS();
        OLED_ShowString(1, 4, (uint8_t*)"MENU", TEXTSIZE_F8X16);
        switch (menu) {
            case 0x00: OLED_ShowString(3, 3, (uint8_t*)"HOME", TEXTSIZE_F8X16); break;
            case 0x10: OLED_ShowString(3, 3, (uint8_t*)"BUZZER", TEXTSIZE_F8X16); break;
            case 0x20: OLED_ShowString(3, 3, (uint8_t*)"TEMP", TEXTSIZE_F8X16); break;
            case 0x30: OLED_ShowString(3, 3, (uint8_t*)"LED", TEXTSIZE_F8X16); break;
            case 0x40: OLED_ShowString(3, 3, (uint8_t*)"COUNT", TEXTSIZE_F8X16); break;
        }
        menu_show_flag = 0;
    }

    // 子菜单内容（非摩斯模式）
    if (content_show_flag && (menu & 0x0f) != 0 && !morse_input_mode) {
        OLED_CLS();
        switch (menu & 0xf0) {
						case 0x00:  // HOME 子界面
								OLED_ShowString(3, 3, (uint8_t*)"HI", TEXTSIZE_F8X16);
								break;
            case 0x10:  // BUZZER 普通模式
                OLED_ShowString(1, 0, (uint8_t*)"K1:HELLO", TEXTSIZE_F8X16);
                OLED_ShowString(3, 0, (uint8_t*)"K2:NUM", TEXTSIZE_F8X16);
                if (key1 == KEY_DOWN) {
                    if (hello_playing) {     //hello_playing =    0 = 没有播放，1 = 正在播放
                        StopHelloMorse();      // 停止播放
                        content_show_flag = 1; 
                    } else {
                        PlayHelloMorse();      // 开始播放
                    }
                }
                if (key2 == KEY_DOWN) {
                    morse_input_mode = 1;
                    current_morse_seq[0] = '\0';
                    last_digit = -1;
                    content_show_flag = 1;
                    return;                //立即返回，避免本次按键被后续摩斯处理代码误识别为输入符号
                }
                break;
            case 0x20: {
                char t[16], h[16];     
								sprintf(t, "T:%d.%dC", dht11_data.temp_int, dht11_data.temp_deci);    //整数    小数
								sprintf(h, "H:%d.%d%%", dht11_data.humi_int, dht11_data.humi_deci);
                OLED_ShowString(1, 0, (uint8_t*)"TEMP", TEXTSIZE_F8X16);
                OLED_ShowString(3, 0, (uint8_t*)t, TEXTSIZE_F8X16);
                OLED_ShowString(5, 0, (uint8_t*)h, TEXTSIZE_F8X16);
                break;
            }
            case 0x30:
                OLED_ShowString(2, 0, (uint8_t*)"LED CTRL", TEXTSIZE_F8X16);
                if (menu == 0x31) {
                    OLED_ShowString(4, 0, (uint8_t*)"STATE: ON", TEXTSIZE_F8X16);
                    SetLedAndBeep(0);
                } else {
                    OLED_ShowString(4, 0, (uint8_t*)"STATE: OFF ", TEXTSIZE_F8X16);
                    SetLedAndBeep(1);
                }
                break;
            case 0x40: {
                char buf[16];
                sprintf(buf, "%lu", Count);                 //将 Count 转换为字符串，存入 buf
                OLED_ShowString(1, 0, (uint8_t*)"COUNT PRIME", TEXTSIZE_F8X16);
                OLED_ShowString(4, 0, (uint8_t*)buf, TEXTSIZE_F8X16);
                break;
            }
        }
        content_show_flag = 0;
    }

    /* ================== 摩斯码数字输入模式================== */
    if ((menu & 0xf0) == 0x10 && (menu & 0x0f) != 0 && morse_input_mode) {
        static uint8_t last_show = 0;

        // 已识别出数字且按下 K1 或 K2             重置状态，开始新一轮输入
        if (last_digit >= 0 && (key1 == KEY_DOWN || key2 == KEY_DOWN)) {
            last_digit = -1;
            current_morse_seq[0] = '\0';
            last_show = 0;
            content_show_flag = 1;  // 刷新屏幕
        }

        // "有刷新请求"或者"还没有显示过"       刷新屏幕显示
        if (content_show_flag || !last_show) {
            OLED_CLS();
            char num_buf[16];
            if (last_digit >= 0)
                sprintf(num_buf, "NUM:%d", last_digit);
            else
                sprintf(num_buf, "NUM:?");
            OLED_ShowString(2, 0, (uint8_t*)num_buf, TEXTSIZE_F8X16);
            OLED_ShowString(4, 0, (uint8_t*)current_morse_seq, TEXTSIZE_F8X16);
            last_show = 1;
            content_show_flag = 0;
        }

        if (key3 == KEY_DOWN) {
            morse_input_mode = 0;
            current_morse_seq[0] = '\0';
            last_digit = -1;
            content_show_flag = 1;
            last_show = 0;
            return;
        }

        // K1: 输入 '.'
        if (key1 == KEY_DOWN) {
            if (strlen(current_morse_seq) < 5) {
                strcat(current_morse_seq, ".");
            }
            last_show = 0;   // 标记需要刷新（但不立即清屏，等待循环统一刷新）
        }

        // K2: 输入 '-'
        if (key2 == KEY_DOWN) {
            if (strlen(current_morse_seq) < 5) {
                strcat(current_morse_seq, "-");
            }
            last_show = 0;
        }

        // 满5个符号立即识别（仅触发一次刷新）
        if (strlen(current_morse_seq) == 5) {
            int digit = MorseSeqToDigit(current_morse_seq);
            if (digit >= 0) {
                last_digit = digit;
                BEEP_ON(); DWT_DelayMs(100); BEEP_OFF();  
            } else {
                last_digit = -1;  // 无效序列 显示问号
            }
            last_show = 0;          
        }
    }
}