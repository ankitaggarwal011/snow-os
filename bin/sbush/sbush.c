#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  // puts("sbush> ");
  write(1, "hello\n", 6);
  while(1);
  return 0;
}
