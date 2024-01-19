#include "views.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>

// 屏幕方向
struct {
  int width;
  int height;
  int rotation;
} screen = {EPD_WIDTH, EPD_HEIGHT, 0};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture_bw = NULL;
SDL_Texture *texture_r = NULL;
void DISPLAY_Init(int width, int height, int rotation = 0) {
  // SDL2 Initialization
  SDL_Init(SDL_INIT_VIDEO);
  screen.rotation = rotation;
  if (screen.rotation == 90 || screen.rotation == 270) {
    screen.width = height;
    screen.height = width;
    window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, height, width, 0);
  } else {
    screen.width = width;
    screen.height = height;
    window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height, 0);
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

// void renderScreen(uint8_t *screen_bw, uint8_t *screen_r) {
//   // Render screen_bw as black and white
//   for (int y = 0; y < EPD_HEIGHT; ++y) {
//     for (int x = 0; x < EPD_WIDTH; ++x) {
//       uint8_t bit = *screen_bw & (0x80 >> (x % 8));
//       if (bit == 0)
//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//       else
//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//       SDL_RenderDrawPoint(renderer, x, y);
//       if (x % 8 == 7)
//         screen_bw++;
//     }
//     if (EPD_WIDTH % 8 != 0)
//       screen_bw++;
//   }
//   // Render screen_r as red where applicable
//   for (int y = 0; y < EPD_HEIGHT; ++y) {
//     for (int x = 0; x < EPD_WIDTH; ++x) {
//       uint8_t bit = *screen_r & (0x80 >> (x % 8));
//       // uint8_t byte = screen_bw[y * width + x / 8];
//       if (bit == 0) {
//         SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//         SDL_RenderDrawPoint(renderer, x, y);
//       }
//       if (x % 8 == 7)
//         screen_r++;
//     }
//     if (EPD_WIDTH % 8 != 0)
//       screen_r++;
//   }
//   // Present the renderer
//   SDL_RenderPresent(renderer);
// }
void renderScreen_bw(uint8_t *screen_bw) {
  // Create SDL surface for both screen_bw and screen_r
  SDL_Surface *combinedSurface = SDL_CreateRGBSurfaceWithFormat(
      0, EPD_WIDTH, EPD_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
  SDL_LockSurface(combinedSurface);
  // Render screen_bw on the left side
  for (int y = 0; y < EPD_HEIGHT; ++y) {
    for (int x = 0; x < EPD_WIDTH; ++x) {
      uint8_t bit = *screen_bw & (0x80 >> (x % 8));
      uint32_t *pixel =
          (uint32_t *)combinedSurface->pixels + y * combinedSurface->w + x;
      if (bit == 0)
        *pixel = SDL_MapRGBA(combinedSurface->format, 0, 0, 0, 255);
      else
        *pixel = SDL_MapRGBA(combinedSurface->format, 255, 255, 255, 255);
      if (x % 8 == 7)
        screen_bw++;
    }
    if (EPD_WIDTH % 8 != 0)
      screen_bw++;
  }

  SDL_UnlockSurface(combinedSurface);
  // Create SDL texture from combined surface
  SDL_Texture *combinedTexture =
      SDL_CreateTextureFromSurface(renderer, combinedSurface);
  // 旋转屏幕
  SDL_Rect dstrect;
  if (screen.rotation == 0) {
    dstrect = {0, 0, EPD_WIDTH, EPD_HEIGHT};
  } else if (screen.rotation == 90) {
    dstrect = {0, screen.height, EPD_WIDTH, EPD_HEIGHT};
  } else if (screen.rotation == 180) {
    dstrect = {screen.width, screen.height, EPD_WIDTH, EPD_HEIGHT};
  } else if (screen.rotation == 270) {
    dstrect = {screen.width, 0, EPD_WIDTH, EPD_HEIGHT};
  } else {
    printf("Unsupported rotation %d\n", screen.rotation);
  }
  // 指定旋转中心
  SDL_Point center = {0, 0};
  // 旋转绘图
  SDL_RenderCopyEx(renderer, combinedTexture, NULL, &dstrect, -screen.rotation,
                   &center, SDL_FLIP_NONE);

  // Destroy the surface and texture
  SDL_FreeSurface(combinedSurface);
  SDL_DestroyTexture(combinedTexture);

  // Present the renderer
  SDL_RenderPresent(renderer);
}

void renderScreen_bwr(uint8_t *screen_bw, uint8_t *screen_r) {
  // Create SDL surface for both screen_bw and screen_r
  SDL_Surface *combinedSurface = SDL_CreateRGBSurfaceWithFormat(
      0, EPD_WIDTH, EPD_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
  SDL_LockSurface(combinedSurface);
  // Render screen_bw on the left side
  for (int y = 0; y < EPD_HEIGHT; ++y) {
    for (int x = 0; x < EPD_WIDTH; ++x) {
      uint8_t bit = *screen_bw & (0x80 >> (x % 8));
      uint32_t *pixel =
          (uint32_t *)combinedSurface->pixels + y * combinedSurface->w + x;
      if (bit == 0)
        *pixel = SDL_MapRGBA(combinedSurface->format, 0, 0, 0, 255);
      else
        *pixel = SDL_MapRGBA(combinedSurface->format, 255, 255, 255, 255);
      if (x % 8 == 7)
        screen_bw++;
    }
    if (EPD_WIDTH % 8 != 0)
      screen_bw++;
  }

  // Render screen_r on the right side
  for (int y = 0; y < EPD_HEIGHT; ++y) {
    for (int x = 0; x < EPD_WIDTH; ++x) {
      uint8_t bit = *screen_r & (0x80 >> (x % 8));
      uint32_t *pixel =
          (uint32_t *)combinedSurface->pixels + y * combinedSurface->w + x;
      if (bit == 0)
        *pixel = SDL_MapRGBA(combinedSurface->format, 255, 0, 0, 255);
      if (x % 8 == 7)
        screen_r++;
    }
    if (EPD_WIDTH % 8 != 0)
      screen_r++;
  }
  SDL_UnlockSurface(combinedSurface);
  // Create SDL texture from combined surface
  SDL_Texture *combinedTexture =
      SDL_CreateTextureFromSurface(renderer, combinedSurface);
  // 旋转屏幕
  SDL_Rect dstrect;
  if (screen.rotation == 0) {
    dstrect = {0, 0, EPD_WIDTH, EPD_HEIGHT};
  } else if (screen.rotation == 90) {
    dstrect = {0, screen.height, EPD_WIDTH, EPD_HEIGHT};
  } else if (screen.rotation == 180) {
    dstrect = {screen.width, screen.height, EPD_WIDTH, EPD_HEIGHT};
  } else if (screen.rotation == 270) {
    dstrect = {screen.width, 0, EPD_WIDTH, EPD_HEIGHT};
  } else {
    printf("Unsupported rotation %d\n", screen.rotation);
  }
  // 指定旋转中心
  SDL_Point center = {0, 0};
  // 旋转绘图
  SDL_RenderCopyEx(renderer, combinedTexture, NULL, &dstrect, -screen.rotation,
                   &center, SDL_FLIP_NONE);

  // Destroy the surface and texture
  SDL_FreeSurface(combinedSurface);
  SDL_DestroyTexture(combinedTexture);

  // Present the renderer
  SDL_RenderPresent(renderer);
}

void DISPLAY_Loop(void (*main_entry)(void)) {
  // Main loop
  int quit = 0;
  SDL_Event event;
  // Render your screens
  main_entry();
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }

    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
  }

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
