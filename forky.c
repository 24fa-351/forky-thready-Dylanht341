#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

FILE *file;

void pattern_one_child(int process_num) {
    srand(time(NULL) ^ getpid());
    int sleep_time = (rand() % 8) + 1;

    fprintf(file, "Process %d (PID: %d) beginning\n", process_num, getpid());
    fflush(file);
    fprintf(file, "Process %d (PID: %d) sleeping for %d seconds\n", process_num, getpid(), sleep_time);
    fflush(file);
    sleep(sleep_time);
    fprintf(file, "Process %d (PID: %d) exiting\n", process_num, getpid());
    fflush(file);
    exit(0);
}

void pattern_one_parent(int num_processes) {
    pid_t pid_array[num_processes];

    for (int ix = 0; ix < num_processes; ++ix) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Fork failed");
            return;
        } else if (pid == 0) {
            pattern_one_child(ix);
        } else {
            pid_array[ix] = pid;
            fprintf(file, "Parent: created Process %d (PID: %d)\n", ix, pid);
            fflush(file);
        }
    }

    for (int ix = 0; ix < num_processes; ++ix) {
        waitpid(pid_array[ix], NULL, 0);
    }
}

void pattern_two_child(int current_process, int num_processes);

void pattern_two_parent(int current_process, int num_processes) {
    if (current_process < num_processes) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Fork failed");
            return;
        } else if (pid == 0) {
            pattern_two_child(current_process + 1, num_processes);
        } else {
            if (current_process == 0) {
                fprintf(file, "Parent: created Process %d (PID: %d)\n", current_process, getpid());
                fflush(file);
            } else {
                fprintf(file, "Process %d (PID: %d) creating Process %d (PID: %d)\n", current_process, getpid(), current_process + 1, pid);
                fflush(file);
            }
            waitpid(pid, NULL, 0);
            fprintf(file, "Process %d (PID: %d) exiting\n", current_process, getpid());
            fflush(file);
            exit(0);
        }
    }
}

void pattern_two_child(int current_process, int num_processes) {
    srand(time(NULL) ^ getpid());
    int sleep_time = (rand() % 8) + 1;

    fprintf(file, "Process %d (PID: %d) beginning\n", current_process, getpid());
    fflush(file);
    fprintf(file, "Process %d (PID: %d) sleeping for %d seconds\n", current_process, getpid(), sleep_time);
    fflush(file);
    sleep(sleep_time);
    if (current_process < num_processes) {
        pattern_two_parent(current_process, num_processes);
    } else {
        fprintf(file, "Process %d (PID: %d) exiting\n", current_process, getpid());
        fflush(file);
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    int pattern = atoi(argv[1]);
    int number_of_processes =  atoi(argv[2]);

    if (number_of_processes < 1 || number_of_processes > 256) {
        printf("Invalid number of processes\n");
    }
    file = fopen("results.txt", "a");
    if (pattern == 1) {
        fprintf(file, "***Pattern 1 starting\n");
        fflush(file);
        pattern_one_parent(number_of_processes);
    } else if (pattern == 2) {
        fprintf(file, "***Pattern 2 starting\n");
        fflush(file);
        pattern_two_parent(1, number_of_processes);
    }
    fclose(file);
    return 0;
}