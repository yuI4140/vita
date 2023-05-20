#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int x, y;          // Window position
  int width, height; // Window size
  const char *title; // Window title
} Window;

void vInit();
void vExit();
void vRWin(const Window *window);
bool isKey(int key);
void vClear();
int vGetKey();
void vWaitForKey();
void vFlushInput();
void vPrintChar(char ch);
void vPrintString(const char *str);
void vPrintFormattedString(const char *format, ...);
void vMoveCursor(int x, int y);
int vGetCursorX();
int vGetCursorY();
int vGetWindowWidth();
int vGetWindowHeight();

#ifdef V_IMP

void vClear() { werase(stdscr); }

void vInit() {
  initscr();            // Initialize ncurses
  cbreak();             // Disable line buffering
  noecho();             // Don't echo user input
  keypad(stdscr, true); // Enable function keys
}

void vExit() {
  endwin(); // Cleanup ncurses
}

void vRWin(const Window *window) {
  mvhline(window->y, window->x, ACS_HLINE, window->width);
  mvhline(window->y + window->height - 1, window->x, ACS_HLINE, window->width);
  mvvline(window->y + 1, window->x, ACS_VLINE, window->height - 2);
  mvvline(window->y + 1, window->x + window->width - 1, ACS_VLINE,
          window->height - 2);
  mvaddch(window->y, window->x, ACS_ULCORNER);
  mvaddch(window->y + window->height - 1, window->x, ACS_LLCORNER);
  mvaddch(window->y, window->x + window->width - 1, ACS_URCORNER);
  mvaddch(window->y + window->height - 1, window->x + window->width - 1,
          ACS_LRCORNER);
}

bool isKey(int key) {
  int ch = getch();
  if (ch == key) {
    return true;
  }
  ungetch(ch);
  return false;
}

int vGetKey() { return getch(); }

void vWaitForKey() { getch(); }

void vFlushInput() { flushinp(); }

void vPrintChar(char ch) { addch(ch); }

void vPrintString(const char *str) { addstr(str); }

void vPrintFormattedString(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vwprintw(stdscr, format, args);
  va_end(args);
}

void vMoveCursor(int x, int y) { move(y, x); }

int vGetCursorX() {
  int x, y;
  getyx(stdscr, y, x);
  return x;
}

int vGetCursorY() {
  int x, y;
  getyx(stdscr, y, x);
  return y;
}

int vGetWindowWidth() { return COLS; }

int vGetWindowHeight() { return LINES; }

#endif
