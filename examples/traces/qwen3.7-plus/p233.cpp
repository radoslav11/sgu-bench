#include <iostream>
#include <cmath>
#include <cstdio>

using namespace std;

int main() {
    int N;
    if (scanf("%d", &N) != 1) return 0;
    while (N--) {
        double X0, Y0, R, XA, YA, XB, YB;
        if (scanf("%lf %lf %lf %lf %lf %lf %lf", &X0, &Y0, &R, &XA, &YA, &XB, &YB) != 7) break;
        
        double ax = XA - X0;
        double ay = YA - Y0;
        double bx = XB - X0;
        double by = YB - Y0;
        
        double mx = (ax + bx) / 2.0;
        double my = (ay + by) / 2.0;
        
        double dx = ax - bx;
        double dy = ay - by;
        double D = sqrt(dx * dx + dy * dy);
        
        if (D < 1e-9) {
            printf("%.6f %.6f\n", X0 + R, Y0);
            continue;
        }
        
        double ux = dy / D;
        double uy = -dx / D;
        
        double h2 = (D * D) / 4.0;
        double V = mx * ux + my * uy;
        double K = R * R - (ax * bx + ay * by);
        
        double A_eq = V * V - R * R;
        double B_eq = -K * V;
        double C_eq = (K * K - 4.0 * R * R * h2) / 4.0;
        
        double delta = B_eq * B_eq - 4.0 * A_eq * C_eq;
        if (delta < 0) delta = 0;
        
        double sqrt_delta = sqrt(delta);
        
        double t1 = (-B_eq + sqrt_delta) / (2.0 * A_eq);
        double t2 = (-B_eq - sqrt_delta) / (2.0 * A_eq);
        
        double max_angle = -1.0;
        double best_cx = X0 + R;
        double best_cy = Y0;
        
        auto check_t = [&](double t) {
            if (K - 2.0 * V * t < -1e-7) return;
            
            double o_prime_x = mx + t * ux;
            double o_prime_y = my + t * uy;
            double r_prime = sqrt(t * t + h2);
            
            double dist_O_Oprime = sqrt(o_prime_x * o_prime_x + o_prime_y * o_prime_y);
            if (dist_O_Oprime < 1e-9) return;
            
            double dir_x = o_prime_x / dist_O_Oprime;
            double dir_y = o_prime_y / dist_O_Oprime;
            
            double c1_x = X0 + R * dir_x;
            double c1_y = Y0 + R * dir_y;
            double c2_x = X0 - R * dir_x;
            double c2_y = Y0 - R * dir_y;
            
            auto get_dist = [](double x1, double y1, double x2, double y2) {
                double dxx = x1 - x2;
                double dyy = y1 - y2;
                return sqrt(dxx * dxx + dyy * dyy);
            };
            
            auto get_angle = [&](double cx, double cy) {
                double cax = XA - cx;
                double cay = YA - cy;
                double cbx = XB - cx;
                double cby = YB - cy;
                double dot = cax * cbx + cay * cby;
                double len_ca = sqrt(cax * cax + cay * cay);
                double len_cb = sqrt(cbx * cbx + cby * cby);
                if (len_ca < 1e-9 || len_cb < 1e-9) return 0.0;
                double cos_theta = dot / (len_ca * len_cb);
                if (cos_theta > 1.0) cos_theta = 1.0;
                if (cos_theta < -1.0) cos_theta = -1.0;
                return acos(cos_theta);
            };
            
            double d1 = get_dist(c1_x, c1_y, o_prime_x + X0, o_prime_y + Y0);
            double d2 = get_dist(c2_x, c2_y, o_prime_x + X0, o_prime_y + Y0);
            
            double best_d = d1;
            double cx = c1_x, cy = c1_y;
            if (fabs(d2 - r_prime) < fabs(d1 - r_prime)) {
                best_d = d2;
                cx = c2_x;
                cy = c2_y;
            }
            
            if (fabs(best_d - r_prime) < 1e-3) {
                double angle = get_angle(cx, cy);
                if (angle > max_angle) {
                    max_angle = angle;
                    best_cx = cx;
                    best_cy = cy;
                }
            }
        };
        
        check_t(t1);
        check_t(t2);
        
        printf("%.6f %.6f\n", best_cx, best_cy);
    }
    return 0;
}
