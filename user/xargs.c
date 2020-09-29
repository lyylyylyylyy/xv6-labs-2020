#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int time[10];
    if(argc <= 1){
        fprintf(2, "Usage: xargs command\n");
        exit(0);
    }

    // argv array.
    char* store[MAXARG];
    // feed argv
    for (int i = 1; i < argc; i++) store[i - 1] = argv[i];
    // 存储读取的字符
    char argument[1000];
    // 读取状态记录
    int stat = 1;
    while (stat) {
        // cnt: 索引    lst_arg: 最后参数索引  argv_cnt: 参数数量
        int cnt = 0, lst_arg = 0, argv_cnt = argc - 1;
        // one character one time
        char ch = 0;
        while (1) {
            stat = read(0, &ch, 1);
            if (stat == 0) exit(0);
            if (ch == ' ' || ch == '\n') {
                argument[cnt++] = 0;
                store[argv_cnt++] = &argument[lst_arg];
                lst_arg = cnt;
                if (ch == '\n') break;
            } else argument[cnt++] = ch;
        }
        store[argv_cnt] = 0;
        if (fork() == 0) {
            exec(store[0], store);
        } else {
            wait(time);
        }
    }

    exit(0);
}