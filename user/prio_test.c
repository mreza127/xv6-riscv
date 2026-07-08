#include "kernel/types.h"
#include "user/user.h"

#define WORK_TICKS 40

struct result {
  int pid;
  int priority;
  int end_time;
};

void
burn_cpu() 
{
  for (volatile long i = 0; i < 50000000; i++) {}
}

int
main(void)
{
  int priorities[] = {10, 50, 50, 90};
  int n = 4;
  int fd[2];
  
  pipe(fd);

  printf("Starting Priority Benchmark ...\n");
  int start_time = uptime();

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[0]);
      setpriority(getpid(), priorities[i]);
      burn_cpu();
      burn_cpu();
      burn_cpu();
      struct result r = { getpid(), priorities[i], uptime()-start_time };
      write(fd[1], &r, sizeof(r));
      close(fd[1]);
      exit(0);
    }
  }
  close(fd[1]);

  printf("\n--- Execution Timeline ---\n");
  printf("PID    PRIORITY    TURNAROUND TIME\n");

  for (int i = 0; i < n; i++) {
    struct result r;
    read(fd[0], &r, sizeof(r));
    printf("%d    %d        %d ticks\n", r.pid, r.priority, r.end_time);
  }
  close(fd[0]);

  for (int i = 0; i < n; i++)
    wait(0);

  printf("\nBenchmark complete.\n");
  exit(0);
}
