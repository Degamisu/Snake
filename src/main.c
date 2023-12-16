#include <genesis.h>
#include <stdlib.h>
// Define constants for the screen size and grid size
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define GRID_SIZE 16

// Snake structure
typedef struct
{
    int x, y;
} SnakeSegment;

SnakeSegment snake[100]; // Maximum number of segments
int snakeLength = 1;     // Initial length of the snake

// Direction enum for snake movement
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

Direction currentDirection = RIGHT; // Initial direction of the snake

// Food position
int foodX, foodY;

void initializeGame()
{
    // Initialize the first snake segment in the middle of the screen
    snake[0].x = SCREEN_WIDTH / 2;
    snake[0].y = SCREEN_HEIGHT / 2;

    // Place the initial food randomly on the screen
    foodX = random() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    foodY = random() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void updateGame()
{
    // Check for input and update snake position accordingly
    if (JOY_readJoypad(JOY_1) & BUTTON_UP && currentDirection != DOWN)
        currentDirection = UP;
    else if (JOY_readJoypad(JOY_1) & BUTTON_DOWN && currentDirection != UP)
        currentDirection = DOWN;
    else if (JOY_readJoypad(JOY_1) & BUTTON_LEFT && currentDirection != RIGHT)
        currentDirection = LEFT;
    else if (JOY_readJoypad(JOY_1) & BUTTON_RIGHT && currentDirection != LEFT)
        currentDirection = RIGHT;

    // Move the snake by updating each segment's position
    for (int i = snakeLength - 1; i > 0; --i)
    {
        snake[i] = snake[i - 1];
    }

    // Update the head of the snake based on the current direction
    switch (currentDirection)
    {
    case UP:
        snake[0].y -= GRID_SIZE;
        break;
    case DOWN:
        snake[0].y += GRID_SIZE;
        break;
    case LEFT:
        snake[0].x -= GRID_SIZE;
        break;
    case RIGHT:
        snake[0].x += GRID_SIZE;
        break;
    }

    // Check for collisions with walls or itself
    if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH || snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT)
    {
        // Game over - hit the wall
        VDP_drawText("Game Over", 15, 15);
        VDP_waitVSync();
        while (1)
        {
        }
    }

    for (int i = 1; i < snakeLength; ++i)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            // Game over - collided with itself
            VDP_drawText("Game Over", 15, 15);
            VDP_waitVSync();
            while (1)
            {
            }
        }
    }

    // Check for collisions with food
    if (snake[0].x == foodX && snake[0].y == foodY)
    {
        // Increase snake length
        snakeLength++;

        // Place new food randomly on the screen
        foodX = random() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
        foodY = random() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
    }

    // Render the snake and food on the screen
    VDP_clearTileMapRect(VDP_BG_A, 0, 0, 40, 28); // Clear the tilemap using VDP_clearTileMapRect instead
    for (int i = 0; i < snakeLength; ++i)
    {
        VDP_drawText("o", snake[i].x / GRID_SIZE, snake[i].y / GRID_SIZE);
    }

    VDP_drawText("*", foodX / GRID_SIZE, foodY / GRID_SIZE);
}

int main()
{
    // Initialize the Genesis system
    VDP_init();
    JOY_init();

    // Set up the screen and VDP plane
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setPlaneSize(32, 32, FALSE); // Use the correct function name and arguments

    // Initialize the random number generator
    srand(1); // Use srand to seed the random number generator

    // Initialize the game
    initializeGame();

    // Game loop
    while (1)
    {
        // Check for user input and update game state
        updateGame();

        // Wait for the next frame
        VDP_waitVSync();
    }

    return 0;
}
