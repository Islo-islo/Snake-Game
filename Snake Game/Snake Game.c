#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Include for Sleep function

#define WIDTH 30
#define HEIGHT 20
#define SIZE 32

int snakeX[WIDTH * HEIGHT], snakeY[WIDTH * HEIGHT], snakeLength;
int appleX, appleY;
int bluePowerX, bluePowerY, yellowPowerX, yellowPowerY;
int dirX, dirY;
int gameOver;
int bluePowerActive, yellowPowerActive;
time_t bluePowerStart, yellowPowerStart;
GLFWwindow* window; // Declaration of window variable

// Key callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_UP && dirY != 1 && action != GLFW_RELEASE) {
        dirX = 0; dirY = -1;
    }
    else if (key == GLFW_KEY_DOWN && dirY != -1 && action != GLFW_RELEASE) {
        dirX = 0; dirY = 1;
    }
    else if (key == GLFW_KEY_LEFT && dirX != 1 && action != GLFW_RELEASE) {
        dirX = -1; dirY = 0;
    }
    else if (key == GLFW_KEY_RIGHT && dirX != -1 && action != GLFW_RELEASE) {
        dirX = 1; dirY = 0;
    }
}

void spawnApple() {
    appleX = rand() % WIDTH;
    appleY = rand() % HEIGHT;
}

void spawnBluePower() {
    bluePowerX = rand() % WIDTH;
    bluePowerY = rand() % HEIGHT;
}

void spawnYellowPower() {
    yellowPowerX = rand() % WIDTH;
    yellowPowerY = rand() % HEIGHT;
}

void init() {
    srand(time(NULL));
    dirX = 0; dirY = -1;
    gameOver = 0;
    bluePowerActive = 0;
    yellowPowerActive = 0;

    snakeLength = 5;
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = WIDTH / 2;
        snakeY[i] = HEIGHT / 2 + i;
    }

    spawnApple();
    spawnBluePower();
    spawnYellowPower();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2i(appleX * SIZE, appleY * SIZE);
    glVertex2i(appleX * SIZE + SIZE, appleY * SIZE);
    glVertex2i(appleX * SIZE + SIZE, appleY * SIZE + SIZE);
    glVertex2i(appleX * SIZE, appleY * SIZE + SIZE);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2i(bluePowerX * SIZE, bluePowerY * SIZE);
    glVertex2i(bluePowerX * SIZE + SIZE, bluePowerY * SIZE);
    glVertex2i(bluePowerX * SIZE + SIZE, bluePowerY * SIZE + SIZE);
    glVertex2i(bluePowerX * SIZE, bluePowerY * SIZE + SIZE);
    glEnd();

    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);
    glVertex2i(yellowPowerX * SIZE, yellowPowerY * SIZE);
    glVertex2i(yellowPowerX * SIZE + SIZE, yellowPowerY * SIZE);
    glVertex2i(yellowPowerX * SIZE + SIZE, yellowPowerY * SIZE + SIZE);
    glVertex2i(yellowPowerX * SIZE, yellowPowerY * SIZE + SIZE);
    glEnd();

    glColor3f(0, 1, 0);
    for (int i = 0; i < snakeLength; i++) {
        glBegin(GL_QUADS);
        glVertex2i(snakeX[i] * SIZE, snakeY[i] * SIZE);
        glVertex2i(snakeX[i] * SIZE + SIZE, snakeY[i] * SIZE);
        glVertex2i(snakeX[i] * SIZE + SIZE, snakeY[i] * SIZE + SIZE);
        glVertex2i(snakeX[i] * SIZE, snakeY[i] * SIZE + SIZE);
        glEnd();
    }

    glfwSwapBuffers(window);
}

void update() {
    if (!gameOver) {
        int prevX = snakeX[0], prevY = snakeY[0];
        int prev2X, prev2Y;
        snakeX[0] += dirX;
        snakeY[0] += dirY;

        for (int i = 1; i < snakeLength; i++) {
            prev2X = snakeX[i];
            prev2Y = snakeY[i];
            snakeX[i] = prevX;
            snakeY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        if (snakeX[0] < 0 || snakeY[0] < 0 || snakeX[0] >= WIDTH || snakeY[0] >= HEIGHT) {
            gameOver = 1;
        }

        for (int i = 1; i < snakeLength; i++) {
            if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
                gameOver = 1;
            }
        }

        if (snakeX[0] == appleX && snakeY[0] == appleY) {
            if (bluePowerActive) {
                snakeLength += 2;
            }
            else {
                snakeLength++;
            }
            spawnApple();
        }

        if (snakeX[0] == bluePowerX && snakeY[0] == bluePowerY) {
            bluePowerActive = 1;
            bluePowerStart = time(NULL);
            spawnBluePower();
        }

        if (snakeX[0] == yellowPowerX && snakeY[0] == yellowPowerY) {
            yellowPowerActive = 1;
            yellowPowerStart = time(NULL);
            spawnYellowPower();
        }

        if (bluePowerActive && difftime(time(NULL), bluePowerStart) > 10) {
            bluePowerActive = 0;
        }

        if (yellowPowerActive && difftime(time(NULL), yellowPowerStart) > 10) {
            yellowPowerActive = 0;
        }
    }
}

int main(void) {
    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(WIDTH * SIZE, HEIGHT * SIZE, "Snake Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback); // Set key callback function
    glOrtho(0, WIDTH * SIZE, HEIGHT * SIZE, 0, -1, 1);

    init();

    while (!glfwWindowShouldClose(window)) {
        draw();
        update();
        if (yellowPowerActive) {
            Sleep(50); // Pause for 50 milliseconds if yellow power is active
        }
        else {
            Sleep(100); // Pause for 100 milliseconds
        }
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
