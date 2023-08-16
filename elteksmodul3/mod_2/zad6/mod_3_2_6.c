#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

DIR *mydir;

int main(){
	int f =0;
	while(1){
	if(f==0){
		f=1;
		if((mydir=opendir("."))==NULL){
		perror("ERROR1");
		exit(EXIT_FAILURE);
		}
	}
	rewinddir(mydir);
	struct stat buf;
	struct dirent *ger;  
	while((ger=readdir(mydir)) != NULL){
		
		stat(ger->d_name,&buf);
		if(S_ISREG(buf.st_mode)){printf("File ");}
		if(S_ISDIR(buf.st_mode)){printf("Dir ");}
		if(S_ISLNK(buf.st_mode)){printf("Link ");}
		printf("%s\n", ger->d_name);
	}
	
	printf("\n\nComand: \n");
	printf("1 Open dir \n");
	printf("2 Go to parent dir \n");
	printf("\n\nInsert num comand: ");
	char s[100];
	char r[10];
	scanf("%s",r);
	if(strcmp(r,"1")==0){
		printf("\nInsert name of dir: ");
		scanf("%s",s);
		int u=0;
		struct dirent *ger; 
		rewinddir(mydir);
		while((ger=readdir(mydir)) != NULL){
			if(strcmp(s,ger->d_name)==0){
				u=1;
				break;
			}
		}
		if(!u){
			printf("\nInsert correct name of dir in this catalog\n\n");
			system("read -p 'Press Enter to continue...' var");
		}else{
			stat(ger->d_name,&buf);
			if(!S_ISDIR(buf.st_mode)){
				printf("\nInsert correct name of dir in this catalog\n");
				system("read -p 'Press Enter to continue...' var");
			}else{

				if((mydir=opendir(ger->d_name))==NULL){
					exit(EXIT_FAILURE);
				}
				chdir(ger->d_name);
				}	
		}
	}else if(strcmp(r,"2")==0){
		closedir(mydir);
		if((mydir=opendir(".."))==NULL){
			perror("ERROR1");
			exit(EXIT_FAILURE);
		}
		chdir("..");
	}
}
}
