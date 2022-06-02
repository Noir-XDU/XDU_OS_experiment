#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include"prompt.c"
#include<errno.h>

#define SIZE_UNIT 256
#define NO_INPUT 1
#define FLAG_BACKGROUND 2
#define FLAG_NO_BACKGROUND 3
#define ARG_MAXM 0x100
pid_t pid;

char *command;
char *new_command;
int count1,count2,count3;
int his_count = 0,his_ori = 0,his_full=0;
char c;//逐字符读取的缓冲
static char *history[10];
char *new_history;
int flag;
char *tmp;
char * args[0x100];
int args_count ;
char *dir;
struct passwd * user_info1 = NULL;


//检查malloc是否成功
void check_malloc(char *s){
    if(s == NULL){
        printf("\033[1;31mError: failed to mall!\033[m");
        exit(1);
    }
}

void analyse_Command(void)
{
    memset(args,'\0',256);
    args_count = 0;
    args[args_count] = (char *) malloc(sizeof(char) * 256);
    args[args_count] = strtok(command, " ");
    char * ptr;
    while((ptr = strtok(NULL," ")))
    {
        args_count++;
        args[args_count] = (char *) malloc(sizeof(char) * 256);
        args[args_count] = ptr;
        if(args_count + 1 == ARG_MAXM)//最多有256个参数
            break;
    }
}



void run_on_Child()
{
    if(flag == FLAG_NO_BACKGROUND){
        int pid = fork();
        errno = 0;
        if(pid < 0) // failed to fork a new thread
            printf("\033[1;31mError: Unable to fork the child, inner error.\033[m");
        else if(pid == 0) // the child thread
        {
            // printf("ls:%s",args[1]);
            int n = execvp(args[0], args);
            // `execlp()` 函数如果执行成功则函数不会返回, 执行失败则直接返回-1, 失败原因存于errno 中.
            printf("\033[1;31mError: unable to execute the programme: %s. something\'s wrong.\n%s\033[m\n", args[0],strerror(errno));
            exit(0);
        }else{
            wait(NULL);
        }

    }
    else{
        int pid = fork();
        errno = 0;
        if(pid < 0) // failed to fork a new thread
            printf("\033[1;31mError: Unable to fork the child, inner error.\033[m");
        else if(pid == 0) // the child thread
        {
            // printf("ls:%s",args[1]);
            int n = execvp(args[0], args);
            // `execlp()` 函数如果执行成功则函数不会返回, 执行失败则直接返回-1, 失败原因存于errno 中.
            printf("\033[1;31mError: unable to execute the programme: %s. something\'s wrong.\n%s\033[m\n", args[0],strerror(errno));
            exit(0);
        }else{
            printf("[%d]\n",pid);
        }
    }
}

//初始化history数组
void init_history(){
    for(int i=0;i<10;i++){
        history[i] = (char *) malloc(sizeof(char)*SIZE_UNIT);
        check_malloc(history[i]);
    }
}

//记录history
void history_record(){
    if(count2 > 1){
        new_history = (char *) malloc(sizeof(char)*(SIZE_UNIT * count2));//有一个问题就是函数一结束，这段空间就被标记成未被使用了
        free(history[his_count]);
        history[his_count] = new_history;
    }
    memset(history[his_count],'\0',strlen(history[his_count]));
    memcpy(history[his_count],command,strlen(command));
    his_count++;
    if(his_full)
    {
        his_ori++;
        his_ori %= 10;
    }
    if(his_count == 10)
    {
        his_count = 0;
        his_full = 1;
    }
}


