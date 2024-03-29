void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.o_term) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.o_term) == -1)  die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = E.o_term;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr"); 
}




