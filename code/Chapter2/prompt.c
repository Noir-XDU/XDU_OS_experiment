#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <pwd.h>

uid_t uid;
int user_path_len;
char host_name[256];//linux主机名最长255个字符
char current_user_path[256];//用于获取当前目录的user_path
char current_path[512];
struct passwd * user_info = NULL;


//获取提示信息:user\@hostname:current_path$，并且在第一次输出时，给出更多的提示信息
void get_Prompt(void){
    uid = getuid();//unistd.h
    user_info = getpwuid(uid); 
    user_path_len = strlen(user_info->pw_dir);
    if(gethostname(host_name,256))//函数成功返回0，失败返回-1，错误号存在外部变量errno中
    {
        printf("\033[1;31m Error:Unable to get the hostname, something must be wrong.\033[m\n");
        exit(-1);
    }
    if(!getcwd(current_path,512))//函数成功current_path的内存空间,失败返回NULL
    {
        printf("\033[1;31m Error:Unable to get the current path, something must be wrong.\033[m\n");
        exit(-1);
    }

    if(uid == 0)//root_uid == 0，root用户终端无颜色
    {
        printf("%s",user_info->pw_name);
        printf("@");
        printf("%s",host_name);
        printf(":");
        if(strlen(current_path) > user_path_len)
        {
            memcpy(current_user_path, current_path, user_path_len);
            current_user_path[user_path_len] = '\0';
            if(!strcmp(current_user_path, user_info->pw_dir))
            {
                printf("~");
                printf("%s",&current_path[user_path_len]);
            }
            else
                printf("%s",current_path);
        }
        else
        {
            printf("%s",current_path);
        }
        printf("# ");
    }
    else
    {
        printf("\033[1;32m");
        printf("%s",user_info->pw_name);
        printf("@");
        printf("%s",host_name);
        printf("\033[m");
        printf(":");
        printf("\033[1;34m");
        if(strlen(current_path) > user_path_len)
        {
            memcpy(current_user_path, current_path, user_path_len);
            current_user_path[user_path_len] = '\0';
            if(!strcmp(current_user_path, user_info->pw_dir))
            {
                printf("~");
                printf("%s",&current_path[user_path_len]);
            }
            else
                printf("%s",current_path);
        }
        else
        {
            printf("%s",current_path);
        }
        printf("\033[m");
        printf("$ ");
    }
    return;
}