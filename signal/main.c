#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void sig_handler(int signum) {
  if (signum == SIGUSR1) {
    printf("Child process received SIGUSR signal from parent!\n");
    exit(0);
  }
}

int main() {
  pid_t pid;

  if ((pid = fork()) == 0) {
    printf("Child process start...\n");
    signal(SIGUSR1, sig_handler);

    // 子进程暂停，等待信号
    while (1)
      pause();
  } else {
    printf("Parent process start...\n");
    // 睡眠1s，确保子进程已准备好接受信号
    sleep(1);
    // 向子进程发送信号
    kill(pid, SIGUSR1);
    printf("Parent sent SIGUSR1!\n");
    printf("Parent process finished.\n");
  }
  return 0;
}
