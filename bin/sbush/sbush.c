#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  write(1, "hello\n", 6);
  if (fork() == 0) {
    write(1, "Parent\n", 7);
  }
  else {
    write(1, "Child\n", 6);
  }
  while(1);
  return 0;
}
