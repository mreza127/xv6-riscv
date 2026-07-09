#include "kernel/types.h"
#include "user/user.h"

static unsigned int seed;

static void srand_local(unsigned int s) {
  seed = s;
}

static int rand_local(void) {
  seed = seed * 1103515245 + 12345;
  return (int)((seed >> 16) & 0x7fff);
}

static void burn_cpu(void) {
  for (volatile long i = 0; i < 200000000; i++);
}

static void run_ps(void) {
  int pid = fork();
  if (pid == 0) {
    setpriority(getpid(), 1);
    char *args[] = {"ps", 0};
    exec("ps", args);
    exit(1);
  }
  wait(0);
}

int
main(void)
{
  int n = 6;
  int gap = 30;   // ticks to wait between each ps snapshot

  setpriority(getpid(), 1);
  settickets(getpid(), 999);

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      srand_local(getpid() * 7919 + uptime() + 1);

      int priority = rand_local() % 101;         // 0..100
      int tickets  = (rand_local() % 150) + 1;   // 1..150
      int role     = rand_local() % 3;           // 0=busy, 1=alternate, 2=sleep-then-exit

      setpriority(getpid(), priority);
      settickets(getpid(), tickets);

      if (role == 0) {
        // busy the whole time -> RUNNABLE/RUNNING in every snapshot
        int end = uptime() + 3 * gap;
        while (uptime() < end)
          burn_cpu();
      } else if (role == 1) {
        // work, sleep, work again -> caught SLEEPING mid-test
        burn_cpu();
        pause(gap + 5);
        burn_cpu();
      } else {
        // sleeps immediately, then exits -> SLEEPING early, ZOMBIE by the last snapshot
        pause(gap + 10);
        exit(0);
      }
      exit(0);
    }
  }

  printf("ps_test: %d children running with random priority/tickets/behavior\n", n);

  for (int round = 1; round <= 3; round++) {
    printf("=== ps snapshot %d ===\n", round);
    run_ps();
    pause(gap);
  }

  for (int i = 0; i < n; i++)
    wait(0);

  printf("ps_test: done\n");
  exit(0);
}
