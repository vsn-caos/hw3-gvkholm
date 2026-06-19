#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <CMD1> <CMD2>\n", argv[0]);
        return 1;
    }
    const char *cmd1 = argv[1];
    const char *cmd2 = argv[2];
    int fds[2];
    if (pipe(fds) < 0) {
        perror("pipe");
        return 1;
    }
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }
    if (pid1 == 0) {
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);
        execlp("sh", "sh", "-c", cmd1, (char *)NULL);
        perror("execlp");
        _exit(127);
    }
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }
    if (pid2 == 0) {
        close(fds[1]);
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]);
        execlp("sh", "sh", "-c", cmd2, (char *)NULL);
        perror("execlp");
        _exit(127);
    }
    close(fds[0]);
    close(fds[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
