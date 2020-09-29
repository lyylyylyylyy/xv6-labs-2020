#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc<=1) {
        printf("please input parameter: second.\n");
    }
    int second = atoi(argv[0]);
    sleep(second);
    exit(0);
}

