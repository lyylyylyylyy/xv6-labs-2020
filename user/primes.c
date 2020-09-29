#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define r 0
#define w 1

int main(int argc, char *argv[])
{
    /**set up the pipeline*/
    int nums[100];
    int file[2];
    int i;
    int count = 0;
    int time[10];

    /**feeds the numbers 2 through 35 into the pipeline*/
    for (int i = 2; i <= 35; i++) {
        nums[count++] = i;
    }


    /**process that reads from its left neighbor over a pipe and writes to its right neighbor over another pipe*/
    while (count > 0) {
        pipe(file);
        if (fork() == 0) {
            /**reads from its left neighbor over a pipe*/
            int prime;
            int current_prime = 0;
            close(file[w]);
            count = -1;

            while (read(file[r], &prime, sizeof(prime))!=0) {
                if (count == -1) {
                    current_prime = prime;
                    count = 0;
                } else {
                    if (prime % current_prime != 0) nums[count++] = prime;
                }
            }
            printf("prime %d\n", current_prime);
            close(file[r]);
        } else {
            /**writes to its right neighbor over another pipe*/
            close(file[r]);
            for (i = 0; i < count; i++) {
                /**write into pipe*/
                write(file[w], &nums[i], sizeof(nums[0]));
            }
            close(file[w]);
            wait(time);
            break;
        }
    }
    exit(0);
}