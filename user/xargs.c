#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("need at least 2 args\n");
    exit(1);
  }

  char *sub_cmd = argv[1];
  char *sub_argv[100];
  int base_cnt = 0;
  char buf[512];
  char *p = buf, *line = buf;

  // inherit args form base argv
  for (int i = 1; i < argc; i++) {
    sub_argv[base_cnt] = argv[i];
    base_cnt++;
  }
  int sub_cnt = base_cnt;

  // read successive args from pipe and exec line by line
  char c;
  while (read(0, &c, 1) > 0) {
    if (c == ' ' || c == '\n') {
      *p = '\0'; // C string spec
      p++;
      if (strlen(line) > 0) {
        sub_argv[sub_cnt] = line;
        sub_cnt++;
      }
      line = p;

      if (c != '\n') continue;

      sub_argv[sub_cnt] = 0; // exec() spec
      if (sub_cnt > base_cnt) { // prevent continuous '\n'
        if (fork() == 0) {
          exec(sub_cmd, sub_argv);
          fprintf(2, "xargs: exec %s failed\n", sub_cmd);                                                                                                                               
          exit(1);     
        }
        wait(0);
      }
      sub_cnt = base_cnt;
      p = line = buf;
    } else {
      *p = c;
      p++;
    }
  }

  exit(0);
}
