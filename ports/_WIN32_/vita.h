#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define VCOLOR_DEFAULT 0
#define VCOLOR_BLACK 1
#define VCOLOR_RED 2
#define VCOLOR_GREEN 3
#define VCOLOR_YELLOW 4
#define VCOLOR_BLUE 5
#define VCOLOR_MAGENTA 6
#define VCOLOR_CYAN 7
#define VCOLOR_WHITE 8

typedef struct {
  int x, y;          // Window position
  int width, height; // Window size
  const char *title; // Window title
} Window;

typedef struct {
  int x, y;
} Cursor;

void vInit();
void vRWin(const Window *window);
bool isKey(int key);
void vClear();
int vGetKey();
void vWaitForKey();
void vPrintChar(char ch);
void vPrintStr(const char *str);
void vPrintF(const char *format, ...);
void vMovCrsr(int x, int y);
int vGetCrsrX();
int vGetCrsrY();
int vGetWinW();
int vGetWinH();
void vSetAttr(int foregroundColor, int backgroundColor);

#ifdef V_IMP
void vClear() { system("cls"); }

void vInit() {
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  GetConsoleCursorInfo(console, &cursorInfo);
  cursorInfo.bVisible = false;
  SetConsoleCursorInfo(console, &cursorInfo);
}

void vRWin(const Window *window) {
  COORD position = {window->x, window->y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
  for (int i = 0; i < window->width; i++)
    printf("-");
  printf("\n");

  for (int i = 0; i < window->height - 2; i++) {
    position.Y++;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
    printf("|");
    position.X += window->width - 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
    printf("|\n");
    position.X = window->x;
  }

  position.Y++;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
  for (int i = 0; i < window->width; i++)
    printf("-");
}

bool isKey(int key) { return GetAsyncKeyState(key) & 0x8000; }

int vGetKey() { return _getch(); }

void vWaitForKey() { _getch(); }

void vPrintChar(char ch) { printf("%c", ch); }

void vPrintStr(const char *str) { printf("%s", str); }

void vPrintF(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

void vMovCrsr(int x, int y) {
  COORD position = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

int vGetCrsrX() {
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
  return consoleInfo.dwCursorPosition.X;
}

int vGetCrsrY() {
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
  return consoleInfo.dwCursorPosition.Y;
}

int vGetWinW() {
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
  return consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
}

int vGetWinH() {
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
  return consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
}

void vSetAttr(int foregroundColor, int backgroundColor) {
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  int color = foregroundColor + (backgroundColor << 4);
  SetConsoleTextAttribute(console, color);
}
#endif
