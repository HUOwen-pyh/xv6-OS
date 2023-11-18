#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

int find(char* path, char* file){

    const static char* cur = ".";
    const static char* pre = "..";
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(path,0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return -1;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return -1;
    }

    if(st.type != T_DIR){
        fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        return -1;
    }

    int len = strlen(path);
    if(strlen(path) + 1 + DIRSIZ > 1024){
        fprintf(2,"path is too long\n");
        close(fd);
        return -1;
    }
    char* p = path + len;
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0){
            continue;
        }
        if(strcmp(de.name,cur) == 0 || strcmp(de.name,pre) == 0){
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(path, &st) < 0){
            printf("find: cannot stat %s\n", path);
            continue;
        }
        if(st.type == T_DIR){
            find(path,file);
            continue;
        }
        if(st.type != T_FILE){
            continue;
        }
        
        if(strcmp(p, file) == 0){
            printf("%s\n",path);
        }
    }
    close(fd);
    return 0;
}

int main(int argc, char* argv[]){

    if(argc != 3){
        fprintf(2,"usage: find + path + filename\n");
        exit(1);
    }

    if(strlen(argv[1])> 1024){
        fprintf(2,"path is too long\n");
        exit(1);
    }

    char path[1024];
    memmove(path, argv[1], strlen(argv[1]));
    path[strlen(argv[1])] = 0;

    if(find(path,argv[2]) == -1){
        fprintf(2,"find failed\n");
        exit(1);
    }

    exit(0);
}