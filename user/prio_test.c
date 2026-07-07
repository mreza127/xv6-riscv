#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int priorities[] = {10, 50, 90};   // high, medium, low
  int n = 3;

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      setpriority(getpid(), priorities[i]);
      for (int iter = 0; iter < 5; iter++) {
        printf("child pid=%d priority=%d iter=%d\n", getpid(), priorities[i], iter);
        for (volatile int k = 0; k < 30000000; k++) ;   // busy-loop
      }
      exit(0);
    }
  }

  for (int i = 0; i < n; i++)
    wait(0);

  printf("prio_test: all children finished\n");
  exit(0);
}
