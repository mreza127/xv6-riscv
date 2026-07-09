#include "kernel/types.h"
#include "user/user.h"

struct result {
  int pid;
  int priority;
  int end_time;
};

void burn_cpu() {
  for (volatile long i = 0; i < 200000000; i++);
}

int
main(void)
{
  int priorities[] = {10, 30, 50, 50, 70, 90};
  int n = 6;
  int fd[2];
  
  pipe(fd);

  printf("Starting Priority Benchmark ...\n");
  
  setpriority(getpid(), 1);

  int start_time = uptime();

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[0]);
      setpriority(getpid(), priorities[i]);
      burn_cpu();
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

  printf("\nPriority Benchmark Results\n");
  printf("=====================================\n");
  printf("PID  PRIORITY  TURNAROUND\n");
  printf("-------------------------------------\n");

  for (int i = 0; i < n; i++) {
    struct result r;
    read(fd[0], &r, sizeof(r));

    // PID (2 digits)
    if (r.pid < 10)
      printf(" ");
    printf("%d   ", r.pid);

    // Priority (3 digits)
    if (r.priority < 10)
      printf("  ");
    else if (r.priority < 100)
      printf(" ");
    printf("%d      ", r.priority);

    // Turnaround time
    if (r.end_time < 10)
      printf("  ");
    else if (r.end_time < 100)
      printf(" ");
    printf("%d ticks\n", r.end_time);
  }
  close(fd[0]);

  for (int i = 0; i < n; i++)
    wait(0);

  printf("=====================================\n");
  printf("\nPriority Benchmark complete.\n\n");
  exit(0);
}
