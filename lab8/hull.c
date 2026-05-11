#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct point {
    double x;
    double y;
    int num;       // исходный номер точки
} POINT;

POINT p[100000];   // массив всех точек
POINT hull[100000]; // массив для точек выпуклой оболочки
int n;             // количество точек

// Функция сравнения для qsort: сначала по x, затем по y
int cmp(const void *a, const void *b) {
    POINT *pa = (POINT*)a;
    POINT *pb = (POINT*)b;
    if (pa->x < pb->x) return -1;
    if (pa->x > pb->x) return 1;
    if (pa->y < pb->y) return -1;
    if (pa->y > pb->y) return 1;
    return 0;
}

// Векторное произведение (b - a) x (c - a)
// >0 – левый поворот, <0 – правый, =0 – коллинеарны
double cross(POINT a, POINT b, POINT c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Построение выпуклой оболочки (алгоритм Эндрю)
int convex_hull(POINT *points, int n, POINT *hull) {
    if (n <= 1) {
        for (int i = 0; i < n; i++) hull[i] = points[i];
        return n;
    }

    // 1. Сортируем точки по x (и по y)
    qsort(points, n, sizeof(POINT), cmp);

    // 2. Строим нижнюю оболочку
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(hull[k-2], hull[k-1], points[i]) <= 0)
            k--;
        hull[k++] = points[i];
    }

    // 3. Строим верхнюю оболочку
    int t = k + 1;
    for (int i = n-2; i >= 0; i--) {
        while (k >= t && cross(hull[k-2], hull[k-1], points[i]) <= 0)
            k--;
        hull[k++] = points[i];
    }

    // Убираем дублирование последней точки (она совпадает с первой)
    return k - 1;
}

int main(int argc, char *argv[]) {
    FILE *In, *Out;
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    In = fopen(argv[1], "r");
    if (!In) {
        printf("Cannot open input file %s\n", argv[1]);
        return 1;
    }

    // Чтение точек
    int num;
    double x, y;
    n = 0;
    while (fscanf(In, "%d%lf%lf", &num, &x, &y) == 3) {
        p[n].num = num;
        p[n].x = x;
        p[n].y = y;
        n++;
    }
    fclose(In);

    if (n == 0) {
        printf("No points read.\n");
        return 1;
    }

    // Построение выпуклой оболочки
    int hull_size = convex_hull(p, n, hull);

    // Запись результата в OUT.dat
    Out = fopen("OUT.dat", "w");
    if (!Out) {
        printf("Cannot create output file OUT.dat\n");
        return 1;
    }

    fprintf(Out, "%d\n", hull_size);
    for (int i = 0; i < hull_size; i++) {
        fprintf(Out, "%d ", hull[i].num);
    }
    fprintf(Out, "\n");
    fclose(Out);

    printf("Convex hull built. %d points written to OUT.dat\n", hull_size);
    return 0;
}