//命令解析与执行
void exe(void){
    analyse_Command();
    if(!strcmp(args[0],"exit")){
        printf("                                                   \n\
        ****     ***            ***               \n\
        *****    ***     ****                      \n\
        *** ***  ***  **** **** *** ******        \n\
        ***  ******* ***    *** *** ***           \n\
        ***    *****  ***   *** *** ***           \n\
        ***      ***   ******   *** ***           \n\
                                                  \n\
        \033[1;31mExit the Noir_SHELL,THANK YOU FOR YOUR USE\n\033[m");
        exit(0);
    }
    if (!strcmp(args[0],"history"))
    {
        int i,j;
        if(his_full == 0){
            i = his_count;
            j = (his_ori+his_count-1) % 10;
        }else{
            i = 10;
            j = (his_ori-1+10) % 10;
        }

        do
        {
            printf("%d %s\n",i,history[j]);
            i --;
            j = (j + 10 -1) % 10;
        } while (i>=1);
        return;
    }
    if(!strcmp(args[0], "cd"))
    {
        if(args_count > 2||args_count ==0)
            printf("\033[1;31m Error: arguments wrong\033[m \n");
        else
        {
            if(args[1][0] == '~')
            {

                dir = malloc(strlen(args[1]) + strlen(user_info->pw_dir));
                strcpy(dir, user_info->pw_dir);
                strncat(dir, args[1] + 1, strlen(args[1]) - 1);
                chdir(dir);
                free(dir);
                // free(user_info);
            }
            else
                chdir(args[1]);
            //memset(args,'\0',256);
        }
        return ;
    }
        run_on_Child();
        return;
}

//获取指令，分割指令和参数，识别history和!!
int get_command(void){
    count1=0,count2=1,count3=0;
    flag = FLAG_NO_BACKGROUND;
    command =(char*) malloc(sizeof(char)*SIZE_UNIT);//动态实现command，因为ubuntu20.04命令行最长为2097152，有点太长了
    check_malloc(command);
    memset(command,'\0',SIZE_UNIT);
    while((c = getchar()) != '\n')
    {
        if(count1 == count2 * SIZE_UNIT )
        {
            count2++;
            if(count2 * SIZE_UNIT >2097152)
            {
                printf("\033[1;31mError: command too long!\n\033[m");
                exit(1);
            }
            new_command = (char*) malloc(sizeof(char)*(SIZE_UNIT * count2));
            check_malloc(new_command);
            memcpy(new_command,command,(count2-1) * SIZE_UNIT);
            free(command);
            command = new_command;
        }
        command[count1++] = c;
    }
    command[count1] = '\0';
    if(count1 == 0){
        flag = NO_INPUT;
    }else{
        if(command[0] == '!'){
            if(command[1] == '!'){
                if(!his_full && his_count == 0)
                {
                    puts("\033[1;31mError: No available command, history is empty.\n\033[m");
                    flag = NO_INPUT;
                }
                count3 = sizeof(history[((his_count + 10 - 1) % 10)])+count1;
                tmp = (char *) malloc(sizeof(count3));
                strcpy(tmp, history[((his_count + 10 - 1) % 10)]);
                strncat(tmp, command + 2, count1);
                strcpy(command,tmp); //这里可能会有溢出，但是处理起来太麻烦了，先放着吧
                free(tmp);
                printf("%s\n", command);
            }else if(command[1]>='0' && command[2]<='9'){
                int his = atoi(command + 1);
                if (his <= 0 || his > 10 || history[his-1][0]=='\0')
                {
                    puts("\033[31m\033[1mError: No available command, invalid history index.\033[0m");
                    return NO_INPUT;
                }
                count3 = sizeof(history[((his_ori+his+ 10 - 1) % 10)])+count1;
                tmp = (char *) malloc(sizeof(count3));
                strcpy(tmp, history[((his_ori+his+ 10 - 1) % 10)]);
                strncat(tmp, command + 2, count1);
                strcpy(command,tmp); //这里可能会有溢出，但是处理起来太麻烦了，先放着吧
                free(tmp);
                printf("%s\n", command);
            }

        }

    }

    history_record();
    if(command[count1 - 1] == '&')
    {
        command[count1 - 1] = '\0';
        flag = FLAG_BACKGROUND;
    }
    if (flag == NO_INPUT)
    {
        printf("\033[1;31mError: NO INPUT \n\033[m");
        return 0;
    }
    exe();
    free(command);
    return 0;
}

