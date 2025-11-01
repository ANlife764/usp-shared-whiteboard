// controller.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define ROWS 20
#define COLS 40

static char (*board)[COLS];
static int shmid, semid;
static pid_t viewer_pid;

// ---- Board Printer ----
void print_board(void) {
    printf("\033[H\033[J"); // clear screen
    for (int i = 0; i < ROWS; i++) {
        fwrite(board[i], 1, COLS, stdout);
        putchar('\n');
    }
    fflush(stdout);
}

// ---- Signal Handler for Cleanup ----
void cleanup(int signo) {
    printf("\n[Controller] Cleaning up...\n");

    if (viewer_pid > 0) kill(viewer_pid, SIGTERM);
    shmdt(board);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    exit(0);
}

int main(void) {
    key_t key = ftok(".", 'W');
    if (key == -1) { perror("ftok"); exit(1); }

    // Create Shared Memory
    shmid = shmget(key, ROWS * COLS, 0666 | IPC_CREAT);
    if (shmid == -1) { perror("shmget"); exit(1); }
    board = shmat(shmid, NULL, 0);
    if (board == (void *)-1) { perror("shmat"); exit(1); }

    // Initialize Board
    memset(board, '.', ROWS * COLS);

    // Create Semaphore
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) { perror("semget"); exit(1); }
    semctl(semid, 0, SETVAL, 1);

    // Handle Ctrl+C
    signal(SIGINT, cleanup);

    // Fork Viewer Process
    viewer_pid = fork();
    if (viewer_pid == 0) {
        while (1) {
            print_board();
            usleep(500000);
        }
        _exit(0);
    }

    printf("[Controller] Shared Whiteboard running.\n");
    printf("[Controller] Run ./user <row> <col> <text> in other terminals.\n");
    printf("Press Ctrl+C here to stop and clean up.\n");

    // Parent just waits
    while (1) pause();
}