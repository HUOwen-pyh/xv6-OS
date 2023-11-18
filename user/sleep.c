#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    if(argc <= 1 || argc > 2){
        fprintf(2,"usage: sleep pattern[sleeping time]\n");
        exit(1);
    }

    uint len = strlen(argv[1]);
    int t = 0;
    for(int i = 0;i<len;i++){
        t *= 10;
        t += (int)(argv[1][i]-'0');
    }
    sleep(t);
    exit(0);
}