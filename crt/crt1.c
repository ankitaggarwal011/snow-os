#include <stdlib.h>
#include <stdlib.h>

void _start() {
  int *argc, argcnt;
  long argcOffset = 0x30;
  __asm__ volatile (
     "movq %%rsp, %%rax;\r\t"
     "add %1, %%rax;\r\t"
     "movq %%rax, %0;\r\t"
     : "=r" (argc)
     : "r" (argcOffset)
     : "rax"
  );
  argcnt = *argc;
  long offset = 0x29E;
  char argv[argcnt][1024];
  char *ptr = ((char*)argc) + offset;
  int i = 0, j = 0;
  while (i< argcnt) {
    if (ptr == NULL) {
      exit(-5);
    }
    if (*ptr == '\0' || j == 1023) {
      argv[i][j] = '\0';
      i++;
      j=0;
      ptr++;
      continue;
    }
    argv[i][j] = *ptr;
    j++;
    ptr++;
  }
  char* envStart = ptr;
  int envCount = 0;
  while (1) {
    if ((*ptr)== '\0') {
        if (*(ptr+1) == '\0') {
          break;
        }
      envCount++;
    }
    ptr++;
  }
  char envp[envCount+1][1024];
  i = 0, j = 0;
  ptr = envStart;
  while (i< envCount && ptr != NULL) {
    if (*ptr == '\0' || j == 1023) {
      envp[i][j] = '\0';
      i++;
      j=0;
      ptr++;
      continue;
    }
    envp[i][j] = *ptr;
    j++;
    ptr++;
  }
  char** envpArray = (char**) envp;
  envpArray[envCount] = NULL;
  envp[envCount][0] = '\0';
  exit(main(argcnt, (char**)argv, (char**)envp));
}
