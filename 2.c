#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char** argv){
    int fd[argc];

    FILE *pFile;
    int num;
    int sum = 0;
    
    if(argc < 2){
        perror("Missing filenames\n");
        return EXIT_FAILURE;
    }

    // for (int i = 1; i < argc / 2; i++){
    //     if((pFile = fopen(argv[i], "r")) == NULL){
    //         printf("Error oppening file %s\n", argv[i]);
    //         return EXIT_FAILURE;
    //     }
    //     while (fscanf(pFile, "%d", &num) == 1){
    //         sum += num;
    //     }
    // }
    // printf("Sum: %d\n", sum);


    if(pipe(fd) == -1){
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid == 0){
        // child
        close(fd[0]);
        int totalChild = 0;

        for (int i = 1; i <= argc / 2; i++){
            if((pFile = fopen(argv[i], "r")) == NULL){
                printf("Error oppening file %s\n", argv[i]);
                return EXIT_FAILURE;
            }
            while (fscanf(pFile, "%d", &num) == 1){
                totalChild += num;
            }
        }
        printf("Sum child: %d\n", totalChild);
        write(fd[1], &totalChild, sizeof(totalChild));
        close(fd[1]);
    } else {
        // parent
        close(fd[1]);
        int num, totalParent = 0;

        for (int i = argc / 2 + 1; i < argc; i++){
            if((pFile = fopen(argv[i], "r")) == NULL){
                printf("Error oppening file %s\n", argv[i]);
                return EXIT_FAILURE;
            }
            while (fscanf(pFile, "%d", &num) == 1){
                totalParent += num;
            }
        }

        printf("Sum parent: %d\n", totalParent);
        wait(NULL);

        int totalChild;
        read(fd[0], &totalChild, sizeof(totalChild));
        close(fd[0]);

        int total = totalParent + totalChild;
        printf("Total: %d\n", total);
    }

    return EXIT_SUCCESS;
}