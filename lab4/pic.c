#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N[100], M[100];
char **pic[100];

void free_pic(int i) {
    if (pic[i] != NULL) {
        for (int k = 0; k < M[i]; k++)
            free(pic[i][k]);
        free(pic[i]);
        pic[i] = NULL;
    }
}

void input(int i, char *filename) {
    FILE *F = fopen(filename, "r");
    if (!F) {
        printf("Failed to open file %s\n", filename);
        return;
    }
    char str[512], tmp[512][512];
    int rows = 0, cols = 0;
    while (fgets(str, 512, F)) {
        int len = strlen(str);
        if (str[len-1] == '\n') len--;
        if (len > cols) cols = len;
        strncpy(tmp[rows], str, len);
        tmp[rows][len] = '\0';
        rows++;
    }
    fclose(F);

    if (pic[i] != NULL) free_pic(i);

    M[i] = rows;
    N[i] = cols;
    pic[i] = (char **)malloc(rows * sizeof(char *));
    for (int r = 0; r < rows; r++) {
        pic[i][r] = (char *)malloc(cols * sizeof(char));
        for (int c = 0; c < cols; c++) {
            if (c < (int)strlen(tmp[r]))
                pic[i][r][c] = tmp[r][c];
            else
                pic[i][r][c] = ' ';
        }
    }
    printf("Picture #%d loaded: %d rows, %d cols\n", i, rows, cols);
}

void output(int i) {
    if (pic[i] == NULL) {
        printf("Picture #%d does not exist\n", i);
        return;
    }
    printf("Picture #%d (%d x %d):\n", i, M[i], N[i]);
    for (int r = 0; r < M[i]; r++) {
        for (int c = 0; c < N[i]; c++)
            putchar(pic[i][r][c]);
        putchar('\n');
    }
}

void clean(int i) {
    if (pic[i] == NULL) return;
    for (int r = 0; r < M[i]; r++)
        for (int c = 0; c < N[i]; c++)
            pic[i][r][c] = ' ';
}

void inverse(int i) {
    if (pic[i] == NULL) return;
    for (int r = 0; r < M[i]; r++)
        for (int c = 0; c < N[i]; c++)
            pic[i][r][c] = (pic[i][r][c] == '*') ? ' ' : '*';
}

void rotate_r(int i) {
    if (pic[i] == NULL) return;
    int oldM = M[i], oldN = N[i];
    int newM = oldN, newN = oldM;
    char **newPic = (char **)malloc(newM * sizeof(char *));
    for (int r = 0; r < newM; r++) {
        newPic[r] = (char *)malloc(newN * sizeof(char));
        for (int c = 0; c < newN; c++)
            newPic[r][c] = pic[i][oldM - 1 - c][r];
    }
    free_pic(i);
    pic[i] = newPic;
    M[i] = newM;
    N[i] = newN;
}

void rotate_l(int i) {
    if (pic[i] == NULL) return;
    int oldM = M[i], oldN = N[i];
    int newM = oldN, newN = oldM;
    char **newPic = (char **)malloc(newM * sizeof(char *));
    for (int r = 0; r < newM; r++) {
        newPic[r] = (char *)malloc(newN * sizeof(char));
        for (int c = 0; c < newN; c++)
            newPic[r][c] = pic[i][c][oldN - 1 - r];
    }
    free_pic(i);
    pic[i] = newPic;
    M[i] = newM;
    N[i] = newN;
}

void frame(int i, char c) {
    if (pic[i] == NULL) return;
    int oldM = M[i], oldN = N[i];
    int newM = oldM + 2, newN = oldN + 2;
    char **newPic = (char **)malloc(newM * sizeof(char *));
    for (int r = 0; r < newM; r++) {
        newPic[r] = (char *)malloc(newN * sizeof(char));
        for (int c2 = 0; c2 < newN; c2++) {
            if (r == 0 || r == newM-1 || c2 == 0 || c2 == newN-1)
                newPic[r][c2] = c;
            else
                newPic[r][c2] = pic[i][r-1][c2-1];
        }
    }
    free_pic(i);
    pic[i] = newPic;
    M[i] = newM;
    N[i] = newN;
}

