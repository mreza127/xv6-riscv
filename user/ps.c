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

  printf("PID\tSTATE\t\tNAME\n");
  for (int i = 0; i < MAX_PROC; i++) {
    if (pi[i].pid == 0)
      continue;
    printf("%d\t%s\t\t%s\n", pi[i].pid, states[pi[i].state], pi[i].name);
  }
  exit(0);
}
