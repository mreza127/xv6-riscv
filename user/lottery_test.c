#include "kernel/types.h"
#include "user/user.h"

struct result {
  int pid;
  int tickets;
  int end_time;
};

void burn_cpu() {
  for (volatile long i = 0; i < 200000000; i++);
}

int 
main(void) 
{
  int tickets[] = {10, 45, 80, 115, 150};
  int n = 5;
  int fd[2];
  
  pipe(fd);

  printf("Starting Lottery Benchmark ...\n");
  
  settickets(getpid(), 999);	

  int start_time = uptime();

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[0]);
      settickets(getpid(), tickets[i]);
      burn_cpu();
      burn_cpu();
      burn_cpu();
      burn_cpu();
      struct result r = { getpid(), tickets[i], uptime()-start_time };    
      write(fd[1], &r, sizeof(r));
      close(fd[1]);
      exit(0);
    }
  }
  close(fd[1]);

  printf("\nLottery Benchmark Results\n");
  printf("=====================================\n");
  printf("PID  TICKETS  TURNAROUND\n");
  printf("-------------------------------------\n");

  for (int i = 0; i < n; i++) {
    struct result r;
    read(fd[0], &r, sizeof(r));
    
    // PID (2 digits)
    if (r.pid < 10)
      printf(" ");
    printf("%d   ", r.pid);

    // Tickets (3 digits)
    if (r.tickets < 10)
      printf("  ");
    else if (r.tickets < 100)
      printf(" ");
    printf("%d      ", r.tickets);

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
  printf("\nLottery Benchmark complete.\n\n");
  exit(0);
}
