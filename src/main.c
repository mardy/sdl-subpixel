/*
 * Copyright (C) 2025 Alberto Mardegan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <SDL.h>
#include <malloc.h>
#include <stdbool.h>

static SDL_Window *window;
static SDL_Renderer *renderer;

void fill_texture(SDL_Texture *texture)
{
    uint8_t *pixels;
    int w, h, pitch;

    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++) {
            uint8_t *dst = pixels + y * pitch + x * 4;
            if ((x + y) % 2 == 0) {
                dst[0] = dst[1] = dst[2] = dst[3] = 255;
            } else {
                dst[0] = dst[1] = dst[2] = 0;
            }
            dst[3] = 255;
        }
    SDL_UnlockTexture(texture);
}

int main(int argc, char **argv)
{
    SDL_Texture *texture;

    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Subpixel test",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                SDL_TEXTUREACCESS_STREAMING,
                                640, 480);
    fill_texture(texture);

    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_QUIT:
                done = true;
                break;
            }
        }

        float t = SDL_GetTicks() / 1000.0;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        float adj_radius = 0.3;
        float adj_x = cosf(t * M_PI) * adj_radius;
        float adj_y = sinf(t * M_PI) * adj_radius;

        /* Draw BW checkered texture */
        SDL_FRect dest = { adj_x, adj_y, 640, 480 };
        SDL_RenderCopyF(renderer, texture, NULL, &dest);

        /* Draw a few lines */
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        for (int i = 0; i < 40; i++) {
            float x, y;
            x = 2 + i * 10 + adj_x;
            y = 1 + adj_y;
            SDL_RenderDrawLineF(renderer, x, y, x + 300, y + 300);
        }
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int i = 0; i < 40; i++) {
            float x, y;
            x = 5 + i * 10 + adj_x;
            y = 1 + adj_y;
            SDL_RenderDrawLineF(renderer, x, y, x - 300, y + 300);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        for (int i = 0; i < 40; i++) {
            float x, y;
            x = 3 + i * 12 + adj_x;
            y = 3 + adj_y;
            SDL_RenderDrawLineF(renderer, x + 1, y, x + 8, y);
            SDL_RenderDrawLineF(renderer, x + 9, y + 1, x + 9, y + 8);
            SDL_RenderDrawLineF(renderer, x + 8, y + 9, x + 1, y + 9);
            SDL_RenderDrawLineF(renderer, x, y + 8, x, y + 1);
        }

        /* And points at the end of the lines */
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int i = 0; i < 40; i++) {
            float x, y;
            x = 302 + i * 10 + adj_x;
            y = 301 + adj_y;
            SDL_RenderDrawPointF(renderer, x, y);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}
