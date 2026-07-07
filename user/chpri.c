#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("usage: chpri <pid> <priority>\n");
    exit(1);
  }

  int pid = atoi(argv[1]);
  int priority = atoi(argv[2]);

  if (setpriority(pid, priority) < 0) {
    printf("chpri: failed (pid %d, priority %d)\n", pid, priority);
    exit(1);
  }

  printf("chpri: pid %d priority set to %d\n", pid, priority);
  exit(0);
}
