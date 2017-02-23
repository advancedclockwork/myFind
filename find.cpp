/*
 * find.cpp
 *
 *  Created on: Aug 15, 2016
 *      Author: owen
 */

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int compare(char *current, char *word){
	if(strstr(current,word) != NULL){
		return 1;
	}
	/*if(strlen(current)>=strlen(word)){
		for(int i=0; i<strlen(current)-strlen(word); i++){
			if(current[i]==word[0]){
				char toCompare[strlen(word)];
				for(int j = 0; j < strlen(word); j++){
					toCompare[j] = current[i+j];
				}
				if(strcmp(toCompare,word) == 0){
					return 1;
				}
			}
		}
	}
	*/
	return 0;
}

void searchDirectory(char *directory, char *word){
	int found = 0;
	DIR  *d;
	struct dirent *dir;
	d = opendir(directory);
	int count = 0;
	char newDirectories[256][256];
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			char current[256];
			strcpy(current,dir->d_name);
			if(dir->d_type && DT_DIR){
				if (current[0] != '.' && current[1] != '.'){
					char newDir[256];//strlen(directory)+1+strlen(current)];
					strcpy(newDir,directory);
					strcat(strcat(newDir,"/"),current);
					strcpy(newDirectories[count],newDir);
					count++;
				}
			}
			if(dir->d_type && DT_REG){
				int check = compare(current, word);
				if(check == 1){
					if(found == 0){
						printf("%s\n", directory);
						found = 1;
					}
					char fullPath[256];//strlen(directory)+1+strlen(current)];
					strcpy(fullPath,directory);
					strcat(strcat(fullPath,"/"),current);
					printf("\t%s (", current);
				    struct stat fileStats;
				    if(stat(fullPath,&fileStats) > -1){
				    	printf("%d, ",fileStats.st_size);
						printf( (fileStats.st_mode & S_IRUSR) ? "r" : "-");
						printf( (fileStats.st_mode & S_IWUSR) ? "w" : "-");
						printf( (fileStats.st_mode & S_IXUSR) ? "x" : "-");
						printf( (fileStats.st_mode & S_IRGRP) ? "r" : "-");
						printf( (fileStats.st_mode & S_IWGRP) ? "w" : "-");
						printf( (fileStats.st_mode & S_IXGRP) ? "x" : "-");
						printf( (fileStats.st_mode & S_IROTH) ? "r" : "-");
						printf( (fileStats.st_mode & S_IWOTH) ? "w" : "-");
						printf( (fileStats.st_mode & S_IXOTH) ? "x" : "-");
					}

				    printf(")\n");
				}
			}
		}
		closedir(d);
		for(int i = 0; i<count; i++){
			searchDirectory(newDirectories[i],word);
		}
	}
}

int main(int argc, char **argv){
	if(argc > 3 || argc < 2){
		printf("please enter a valid argument\n");
	}
	else if(argc == 2){
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		searchDirectory(cwd, argv[1]);
	}
	else{
		searchDirectory(argv[2], argv[1]);
	}
	return 0;
}

