/*================================================================
*   Copyright (C) 2019 Sangfor Ltd. All rights reserved.
*   
*   文件名称：ls.c
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

#include <dirent.h>
#include <sys/types.h>

#define IS_A 1
#define IS_L 12
#define IS_I 9
#define IS_S 19

#define IS_PARA(para,flag) ((para) & (1<<(flag-1)))
int Parameter = 0;
void AnalPara(char *argv[],int argc)
{
    int i = 1;
    for(;i<argc;++i)
    {
        if(strncmp(argv[i],"-",1)!=0)
        {
            continue;
        }
        char *p = argv[i] + 1;//使得p指针指向第一个有效字符
        while(*p!=0)
        {
            Parameter |= 1 << (*p - 'a');
            p++;
        }
        /*
        if(strstr(argv[i],"a")!=NULL)
        {
            Parameter |= 1 << IS_A;
        }
        if(strstr(argv[i],"l")!=NULL)
        {
            Parameter |= 1 << IS_L;
        }
        if(strstr(argv[i],"i")!=NULL)
        {
            Parameter |= 1 << IS_I;
        }*/

    }
}
int main(int argc,char *argv[])
{
    AnalPara(argv,argc);
    char CurrentPath[128]={0};
    getcwd(CurrentPath,127);

    DIR *dirp = opendir(CurrentPath);//目录流
    assert(NULL != dirp);

    struct dirent *p = NULL;
    while(p=readdir(dirp))
    {
        if(!IS_PARA(Parameter,IS_A) && (strncmp(p->d_name,".",1)==0||strncmp(p->d_name,"..",2)==0))
            continue;
        if(IS_PARA(Parameter,IS_I))
        {
            printf("%d  ",p->d_ino);
        }
        printf("%s  ",p->d_name);
    }
    printf("\n");
    closedir(dirp);
}
