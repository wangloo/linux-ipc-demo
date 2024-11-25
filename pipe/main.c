#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

int main() {
  int pipefd[2];
  pid_t pid;
  char buf[128];

  // 创建匿名管道
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (fork() == 0) {
    // 关闭写端，子进程只读数据
    close(pipefd[PIPE_WRITE]);
    read(pipefd[PIPE_READ], buf, sizeof(buf));
    printf("child recv: %s\n", buf);
    
    // 子进程退出前关闭读端
    close(pipefd[PIPE_WRITE]);
  } else {
    // 关闭读端，父进程只读数据
    close(pipefd[PIPE_READ]);
    // 向管道写入数据
    const char *message = "Hello, child process!";
    write(pipefd[PIPE_WRITE], message, strlen(message) + 1);

    // 关闭写端
    close(pipefd[PIPE_WRITE]);
    // 等待子进程结束
    wait(NULL);
  }
  return 0;
}
