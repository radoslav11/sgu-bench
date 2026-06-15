#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long double xA, yA, vAx, vAy, rA, mA;
    if (!(cin >> xA >> yA >> vAx >> vAy >> rA >> mA)) return 0;
    
    long double xB, yB, vBx, vBy, rB, mB;
    cin >> xB >> yB >> vBx >> vBy >> rB >> mB;
    
    long double t;
    cin >> t;

    long double dx0 = xA - xB;
    long double dy0 = yA - yB;
    long double dvx = vAx - vBx;
    long double dvy = vAy - vBy;

    long double a = dvx * dvx + dvy * dvy;
    long double b = 2.0 * (dx0 * dvx + dy0 * dvy);
    long double c = dx0 * dx0 + dy0 * dy0 - (rA + rB) * (rA + rB);

    long double t_coll = -1.0;
    long double D = b * b - 4.0 * a * c;

    if (a > 1e-12 && D >= -1e-9) {
        if (D < 0) D = 0;
        long double t1 = (-b - sqrt(D)) / (2.0 * a);
        if (t1 >= -1e-9 && t1 <= t + 1e-9) {
            t_coll = max((long double)0.0, t1);
            if (t_coll > t) t_coll = t;
        }
    }

    if (t_coll >= 0) {
        long double xA_c = xA + vAx * t_coll;
        long double yA_c = yA + vAy * t_coll;
        long double xB_c = xB + vBx * t_coll;
        long double yB_c = yB + vBy * t_coll;

        long double dx_c = xB_c - xA_c;
        long double dy_c = yB_c - yA_c;
        long double dist_c = sqrt(dx_c * dx_c + dy_c * dy_c);
        
        long double nx = dx_c / dist_c;
        long double ny = dy_c / dist_c;

        long double vA_para = vAx * nx + vAy * ny;
        long double vB_para = vBx * nx + vBy * ny;

        long double vA_para_new = ((mA - mB) * vA_para + 2.0 * mB * vB_para) / (mA + mB);
        long double vB_para_new = ((mB - mA) * vB_para + 2.0 * mA * vA_para) / (mA + mB);

        long double dvA_para = vA_para_new - vA_para;
        long double dvB_para = vB_para_new - vB_para;

        long double vAx_new = vAx + dvA_para * nx;
        long double vAy_new = vAy + dvA_para * ny;
        long double vBx_new = vBx + dvB_para * nx;
        long double vBy_new = vBy + dvB_para * ny;

        long double dt = t - t_coll;
        xA = xA_c + vAx_new * dt;
        yA = yA_c + vAy_new * dt;
        xB = xB_c + vBx_new * dt;
        yB = yB_c + vBy_new * dt;

        vAx = vAx_new;
        vAy = vAy_new;
        vBx = vBx_new;
        vBy = vBy_new;
    } else {
        xA += vAx * t;
        yA += vAy * t;
        xB += vBx * t;
        yB += vBy * t;
    }

    cout << fixed << setprecision(6);
    cout << xA << " " << yA << " " << vAx << " " << vAy << "\n";
    cout << xB << " " << yB << " " << vBx << " " << vBy << "\n";

    return 0;
}
