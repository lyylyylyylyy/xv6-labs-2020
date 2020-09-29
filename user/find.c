#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define r 0
#define w 1

int time[10];

void find(char* path, char *path_t, char* filename, int flag)
{
    int fd;
    struct stat st;
    struct dirent de;

    char cur_path[DIRSIZ + 1];
    strcpy(cur_path, path);
    if (flag) {
        int len = strlen(cur_path);
        cur_path[len] = '/';
        strcpy(cur_path + len + 1, path_t);
    }

    if((fd = open(cur_path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
        case T_DIR:
            while(read(fd, &de, sizeof(de))) {
                if (de.inum == 0) continue;
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
                printf("%s\n", cur_path);
                find(cur_path, de.name, filename, 1);
            }
            break;
        case T_FILE:
            if (strcmp(path_t, filename) == 0) {
                printf("%s\n", cur_path);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc<=1) {
        printf("please input filename.\n");
        exit(0);
    }

    char* path = argv[1];
    char* filename = argv[2];

    find(path, "", filename, 0);
    exit(0);
}