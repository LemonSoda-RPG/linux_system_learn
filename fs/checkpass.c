#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <string.h>

int main(int argc,char **argv)
{


    if(argc<2)
    {

    }
    struct spwd *shadowline;

    char* inputpass;
    inputpass = getpass("input password:");
    shadowline = getspnam(argv[1]);

    char* crypted_pass = crypt(inputpass,shadowline->sp_pwdp);
    if(strcmp(shadowline->sp_pwdp,crypted_pass)==0)
    {
        printf("加密正确\n");
    }
    else()
    {
        puts("faile!");
    }

    exit(0);
}