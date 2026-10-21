#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  char *end = sbrk(PGSIZE*32);
  end += 8; // position of "very"
  for (int i = 1; i < 32; i++) {
    char buf[5]; buf[4] = '\0';
    memmove(buf, end, 4);
    printf("the str is: %s\n", buf);
    if (strcmp(buf, "very") == 0) {
      printf("found secret\n");
      end += 24; // position of secret
      write(2, end, 8);
      break;
    }
    end += PGSIZE;
  }

  exit(1);
}
