#include <cstdio>
#include <cmath>
#include <cstring>

using namespace std;

struct Matrix {
    double m[4][4];
    Matrix() {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                m[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
    }
    Matrix operator*(const Matrix& other) const {
        Matrix res;
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                res.m[i][j] = 0.0;
                for(int k = 0; k < 4; ++k) {
                    res.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return res;
    }
};

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    
    Matrix total;
    for(int i = 0; i < n; ++i) {
        char type;
        scanf(" %c", &type);
        Matrix current;
        if (type == 'T') {
            double tx, ty, tz;
            scanf("%lf %lf %lf", &tx, &ty, &tz);
            current.m[0][3] = tx;
            current.m[1][3] = ty;
            current.m[2][3] = tz;
        } else if (type == 'S') {
            double sx, sy, sz;
            scanf("%lf %lf %lf", &sx, &sy, &sz);
            current.m[0][0] = sx;
            current.m[1][1] = sy;
            current.m[2][2] = sz;
        } else if (type == 'R') {
            double rx, ry, rz, angle;
            scanf("%lf %lf %lf %lf", &rx, &ry, &rz, &angle);
            double len = sqrt(rx*rx + ry*ry + rz*rz);
            double nx = rx / len;
            double ny = ry / len;
            double nz = rz / len;
            double rad = angle * acos(-1.0) / 180.0;
            double c = cos(rad);
            double s = sin(rad);
            double omc = 1.0 - c;
            
            current.m[0][0] = c + nx*nx*omc;
            current.m[0][1] = nx*ny*omc - nz*s;
            current.m[0][2] = nx*nz*omc + ny*s;
            current.m[0][3] = 0.0;

            current.m[1][0] = ny*nx*omc + nz*s;
            current.m[1][1] = c + ny*ny*omc;
            current.m[1][2] = ny*nz*omc - nx*s;
            current.m[1][3] = 0.0;

            current.m[2][0] = nz*nx*omc - ny*s;
            current.m[2][1] = nz*ny*omc + nx*s;
            current.m[2][2] = c + nz*nz*omc;
            current.m[2][3] = 0.0;

            current.m[3][0] = 0.0;
            current.m[3][1] = 0.0;
            current.m[3][2] = 0.0;
            current.m[3][3] = 1.0;
        }
        total = current * total;
    }
    
    int m;
    if (scanf("%d", &m) != 1) return 0;
    for(int i = 0; i < m; ++i) {
        double vx, vy, vz;
        scanf("%lf %lf %lf", &vx, &vy, &vz);
        
        double rx = total.m[0][0]*vx + total.m[0][1]*vy + total.m[0][2]*vz + total.m[0][3];
        double ry = total.m[1][0]*vx + total.m[1][1]*vy + total.m[1][2]*vz + total.m[1][3];
        double rz = total.m[2][0]*vx + total.m[2][1]*vy + total.m[2][2]*vz + total.m[2][3];
        
        char buf1[32], buf2[32], buf3[32];
        sprintf(buf1, "%.2f", rx);
        sprintf(buf2, "%.2f", ry);
        sprintf(buf3, "%.2f", rz);
        
        if (strcmp(buf1, "-0.00") == 0) strcpy(buf1, "0.00");
        if (strcmp(buf2, "-0.00") == 0) strcpy(buf2, "0.00");
        if (strcmp(buf3, "-0.00") == 0) strcpy(buf3, "0.00");
        
        printf("%s %s %s\n", buf1, buf2, buf3);
    }
    
    return 0;
}
