#include <stdio.h>
#include <stdlib.h>
#include <ncurses>  // for _kbhit() and _getch()
#include <windows.h> // for Sleep()

#define WIDTH 20
#define HEIGHT 10
#define INITIAL_SNAKE_LENGTH 3

// Directions
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// Snake structure
typedef struct {
    int x;
    int y;
} Point;

// Global variables
Point snake[WIDTH * HEIGHT];  // Max size of the snake
int snakeLength = INITIAL_SNAKE_LENGTH;
int direction = RIGHT;  // Initial direction
int gameOver = 0;
Point food;

// Function declarations
void setup();
void draw();
void input();
void logic();
void generateFood();
int collision();

int main() {
    setup();
    
    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(100);  // control the speed of the game
    }

    printf("Game Over! Final length of the snake: %d\n", snakeLength);
    return 0;
}

void setup() {
    // Set up initial position of snake
    snake[0].x = 5;
    snake[0].y = 5;
    for (int i = 1; i < snakeLength; i++) {
        snake[i].x = snake[i - 1].x - 1;
        snake[i].y = snake[i - 1].y;
    }

    // Generate initial food
    generateFood();
}

void draw() {
    system("cls");  // Clear the console

    // Draw the top border
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    // Draw the game area
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0) {
                printf("#");  // Left border
            } else if (x == WIDTH - 1) {
                printf("#");  // Right border
            } else if (x == food.x && y == food.y) {
                printf("F");  // Food
            } else {
                int snakePart = 0;
                for (int i = 0; i < snakeLength; i++) {
                    if (snake[i].x == x && snake[i].y == y) {
                        printf("O");  // Snake body
                        snakePart = 1;
                        break;
                    }
                }
                if (!snakePart) {
                    printf(" ");  // Empty space
                }
            }
        }
        printf("\n");
    }

    // Draw the bottom border
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");
}

void input() {
    if (_kbhit()) {
        char current = _getch();
        if (current == 'w' && direction != DOWN) {
            direction = UP;
        } else if (current == 's' && direction != UP) {
            direction = DOWN;
        } else if (current == 'a' && direction != RIGHT) {
            direction = LEFT;
        } else if (current == 'd' && direction != LEFT) {
            direction = RIGHT;
        }
    }
}

void logic() {
    Point previousHead = snake[0];
    Point previousTail = snake[snakeLength - 1];
    Point newHead = previousHead;

    // Move the snake's head in the current direction
    switch (direction) {
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
    }

    // Move the body
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Update the head
    snake[0] = newHead;

    // If the snake eats food, grow the snake
    if (snake[0].x == food.x && snake[0].y == food.y) {
        snakeLength++;
        generateFood();
    }

    // Check for collisions
    gameOver = collision();
}

void generateFood() {
    food.x = rand() % (WIDTH - 2) + 1;  // Random x coordinate for food
    food.y = rand() % (HEIGHT - 2) + 1;  // Random y coordinate for food
}

int collision() {
    // Check if the snake hits the wall
    if (snake[0].x <= 0 || snake[0].x >= WIDTH - 1 || snake[0].y <= 0 || snake[0].y >= HEIGHT - 1) {
        return 1;
    }

    // Check if the snake runs into itself
    for (int i = 1; i < snakeLength; i++) {
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
            return 1;
        }
    }

    return 0;
}

