#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <SDL3/SDL.h>

#define WIDTH_GRID 256
#define HEIGHT_GRID 256
uint8_t cells[WIDTH_GRID * HEIGHT_GRID];
uint8_t cellsFake[WIDTH_GRID * HEIGHT_GRID];

void setAlive(int x, int y)
{
    if (x < WIDTH_GRID && y < HEIGHT_GRID)
    {
        cellsFake[x + y * WIDTH_GRID] = 0xFF;
    }
}

void setDeath(int x, int y)
{
    if (x < WIDTH_GRID && y < HEIGHT_GRID)
    {
        cellsFake[x + y * WIDTH_GRID] = 0;
    }
}

void interrogacion()
{
    for (int y = 0; y < HEIGHT_GRID; y++)
    {
        for (int x = 0; x < WIDTH_GRID; x++)
        {
            int neighbors = 0;

            if (cells[(x - 1) + y * WIDTH_GRID])
                neighbors++;
            if (cells[(x - 1) + (y + 1) * WIDTH_GRID])
                neighbors++;
            if (cells[x + (y - 1) * WIDTH_GRID])
                neighbors++;
            if (cells[(x + 1) + y * WIDTH_GRID])
                neighbors++;
            if (cells[(x - 1) + (y - 1) * WIDTH_GRID])
                neighbors++;
            if (cells[(x + 1) + (y - 1) * WIDTH_GRID])
                neighbors++;
            if (cells[(x + 1) + (y + 1) * WIDTH_GRID])
                neighbors++;
            if (cells[x + (y + 1) * WIDTH_GRID])
                neighbors++;

            if (cells[x + y * WIDTH_GRID]) // viva
            {
                if (neighbors < 2 || neighbors > 3)
                    setDeath(x, y);
            }
            else // morta
            {
                if (neighbors == 3)
                    setAlive(x, y);
            }
        }
    }
    memcpy(cells, cellsFake, WIDTH_GRID * HEIGHT_GRID);
}

int main()
{
    int width = WIDTH_GRID*4;
    int height = HEIGHT_GRID*4;
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("GameOfLife", width, height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderLogicalPresentation(renderer, WIDTH_GRID, HEIGHT_GRID, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    SDL_Texture *grid = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, WIDTH_GRID, HEIGHT_GRID);
    SDL_SetTextureScaleMode(grid, SDL_SCALEMODE_PIXELART);
    bool running = true;
    bool resume_pause = false;

    for (int i = 0; i < WIDTH_GRID * HEIGHT_GRID; i++)
    {
        cells[i] = 0;
    }

    setAlive(20, 20);
    setAlive(21, 21);
    setAlive(19, 22);
    setAlive(20, 22);
    setAlive(21, 22);

    interrogacion();
    SDL_UpdateTexture(grid, NULL, cells, WIDTH_GRID);

    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            else if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                
                int x = event.button.x / (width / WIDTH_GRID);
                int y = event.button.y / (height / HEIGHT_GRID);
                setAlive(x, y);
                int index = x + y * WIDTH_GRID;
                cells[index] = cellsFake[index];
                
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.scancode == SDL_SCANCODE_S)
                {
                    resume_pause = !resume_pause;
                    
                }
            }
        }

        if (resume_pause == true)
        {
            interrogacion();
        }
        SDL_UpdateTexture(grid, NULL, cells, WIDTH_GRID);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, grid, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000.f / (resume_pause ? 10 : 60));
    }
}