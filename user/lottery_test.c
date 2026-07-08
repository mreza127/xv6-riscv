#include "kernel/types.h"
#include "user/user.h"

struct result {
  int pid;
  int tickets;
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
  int tickets[] = {10, 20, 30};
  int n = 3;
  int fd[2];
  
  pipe(fd);

  printf("Starting Lottery Benchmark ...\n");
  int start_time = uptime();

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[0]);
      settickets(tickets[i]);
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

  printf("\n--- Execution Timeline ---\n");
  printf("PID    TICKETS        TURNAROUND TIME\n");
  
  for (int i = 0; i < n; i++) {
    struct result r;
    read(fd[0], &r, sizeof(r));
    printf("%d    %d        %d ticks\n", r.pid, r.tickets, r.end_time);
  }
  close(fd[0]);

  for (int i = 0; i < n; i++) 
    wait(0);

  printf("\nLottery Benchmark complete.\n");
  exit(0);
}
