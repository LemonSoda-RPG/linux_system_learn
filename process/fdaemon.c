//
// Created by jiacheng on 4/17/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    daemon(0,0);
    int i=0;
    while(1) {
        fprintf(stdout,"%d",i);
        i++;
        sleep(1);
    }
    exit(0);
}