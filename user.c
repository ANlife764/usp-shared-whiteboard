// user.c — Interactive Shared Whiteboard User Tool
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define ROWS 20
#define COLS 40

static char (*board)[COLS];
static int semid;

void sem_lock() {
    struct sembuf op = {0, -1, 0};
    semop(semid, &op, 1);
}
void sem_unlock() {
    struct sembuf op = {0, 1, 0};
    semop(semid, &op, 1);
}

void draw_text(int row, int col, char *text) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;
    int len = strlen(text);
    if (col + len > COLS) len = COLS - col;
    strncpy(&board[row][col], text, len);
}

void draw_line(int r1, int c1, int r2, int c2, char ch) {
    if (r1 == r2) { // horizontal
        if (c1 > c2) { int tmp = c1; c1 = c2; c2 = tmp; }
        for (int c = c1; c <= c2 && c < COLS; c++)
            if (r1 >= 0 && r1 < ROWS) board[r1][c] = ch;
    } else if (c1 == c2) { // vertical
        if (r1 > r2) { int tmp = r1; r1 = r2; r2 = tmp; }
        for (int r = r1; r <= r2 && r < ROWS; r++)
            if (c1 >= 0 && c1 < COLS) board[r][c1] = ch;
    } else {
        printf("Only horizontal/vertical lines supported.\n");
    }
}

int main() {
    key_t key = ftok(".", 'W');
    int shmid = shmget(key, ROWS * COLS, 0666);
    semid = semget(key, 1, 0666);
    board = shmat(shmid, NULL, 0);

    char mode[10];
    while (1) {
        printf("\nChoose mode (text/line/exit): ");
        scanf("%s", mode);

        if (strcmp(mode, "text") == 0) {
            int row, col;
            char text[100];
            printf("Enter row: "); scanf("%d", &row);
            printf("Enter col: "); scanf("%d", &col);
            printf("Enter text: "); scanf("%s", text);

            sem_lock();
            draw_text(row, col, text);
            sem_unlock();
        }
        else if (strcmp(mode, "line") == 0) {
            int r1, c1, r2, c2;
            char ch;
            printf("Enter r1: "); scanf("%d", &r1);
            printf("Enter c1: "); scanf("%d", &c1);
            printf("Enter r2: "); scanf("%d", &r2);
            printf("Enter c2: "); scanf("%d", &c2);
            printf("Enter char: "); scanf(" %c", &ch);

            sem_lock();
            draw_line(r1, c1, r2, c2, ch);
            sem_unlock();
        }
        else if (strcmp(mode, "exit") == 0) {
            break;
        }
        else {
            printf("Invalid mode. Try again.\n");
        }
    }

    shmdt(board);
    return 0;
}
