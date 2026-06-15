#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        double x0, y0, r, xa, ya, xb, yb;
        scanf("%lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &r, &xa, &ya, &xb, &yb);
        
        double dx = xa - x0;
        double dy = ya - y0;
        double dxa = xb - x0;
        double dyb = yb - y0;
        
        double len1_sq = dx * dx + dy * dy;
        double len2_sq = dxa * dxa + dyb * dyb;
        
        double nx = dx / len1_sq + dxa / len2_sq;
        double ny = dy / len1_sq + dyb / len2_sq;
        
        double len = sqrt(nx * nx + ny * ny);
        nx /= len;
        ny /= len;
        
        double cx = x0 + nx * r;
        double cy = y0 + ny * r;
        
        printf("%.6f %.6f\n", cx, cy);
    }
    return 0;
}
