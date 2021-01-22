#ifndef INIT_H
#define INIT_H

#define WINDOWPOS_X     300
#define WINDOWPOS_Y     300
#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   480

void initOpenGL();
void initSDL();
void compileShaders();
void createTriangle();
void drawTriangle();
void clearWindow();
void processKeyboard();
int checkQuit();
void quitSDL();

#endif