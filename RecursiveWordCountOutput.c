#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void countWordsLinesChars(FILE * fp, char * newStr){
    int lines = 0, words = 0, characters = 1;
    char c = fgetc(fp), prev = c;
    while(( c = fgetc(fp))!= EOF){
        if(c == '\n'){
            lines++;
        }
        if(isspace(c) && !(isspace(prev))){
            words++;
        }
        characters++;
        prev = c;
    }
    printf("\t%d\t%d\t%d\t%s\n", lines, words, characters, newStr);
}

void gotDirectory(char * newStr){
    sprintf(newStr, "%s/", newStr); //add a slash to the end for recursion purposes 
    printf("wc: %s: Is a directory\n", newStr);
    wordCount(newStr);

}

void wordCount(const char *name)
{
    DIR *dir = opendir(name);
    struct dirent *entry;
    struct stat mystat;
    if(dir != NULL)
    {
        while(entry = readdir(dir))
	    {
            char newStr[1024];
            sprintf(newStr, "%s%s", name, entry->d_name);
            FILE * fp;
            if(entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
                gotDirectory(newStr);
            }  
            else if(entry->d_type == DT_REG){ //might need to add exception for .swc and .swwm files,
                if((fp = fopen(newStr, "r")) != NULL){ // they dont show up on WC
                    countWordsLinesChars(fp, newStr);
                }
            }
	    }
    closedir(dir);
    }
}

int main(int argc, char* argv[])
{
    int i = 0;
    for(i = 0; i < argc; i++)
    {
   	    wordCount(argv[i]);
    }
    return 0;
}
