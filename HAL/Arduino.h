#ifndef __ARDUINO_H__
#define __ARDUINO_H__
enum { INPUT, OUTPUT };
enum { LOW, HIGH };
void digitalWrite(int pin, int value);
int digitalRead(int pin);
void pinMode(int pin, int mode);
void delay(int ms);

typedef struct {
  void (*begin)(int baud);
  void (*print)(const char *str);
  void (*println)(const char *str);
} Serial_t;
extern Serial_t Serial;

#endif
