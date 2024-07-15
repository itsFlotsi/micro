/* includes */
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

/* defines */

#define CTRL_KEY(k) ((k) & 0x1f)
/* data */

struct termios termios;

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios) == -1) {
        die("tcsetattr");
    };
}
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &termios) == -1) die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP| IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcesetattr");
}
char editorReadKey() {
    int nread;
    char c;
    while((nread = read(STDIN_FILENO, &c,1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    return c;
}
/* output */
void editorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J",4);
}
/* input */
void editorProcessKeypress() {
    char input = editorReadKey();

    switch (input) {
        case CTRL_KEY('q'):
            exit(EXIT_SUCCESS);
            break;
    }
}

/* init */
int main() {
    enableRawMode();

    while(1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }
    return 0;
}