void dotting(int i) {
    if (pic[i] == NULL) return;
    for (int r = 0; r < M[i]; r++)
        for (int c = 0; c < N[i]; c++)
            if (pic[i][r][c] == ' ')
                pic[i][r][c] = '.';
}

void save(int i, char *filename) {
    if (pic[i] == NULL) {
        printf("Picture #%d does not exist\n", i);
        return;
    }
    FILE *F = fopen(filename, "w");
    if (!F) {
        printf("Failed to create file %s\n", filename);
        return;
    }
    for (int r = 0; r < M[i]; r++) {
        for (int c = 0; c < N[i]; c++)
            fputc(pic[i][r][c], F);
        fputc('\n', F);
    }
    fclose(F);
    printf("Picture #%d saved to %s\n", i, filename);
}

void print_help() {
    printf("\nAvailable commands:\n");
    printf("  load <i> <file>   - load picture #i from file\n");
    printf("  out <i>           - output picture #i\n");
    printf("  clean <i>         - clear picture #i (fill with spaces)\n");
    printf("  inv <i>           - invert picture #i\n");
    printf("  rot_r <i>         - rotate picture #i clockwise\n");
    printf("  rot_l <i>         - rotate picture #i counterclockwise\n");
    printf("  frame <i> <c>     - add a frame of character c around picture #i\n");
    printf("  dot <i>           - replace spaces with dots in picture #i\n");
    printf("  save <i> <file>   - save picture #i to file\n");
    printf("  help              - show this help window\n");
    printf("  quit              - exit\n");
}

int main() {
    for (int i = 0; i < 100; i++) pic[i] = NULL;

    printf("Interactive ASCII picture editor\n");
    print_help();

    char cmd[32], fname[256];
    int idx;
    char ch;
    while (1) {
        printf("\n> ");
        if (scanf("%s", cmd) != 1) break;
        if (strcmp(cmd, "quit") == 0) break;
        else if (strcmp(cmd, "help") == 0) print_help();
        else if (strcmp(cmd, "load") == 0) {
            if (scanf("%d %s", &idx, fname) == 2) {
                if (idx >= 0 && idx < 100) input(idx, fname);
                else printf("Index must be 0-99\n");
            } else printf("Usage: load <index> <filename>\n");
        }
        else if (strcmp(cmd, "out") == 0) {
            if (scanf("%d", &idx) == 1) output(idx);
            else printf("Usage: out <index>\n");
        }
        else if (strcmp(cmd, "clean") == 0) {
            if (scanf("%d", &idx) == 1) clean(idx);
            else printf("Usage: clean <index>\n");
        }
        else if (strcmp(cmd, "inv") == 0) {
            if (scanf("%d", &idx) == 1) inverse(idx);
            else printf("Usage: inv <index>\n");
        }
        else if (strcmp(cmd, "rot_r") == 0) {
            if (scanf("%d", &idx) == 1) rotate_r(idx);
            else printf("Usage: rot_r <index>\n");
        }
        else if (strcmp(cmd, "rot_l") == 0) {
            if (scanf("%d", &idx) == 1) rotate_l(idx);
            else printf("Usage: rot_l <index>\n");
        }
        else if (strcmp(cmd, "frame") == 0) {
            if (scanf("%d %c", &idx, &ch) == 2) frame(idx, ch);
            else printf("Usage: frame <index> <char>\n");
        }
        else if (strcmp(cmd, "dot") == 0) {
            if (scanf("%d", &idx) == 1) dotting(idx);
            else printf("Usage: dot <index>\n");
        }
        else if (strcmp(cmd, "save") == 0) {
            if (scanf("%d %s", &idx, fname) == 2) save(idx, fname);
            else printf("Usage: save <index> <filename>\n");
        }
        else printf("Unknown command. Type 'help'.\n");
        while (getchar() != '\n');
    }

    for (int i = 0; i < 100; i++)
        if (pic[i] != NULL) free_pic(i);

    return 0;
}