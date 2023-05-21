#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
// Init Window
void vInit();
// exit Window
void vExit();
// render window
void vRWin(const Window *window);
bool isKey(int key);
void vClear();
int vGetKey();
// move cursor from Cursor struct
void vCrsrMov(Cursor *crsr);
void vWaitForKey();
void vFlushInput();
void vPrintChar(char ch);
void vPrintStr(const char *str);
// vPrintStr with format
void vPrintF(const char *format, ...);
// move cursor from x and y
void vMovCrsr(int x, int y);
// get Cursor-X
int vGetCrsrX();
// get Cursor-Y
int vGetCrsrY();
// get WIndow Width
int vGetWinW();
// get Window Height
int vGetWinH();
// Set text attributes
void vSetAttr(int foregroundColor, int backgroundColor);

#ifdef V_IMP
void vClear() { werase(stdscr); }

void vInit() {
  initscr();            // Initialize ncurses
  cbreak();             // Disable line buffering
  noecho();             // Don't echo user input
  keypad(stdscr, true); // Enable function keys
  start_color();        // Enable color support
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

void vPrintStr(const char *str) { addstr(str); }

void vPrintF(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vwprintw(stdscr, format, args);
  va_end(args);
}

void vMovCrsr(int x, int y) { move(y, x); }

void vCrsrMov(Cursor *crsr) { move(crsr->y, crsr->x); }

int vGetCrsrX() {
  int x, y;
  getyx(stdscr, y, x);
  return x;
}

int vGetCrsrY() {
  int x, y;
  getyx(stdscr, y, x);
  return y;
}

int vGetWinW() { return COLS; }

int vGetWinH() { return LINES; }

void vSetAttr(int foregroundColor, int backgroundColor) {
  attron(COLOR_PAIR(foregroundColor) | (backgroundColor << 3));
}

#endif
