#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
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
void vGetStr(char *str, size_t size);
float deltaTime();
#ifdef V_IMP
void vClear() { werase(stdscr); }

void vInit() {
  initscr();            // Initialize ncurses
  cbreak();             // Disable line buffering
  noecho();             // Don't echo user input
  keypad(stdscr, true); // Enable function keys
  start_color();        // Enable color support
  use_default_colors(); // Use default terminal colors
  curs_set(0);          // Hide cursor

  // Define color pairs
  init_pair(1, VCOLOR_DEFAULT, VCOLOR_BLACK);
  init_pair(2, VCOLOR_BLACK, VCOLOR_GREEN);
  init_pair(3, VCOLOR_BLACK, VCOLOR_CYAN);
  init_pair(4, VCOLOR_BLACK, VCOLOR_YELLOW);
  init_pair(5, VCOLOR_BLACK, VCOLOR_BLUE);
  init_pair(6, VCOLOR_BLACK, VCOLOR_MAGENTA);
  init_pair(7, VCOLOR_BLACK, VCOLOR_RED);
  init_pair(8, VCOLOR_BLACK, VCOLOR_WHITE);
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

  va_list args_copy;
  va_copy(args_copy, args);
  int size = vsnprintf(NULL, 0, format, args_copy);
  va_end(args_copy);

  if (size >= 0) {

    char *buffer = (char *)malloc((size + 1) * sizeof(char));
    if (buffer != NULL) {

      vsnprintf(buffer, size + 1, format, args);
      vPrintStr(buffer);
      free(buffer);
    }
  }

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
void vGetStr(char *str, size_t size) {
  vFlushInput(); // Flush any pending input

  int ch;
  size_t i = 0;

  while (i < size - 1) {
    ch = vGetKey();

    if (ch == '\n' || ch == '\r') {
      break;
    } else if (ch == KEY_BACKSPACE || ch == 127) {
      if (i > 0) {
        i--;
        vPrintChar('\b');
        vPrintChar(' ');
        vPrintChar('\b');
      }
    } else if (ch >= 32 && ch <= 126) {
      str[i] = ch;
      i++;
      vPrintChar(ch);
    }
  }

  str[i] = '\0';
}
float deltaTime() {
  static struct timeval lastTime;
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);

  if (lastTime.tv_sec == 0 && lastTime.tv_usec == 0) {
    lastTime = currentTime;
  }

  float deltaTime = (currentTime.tv_sec - lastTime.tv_sec) +
                    (currentTime.tv_usec - lastTime.tv_usec) / 1000000.0f;
  lastTime = currentTime;

  return deltaTime;
}
#endif
