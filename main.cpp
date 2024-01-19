#include "./epd_views/views.h"
#include <GUI_Paint.h>
#include <display.h>

void display() {
  // create_view_bw(time_view_bw, 0);
  create_view_bw(txt_reader, 0);
}

int main(int argc, char *argv[]) {
  DISPLAY_Init(EPD_WIDTH, EPD_HEIGHT, 90);
  // DISPLAY_Init(EPD_HEIGHT, EPD_WIDTH);
  DISPLAY_Loop(display);

  return 0;
}
