#include <Arduino.h>
#include <stdio.h>
#include <unistd.h>
// 颜色输出宏定义
#define COLOR_NONE "\033[m"
#define COLOR_RED "\033[0;32;31m"
#define COLOR_GREEN "\033[0;32;32m"
#define COLOR_YELLOW "\033[1;33m"

#define INFO_COLOR COLOR_GREEN
#define ERROR_COLOR COLOR_RED
#define WARNING_COLOR COLOR_YELLOW

#define INFO(fmt, ...)                                                         \
  printf(INFO_COLOR "[INFO] " fmt COLOR_NONE "\n", ##__VA_ARGS__)

#define ERROR(fmt, ...)                                                        \
  printf(ERROR_COLOR "[ERROR] " fmt COLOR_NONE "\n", ##__VA_ARGS__)

#define PRINT(fmt, ...)                                                        \
  printf(WARNING_COLOR "[Serial] " fmt COLOR_NONE "\n", ##__VA_ARGS__)

static void begin(int baud) { PRINT("Serial begin at baud:%d", baud); }
static void print(const char *str) { PRINT("%s", str); }
static void println(const char *str) { PRINT("%s", str); }

Serial_t Serial = {begin, print, println};

void digitalWrite(int pin, int value) { INFO("Pin %d set to %d", pin, value); }

int digitalRead(int pin) {
  INFO("Pin %d read value %d", pin, 1);
  return 1;
}

void pinMode(int pin, int mode) { INFO("Pin %d set to mode %d", pin, mode); }

void delay(int ms) {
  INFO("Delay %d ms", ms);
  usleep(ms * 1000);
}
