#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
int main()
{
    struct passwd *passres;
    passres = getpwnam("jc");
    printf("%s\n",passres->pw_name);

    printf("%s\n",passres->pw_dir);
    return 0;
    
}