#include "kernel/types.h"
#include "kernel/pinfo.h"
#include "user/user.h"

// based on proc.h
char *states[] = {
  "UNUSED", "USED", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"
};

int
main(void)
{
  struct pinfo pi[MAX_PROC];
  int n = getpinfo(pi);

  if (n < 0) {
    printf("ps: getpinfo failed\n");
    exit(1);
  }

  if (n == 0) {
    printf("\nps: there is no process\n");
    exit(0);
  }
	
  printf("PID  STATE      PRI  TICKETS  NAME\n");
  printf("----------------------------------------\n");

  for (int i = 0; i < MAX_PROC; i++) {
    if (pi[i].pid == 0)
      continue;

    // PID (2 columns)
    if (pi[i].pid < 10)
      printf(" ");
    printf("%d   ", pi[i].pid);

    // STATE (10 columns)
    printf("%s", states[pi[i].state]);
    int j = 0;
    while (states[pi[i].state][j])
      j++;
    while (j++ < 10)
      printf(" ");

    // PRIORITY (3 columns)
    if (pi[i].priority < 10)
      printf("  ");
    else if (pi[i].priority < 100)
      printf(" ");
    printf("%d   ", pi[i].priority);

    // TICKETS (3 columns)
    if (pi[i].tickets < 10)
      printf("  ");
    else if (pi[i].tickets < 100)
      printf(" ");
    printf("%d      ", pi[i].tickets);

    // NAME
    printf("%s\n", pi[i].name);
  }

  printf("----------------------------------------\n");

  exit(0);
}
