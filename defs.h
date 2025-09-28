#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define MEDIT_VERSION "0.0.1"
#define MEDIT_TABSTOP 2

#define HL_HIGHLIGHT_STRINGS (1<<0)
#define HL_HIGHLIGHT_NUMBERS (1<<1)

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#define CTRL_KEY(k) ((k) & 0x1f)

#define ABUF_INIT {NULL, 0}

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

enum editorHighlight {
  HL_NORMAL = 0,
  HL_STRING,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_KEYWORD3,
  HL_NUMBER,
  HL_MATCH
};

struct editorSyntax {
  char *filetype;
  char **filematch;
  char **keywords;
  char *singleline_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  int flags;
};

typedef struct erow {
  int idx;
  int size;
  int rsize;
  char *chars;
  char *render;
  unsigned char *hl;
  int hl_open_comment;
} erow;

struct editorConfig {
  int cx, cy;
  int rx;
  int srows;
  int scols;
  int rowoff;
  int coloff;
  int numrows;
  erow *row;
  int dirty;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct editorSyntax *syntax;
  struct termios o_term;
};

struct editorConfig E;

char *py_hl_exts[] = {".py", NULL};
char *py_hl_keywords[] = {
  "print>", "abs>", "all>", "ascii>", "int>", "bool>", "str>", 
  "dict>", "float>", "min>", "max>", "range>",
  "False|","None|","True|",
  "and","as","assert","async","await","break",
  "class","continue","def","del","elif","else","except","finally",
  "for","from","global","if","import","in","is","lambda","nonlocal",
  "not","or","pass","raise","return","try","while","with","yield", 
  NULL
};

struct editorSyntax HLDB[] = {
  {
    "python",
    py_hl_exts,
    py_hl_keywords,
    "#", "'''", "'''",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
  },
};

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}




