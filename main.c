#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


long long decodeValue(const char *value, int base) {
    return strtoll(value, NULL, base);
}


typedef struct {
    int x;
    long long y;
} Point;


double lagrangeInterpolation(Point *points, int k) {
    double constantTerm = 0;

    for (int i = 0; i < k; i++) {
        int xi = points[i].x;
        long long yi = points[i].y;
        double term = yi;

        for (int j = 0; j < k; j++) {
            if (i != j) {
                int xj = points[j].x;
                term *= (0 - xj) / (double)(xi - xj);
            }
        }

        constantTerm += term;
    }

    return round(constantTerm); 
}


int readJsonFile(const char *filename, Point *points, int *k) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 0;
    }

    char line[1024];
    int n, pointCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "\"n\": %d,", &n) == 1) {
            continue;
        }
        if (sscanf(line, "\"k\": %d,", k) == 1) {
            continue;
        }

        int index, base;
        char value[256];
        if (sscanf(line, "\"%d\": { \"base\": \"%d\", \"value\": \"%[^\"]\" },", &index, &base, value) == 3) {
            points[pointCount].x = index;
            points[pointCount].y = decodeValue(value, base);
            pointCount++;
        }
    }

    fclose(file);
    return pointCount;
}

int main() {
    const char *filenames[] = {"input1.json", "input2.json"};
    Point points[256];
    int k;

    for (int i = 0; i < 2; i++) {
        int pointCount = readJsonFile(filenames[i], points, &k);

        if (pointCount < k) {
            fprintf(stderr, "Error: Not enough points to solve the polynomial\n");
            continue;
        }

        double constantTerm = lagrangeInterpolation(points, k);
        printf("The secret (constant term c) for Test Case %d is: %lf\n", i + 1, constantTerm);
    }

    return 0;
}
