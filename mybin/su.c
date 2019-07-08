/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   文件名称：su.c
*   创 建 者：gaojie
*   创建日期：2019年07月07日
*   描    述：
*
================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include <shadow.h>
#include <pwd.h>
#include <sys/types.h>
int main(int argc,char *argv[])
{
    char *user = "root";
    if(argc > 1)
    {
        user = argv[1];
    }
    printf("Password: ");
    fflush(stdout);

    char passwd[128]={0};
    fgets(passwd,128,stdin);
    passwd[strlen(passwd) - 1]=0;
    struct spwd *sp = getspnam(user);
    assert(sp != NULL);

    // sp->sp_pwdp 
    char *p = sp->sp_pwdp;
    char salt[128]={0};
    int i = 0;
    int count=0;
    while(*p != 0)//第三个$符号退出
    {
        salt[i++]=*p;
        if(*p == '$')
        {
            count++;
        }
        if(count==3)
            break;
        p++;
    }
    char *pwd = crypt(passwd,salt);
    assert(pwd != NULL);
    //printf("%s\n%s\n",sp->sp_pwdp,pwd);
    if((strcmp(pwd,sp->sp_pwdp)!=0))
    {
        printf("passwd error\n");
        exit(0);
    }
    pid_t n = fork();
    assert(n != -1);

    if(n==0)
    {
        struct passwd *Userinfo = getpwnam(user);
        setuid(Userinfo->pw_uid);
        setenv("HOME",Userinfo->pw_dir,1);
        execl(Userinfo->pw_shell,Userinfo->pw_shell,(char*)0);
        printf("su exec fail\n");
        exit(0);
    }
    else
    {
        wait(NULL);
    }
    
}
