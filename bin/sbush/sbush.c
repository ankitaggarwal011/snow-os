#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  char *welcome = "Welcome to SBUnix!"
  int bytes = write(1, welcome, sizeof(welcome));
  char tmp[1];
  tmp[0] = '0' + bytes;
  write(1, (char *) tmp, sizeof(tmp));
  /*
  if (fork() == 0) {
    write(1, "Parent\n", 7);
  }
  else {
    write(1, "Child\n", 6);
  }
  */
  while(1);
  return 0;
}
