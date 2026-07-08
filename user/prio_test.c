#include "kernel/types.h"
#include "user/user.h"

#define WORK_TICKS 40

struct result {
  int priority;
  long count;
};

int
main(void)
{
  int priorities[] = {10, 20, 50, 50, 70, 90};
  int n = 6;
  int fd[2];
  pipe(fd);

  int start = uptime();

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[0]);
      setpriority(getpid(), priorities[i]);
      long count = 0;
      while (uptime() - start < WORK_TICKS)
        count++;
      struct result r = { priorities[i], count };
      write(fd[1], &r, sizeof(r));
      close(fd[1]);
      exit(0);
    }
  }
  close(fd[1]);

  for (int i = 0; i < n; i++) {
    struct result r;
    read(fd[0], &r, sizeof(r));
    printf("priority=%d: %ld loop iterations\n", r.priority, r.count);
  }
  close(fd[0]);

  for (int i = 0; i < n; i++)
    wait(0);

  exit(0);
}
