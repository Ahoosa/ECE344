#include "common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h>

 
     

/* make sure to use syserror() when a system call fails. see common.h */
/*need to open the file, create an empty file,read from the opened file, write to the created file, close the opened files*/
/*opendir then readdir one by one then closedir one by one--> mkdir, use stat to see what youre copying is a file if it is then copy file if its directory recursively copy-->chmod now*/
void
usage()
{
	fprintf(stderr, "Usage: cpr srcdir dstdir\n");
	exit(1);
}

int copyFile(char *newpath,char *argv2){
	
	
	//opening a file
	int fd;//-ve if there is an error
	
	if(( fd=open(newpath, O_RDONLY))<0) syserror(open,newpath);

	int creatf; //creating write/read
	if((creatf=creat(argv2, 00700))<0) syserror(creat,argv2);

	
	//reading a file, need to continuously read until ret is 0. data is placed in buf
	char buf[4096];
	int ret;
	while((ret=read(fd,buf,4096))!=0){
		ssize_t wr= write(creatf, buf, ret);
		if(wr<0) syserror(write,argv2);
		//ret=read(fd,buf,4096);
	}

	

	int cl;
	if((cl=close(fd))<0) syserror(close,newpath);
	
	int cl2;
	if((cl2=close(creatf))<0) syserror(close,argv2);

	

	return 0;
	
}






int openDir(char *argv1,char *argv2){

	

	DIR *opdir;
	if((opdir=opendir(argv1))==NULL) syserror(opendir,argv1);
   
	
	struct dirent *reading; 
	struct stat sb;
	char newDest[500];
 	char newpath[500];
	char nameF[500];
	reading=readdir(opdir);

	int newdir;
	if((newdir=mkdir(argv2, 00700))<0) syserror(mkdir,argv2);

	while((reading=readdir(opdir))!=NULL){

	
	
	if(!(strcmp(reading->d_name,"..")==0)){
	sprintf(newpath,"%s/%s",argv1,reading->d_name);
	sprintf(newDest,"%s/%s",argv2,reading->d_name);
	printf(nameF,"%s",reading->d_name);
	int statVal=stat(newpath, &sb);
	if(statVal<0) syserror(stat,newpath);

	
	
	if(S_ISREG(sb.st_mode)){
	  
	        copyFile(newpath,newDest);	
		int priv=chmod(newDest, sb.st_mode & ~S_IFMT);
	        if(priv<0) syserror(chmod,newDest);
	}
		      
	else if(S_ISDIR(sb.st_mode)) {
  
	
		openDir(newpath,newDest);
		int priv=chmod(newDest, sb.st_mode & ~S_IFMT);
	      if(priv<0) syserror(chmod,newDest);
		
	}
	}
	
	
}

          

	int priv=chmod(argv2, sb.st_mode & ~S_IFMT);
	if(priv<0) syserror(chmod,argv2);

	int cldir;
	if((cldir=closedir(opdir))<0) syserror(closedir,newpath);

	return 0;

	
	
}




int
main(int argc, char *argv[])
{
	if (argc != 3) {
		usage();
	}



	openDir(argv[1],argv[2]);
	
	
	
	return 0;
}
