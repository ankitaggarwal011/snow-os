#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  char *welcome = "Welcome to SBUnix!\n";
  write(1, welcome, 19);
  
  /*
  if (fork() == 0) {
    write(1, "Parent\n", 7);
    yield();
    write(1, "Parent is back\n", 15);
  }
  else {
    write(1, "Child\n", 6);
  }
  */
  
  while(1);
  return 0;
}
