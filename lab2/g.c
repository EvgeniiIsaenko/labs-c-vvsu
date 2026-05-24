// Найти все области, статистику глифов:
// количество черных пикселей, связность, диаметр наибольшей области
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define DEBUG 3 
// #undef DEBUG
#define ISBIT(n,x) (((01<<(n))&(x))?1:0)

void outbyte( char byte) {
    int i;
    for ( i=7; i>-1; i-- )
        putchar(ISBIT((i),byte)?'*':' ');
    fflush(stdout);
}

void outbytes( int n, char *byte) {
    int i;
    for (i=0; i<n; i++)
        outbyte(byte[i]);
    putchar('\n');
}

typedef struct img {
    int w; int h;
    int dx;
    int count;
    int id;
    int bytes;
    double density;
    int diam;
    int perim;
    int conn;
    unsigned char *data;
} IMG;

int popcnt8 ( unsigned char i) {
    int count = 0;
    while(i) {
        ++count;
        i = (i-1) & i;
    }
    return count;
}

IMG *init_img(int id, int w, int h) {
    IMG *t = (IMG *)malloc(sizeof(IMG)); 
    t->w = w; t->h = h; t->dx = 0;
    t->count = 0;
    t->id = id;
    t->bytes = (((w+7)/8) * h);
    t->data = (unsigned char *)calloc(t->bytes, 1);
    return t;
}

IMG *load_img(int id, char *s) {
    FILE *F;
    IMG *I = (IMG *)malloc(sizeof(IMG));
    F = fopen(s, "rb");
    fread(&(I->w), sizeof(int), 1, F);
    fread(&(I->h), sizeof(int), 1, F);
    fread(&(I->dx), sizeof(int), 1, F);
    fread(&(I->count), sizeof(int), 1, F);
    fread(&(I->id), sizeof(int), 1, F);
    fread(&(I->bytes), sizeof(int), 1, F);
    I->data = (unsigned char *)calloc(I->bytes, 1);
    fread(I->data, 1, I->bytes, F);
    fclose(F);
    return I;
}

int xcomp (IMG **a, IMG **b) {
    return -(*a)->w + (*b)->w;
}

int ycomp (IMG **a, IMG **b) {
    return -(*a)->h + (*b)->h;
}

IMG *G[50000];
int N;

// Получение пикселя (x - столбец, y - строка)
int get_pixel(IMG *img, int x, int y) {
    if (x < 0 || x >= img->w || y < 0 || y >= img->h) return 0;
    int row_bytes = (img->w + 7) / 8;
    int byte_idx = y * row_bytes + x / 8;
    int bit = 7 - (x % 8);
    return (img->data[byte_idx] >> bit) & 1;
}

typedef struct { int x, y; } Point;

void compute_stats(IMG *img) {
    int w = img->w, h = img->h;
    int total_pixels = 0;          // общее количество чёрных пикселей
    int conn = 0;                  // количество компонент связности
    int max_area = 0;              // размер наибольшей области
    double max_diameter = 0.0;     // диаметр наибольшей области

    // Массив посещённых пикселей
    char *visited = (char *)calloc(w * h, 1);
    if (!visited) return;

    // Смещения для 4-связности
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    Point *queue = (Point *)malloc(w * h * sizeof(Point));
    if (!queue) {
        free(visited);
        return;
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (get_pixel(img, x, y) && !visited[y * w + x]) {
                conn++;
                int area = 0;
                int front = 0, rear = 0;
                Point *comp_points = (Point *)malloc(w * h * sizeof(Point));
                if (!comp_points) {
                    free(queue);
                    free(visited);
                    return;
                }

                // BFS
                queue[rear++] = (Point){x, y};
                visited[y * w + x] = 1;
                while (front < rear) {
                    Point p = queue[front++];
                    comp_points[area++] = p;
                    total_pixels++;
                    for (int d = 0; d < 4; d++) {
                        int nx = p.x + dx[d];
                        int ny = p.y + dy[d];
                        if (nx >= 0 && nx < w && ny >= 0 && ny < h &&
                            get_pixel(img, nx, ny) && !visited[ny * w + nx]) {
                            visited[ny * w + nx] = 1;
                            queue[rear++] = (Point){nx, ny};
                        }
                    }
                }

                // максимальное расстояние между пикселями
                double diam = 0.0;
                if (area > 1) {
                    double max_sq = 0.0;
                    for (int i = 0; i < area; i++) {
                        for (int j = i + 1; j < area; j++) {
                            int dxc = comp_points[i].x - comp_points[j].x;
                            int dyc = comp_points[i].y - comp_points[j].y;
                            double dist_sq = dxc * dxc + dyc * dyc;
                            if (dist_sq > max_sq) max_sq = dist_sq;
                        }
                    }
                    diam = sqrt(max_sq);
                }
                // Обновление глобальных максимумов
                if (area > max_area) max_area = area;
                if (diam > max_diameter) max_diameter = diam;
                free(comp_points);
            }
        }
    }

    free(queue);
    free(visited);

    // Сохранение результатов в структуру (и пересчёт count)
    img->count = total_pixels;
    img->conn = conn;
    img->diam = (int)(max_diameter + 0.5);   // округление до целого
    img->perim = max_area;                   // используем perim для хранения max_area
    // Вывод на экран
    printf("Glyph id %d: pixels=%d, components=%d, max_area=%d, max_diameter=%d\n",
           img->id, img->count, img->conn, img->perim, img->diam);
}

int main(int argc, char *argv[]) {
    N = argc - 1;
    // Загружаем все глифы и вычисляем статистику
    for (int i = 1; i < argc; i++) {
        G[i] = load_img(i, argv[i]);
        printf("Loaded %s\n", argv[i]);
        // Вывод битовой карты (отладочный, при необходимости)
#ifdef DEBUG
        for (int y = 0; y < G[i]->h; y++) {
            int row_bytes = (G[i]->w + 7) / 8;
            outbytes(row_bytes, &G[i]->data[y * row_bytes]);
        }
#endif
        compute_stats(G[i]);
    }
    return 0;
}