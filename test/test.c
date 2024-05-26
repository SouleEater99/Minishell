#include "../include/minishell.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int main() {

char *term_name = ttyname(STDIN_FILENO);
if (term_name) {
    printf("Terminal name: %s\n", term_name);
}

}
