#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char **argv){
    int src_file, dst_file;
    struct stat s;
    char *buff;
    if(argc!=3){
        fprintf(stderr,"usage: ./mycp source_file_path destination_file_path\n");
        return -1;
    }

  	src_file = open(argv[1],O_RDONLY);
    if(src_file == -1){
        fprintf(stderr,"Fail to open %s. Please check! \n",argv[1]);
        return -5;
    }
  	fstat(src_file,&s); //获取文件属性
    buff = (char*) malloc(sizeof(char) * s.st_size);
    if (buff == NULL)
      {
        fprintf(stderr,"Malloc error!");
        return -3;
      }
	  ssize_t r = read(src_file, buff, s.st_size);
  
    if(r == 0 ){
        fprintf(stderr,"Nothing in dst_file!\n");
    }else if(r == -1){
        fprintf(stderr,"A error happened!\n");
        return -4;
    }else{
    		fprintf(stderr,"successfully read!\n");
    }
  
  	dst_file = open(argv[2],O_CREAT|O_WRONLY,0600);
    if(dst_file == -1){
        fprintf(stderr,"Can't find %s.\n",argv[2]);
        return -5;
    }


    ssize_t w = write(dst_file, buff,s.st_size);
    if(w == -1 ){
        fprintf(stderr,"A error happened!\n");
        return -6;
    }
    
    fprintf(stderr,"Done. %d word has been writed\n",s.st_size);
    close(src_file);
    close(dst_file);
    return 0;
}