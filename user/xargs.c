#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    if(argc == 1){
        fprintf(2, "usage: [command ...] | xargs [command ...]\n");
        exit(1);
    }
    
    int pid = fork();
    if(pid < 0){
        fprintf(2,"fork failed\n");
        exit(1);
    }
    if(pid == 0){
        char Buf[1024];
        char* buf = Buf;
        gets(buf, 1024);
        char* nxtArgv[MAXARG];
        char* path = argv[1];
        int len;
        for(len = 1;len < argc;len ++){
            nxtArgv[len - 1] = argv[len];
        }
        while(buf[0] != 0){
            int tmp = strlen(buf);
            buf[tmp-1] = 0;
            nxtArgv[len - 1] = buf;
            buf += tmp;
            len ++;
            if(len >= MAXARG){
                fprintf(2,"command is too long\n");
                exit(1);
            }
            gets(buf, 1024);
        }
        nxtArgv[len - 1] = (char*)0;
        exec(path, nxtArgv);
        nxtArgv[len - 1] = buf;
        nxtArgv[len] = (char*)0;
        
        exit(0);
    }
    wait(0);
    //printf("II\n");
    exit(0);
}