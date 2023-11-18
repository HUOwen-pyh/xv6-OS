#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    if(argc != 1){
        fprintf(2, "usage: primes\n");
        exit(1);
    }
    int p[2];
    int i;
    for(i = 1;i<=35;i++){
        if(i == 1){
            pipe(p);
            int pid = fork();
            if(pid == 0){
                int rp = -1;
                int pri = 0;
                int cur = 0;
                while(cur != 35){
                    if(rp == -1){
                        rp = p[0];
                        close(p[1]);
                        read(rp,&cur,4);
                        pri = cur;
                        printf("prime %d\n", pri);
                        p[0] = -1;
                    }
                    else{
                        read(rp,&cur,4);
                        if(cur % pri == 0 && cur != 35){
                            continue;
                        }
                        if(p[0] == -1){
                            if(cur == 35){
                                close(rp);
                                exit(0);
                            }
                            pipe(p);
                            pid = fork();
                            if(pid == 0){
                                rp = -1;
                                continue;
                            }
                            close(p[0]);
                        }
                        write(p[1],&cur,4);
                    }
                }
                wait(0);
                close(rp);
                close(p[1]);
                exit(0);
            }
            close(p[0]);
            continue;
        }
        write(p[1], &i, 4);
    }
    wait(0);
    close(p[1]);
    exit(0);
}