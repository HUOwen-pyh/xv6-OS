#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    if(argc != 1){
        fprintf(2,"usage: pingpong\n");
        exit(1);
    }

    int p[2];
    if(pipe(p) == -1){
        fprintf(2,"pipe in pingpong");
    }
    char buf[4];
    int pid = fork();
    if(pid < 0){
        fprintf(2,"fork in pingpong");
    }
    if(pid == 0){
        pid = getpid();
        char* sd = "pong";
        read(p[0],buf,4);
        printf("%d: received %s\n", pid, buf);
        write(p[1],sd,4);
        close(p[0]);
        close(p[1]);
    }
    else{
        pid = getpid();
        char* sd = "ping";
        write(p[1],sd,4);
        read(p[0],buf,4);
        printf("%d: received %s\n", pid, buf);
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}