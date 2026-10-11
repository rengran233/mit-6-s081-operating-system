#include "kernel/types.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

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
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("ls: path too long\n");
    return;
  }
  // copy path to buf and move pointer
  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  // read every dirent
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    // . and .. directories will cause stack overflow
    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;
    // if inode is null
    if(de.inum == 0)
      continue;

    // attach base name after path
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0; // de.name has no tail '\0'
    // get stat here
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }

    // if dirent is a directory, find recursively
    if(st.type == T_DIR)
      find(buf, target);

    // print target path
    if(strcmp(de.name, target) == 0){
      printf("%s\n", buf);
    }
  }

  close(fd);
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
