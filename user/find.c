#include "kernel/types.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

char*
get_base(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return base name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  *(buf + strlen(p)) = 0;

  return buf;
}

void
find(char *path, char *target)
{
  char buf[512], *p;  // buf and pointer writing in buf
  int fd;             // fd of file in path
  struct dirent de;   // directory entry
  struct stat st;     // file metadata

  if((fd = open(path, O_RDONLY)) < 0){
      fprintf(2, "find: cannot open %s\n", path);
      return;
  }

  if(fstat(fd, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", path);
      close(fd);
      return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:{
    // print the path
    char *base = get_base(path);
    if (strcmp(base, target) == 0) {
      printf("%s\n", path);
    }
    break;
    }

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    // write after path
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    // read every dirent
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      // if . and .. directories will cause stack overflow
      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0; // de.name has no tail '\0'
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      find(buf, target);
    }
    break;
  }

  close(fd);
}

void
_test(char *path)
{
  char *p;
  p = get_base(path);
  printf("base name: %s\n", p);
}

int
main(int argc, char *argv[])
{
    char *path = argv[1];
    char *target = argv[2];

    if (argc < 3) {
        printf("error: need three args\n");
        exit(1);
    }

    find(path, target);

    exit(0);
}
