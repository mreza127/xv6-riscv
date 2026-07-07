#ifndef _PINFO_H_
#define _PINFO_H_

#define MAX_PROC 64   // based on NPROC in kernel/param.h

struct pinfo {
  int pid;
  int state;          // enum numeric value
  char name[16];
};

#endif
