#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Ghost {
    double x, y, z, r;
};

int main() {
    double W, H, D;
    cin >> W >> H >> D;
    int N;
    cin >> N;
    vector<Ghost> ghosts;
    for (int i = 0; i < N; i++) {
        double x, y, z, r;
        cin >> x >> y >> z >> r;
        ghosts.push_back({x, y, z, r});
    }

    vector<vector<double>> candidates;

    // 8 corners
    vector<double> xs = {0, W};
    vector<double> ys = {0, H};
    vector<double> zs = {0, D};
    for (double x : xs) {
        for (double y : ys) {
            for (double z : zs) {
                candidates.push_back({x, y, z});
            }
        }
    }

    // center
    candidates.push_back({W/2.0, H/2.0, D/2.0});

    // 3 walls + 1 ghost
    for (int wx = 0; wx < 2; wx++) {
        for (int wy = 0; wy < 2; wy++) {
            for (int wz = 0; wz < 2; wz++) {
                for (int i = 0; i < N; i++) {
                    double ax = (wx == 0) ? 1.0 : -1.0;
                    double bx = (wx == 0) ? 0.0 : W;
                    double ay = (wy == 0) ? 1.0 : -1.0;
                    double by = (wy == 0) ? 0.0 : H;
                    double az = (wz == 0) ? 1.0 : -1.0;
                    double bz = (wz == 0) ? 0.0 : D;

                    double X = ghosts[i].x, Y = ghosts[i].y, Z = ghosts[i].z, R = ghosts[i].r;

                    double A = ax*ax + ay*ay + az*az - 1.0;
                    double B_coef = 2.0 * (ax*(bx - X) + ay*(by - Y) + az*(bz - Z) - R);
                    double C_coef = (bx-X)*(bx-X) + (by-Y)*(by-Y) + (bz-Z)*(bz-Z) - R*R;

                    vector<double> roots;
                    if (fabs(A) < 1e-12) {
                        if (fabs(B_coef) > 1e-12) {
                            double r0 = -C_coef / B_coef;
                            if (r0 >= -1e-12) {
                                roots.push_back(r0);
                            }
                        }
                    } else {
                        double disc = B_coef*B_coef - 4.0*A*C_coef;
                        if (disc >= -1e-12) {
                            if (disc < 0) disc = 0;
                            double r1 = (-B_coef + sqrt(disc)) / (2.0*A);
                            double r2 = (-B_coef - sqrt(disc)) / (2.0*A);
                            if (r1 >= -1e-12) {
                                roots.push_back(r1);
                            }
                            if (r2 >= -1e-12 && fabs(r1 - r2) > 1e-12) {
                                roots.push_back(r2);
                            }
                        }
                    }

                    for (double r0 : roots) {
                        double x = ax * r0 + bx;
                        double y = ay * r0 + by;
                        double z = az * r0 + bz;
                        candidates.push_back({x, y, z});
                    }
                }
            }
        }
    }

    // 4 ghosts: if N >= 4
    if (N >= 4) {
        for (int i0 = 0; i0 < N; i0++) {
            for (int i1 = i0+1; i1 < N; i1++) {
                for (int i2 = i1+1; i2 < N; i2++) {
                    for (int i3 = i2+1; i3 < N; i3++) {
                        vector<int> idx = {i0, i1, i2, i3};
                        vector<vector<double>> eqs;
                        auto make_eq = [&](int a, int b) -> vector<double> {
                            const Ghost& ga = ghosts[a];
                            const Ghost& gb = ghosts[b];
                            double a_coef = 2*(ga.x - gb.x);
                            double b_coef = 2*(ga.y - gb.y);
                            double c_coef = 2*(ga.z - gb.z);
                            double d_coef = -2*(gb.r - ga.r);
                            double e_coef = gb.r*gb.r - ga.r*ga.r 
                                            + ga.x*ga.x + ga.y*ga.y + ga.z*ga.z 
                                            - gb.x*gb.x - gb.y*gb.y - gb.z*gb.z;
                            return {a_coef, b_coef, c_coef, d_coef, e_coef};
                        };
                        eqs.push_back(make_eq(0, 1));
                        eqs.push_back(make_eq(0, 2));
                        eqs.push_back(make_eq(0, 3));

                        vector<vector<double>> mat(3, vector<double>(5, 0.0));
                        for (int r = 0; r < 3; r++) {
                            for (int c = 0; c < 5; c++) {
                                mat[r][c] = eqs[r][c];
                            }
                        }

                        // Gaussian elimination with partial pivoting
                        for (int col = 0; col < 3; col++) {
                            int pivot_row = col;
                            for (int row = col; row < 3; row++) {
                                if (fabs(mat[row][col]) > fabs(mat[pivot_row][col])) {
                                    pivot_row = row;
                                }
                            }
                            if (fabs(mat[pivot_row][col]) < 1e-12) {
                                continue;
                            }
                            swap(mat[col], mat[pivot_row]);
                            for (int row = col + 1; row < 3; row++) {
                                double factor = mat[row][col] / mat[col][col];
                                for (int c = col; c < 5; c++) {
                                    mat[row][c] -= factor * mat[col][c];
                                }
                            }
                        }

                        // Check for singular matrix
                        if (fabs(mat[2][2]) < 1e-12 || fabs(mat[1][1]) < 1e-12 || fabs(mat[0][0]) < 1e-12) {
                            continue;
                        }

                        // Back substitution: z, y, x in terms of r (the last variable)
                        double z_coef = -mat[2][3] / mat[2][2];
                        double z_const = mat[2][4] / mat[2][2];
                        double y_coef = (-mat[1][2] * z_coef - mat[1][3]) / mat[1][1];
                        double y_const = (mat[1][4] - mat[1][2] * z_const) / mat[1][1];
                        double x_coef = (-mat[0][1] * y_coef - mat[0][2] * z_coef - mat[0][3]) / mat[0][0];
                        double x_const = (mat[0][4] - mat[0][1] * y_const - mat[0][2] * z_const) / mat[0][0];

                        // Use the first ghost in the combination for the sphere equation
                        double X0 = ghosts[idx[0]].x;
                        double Y0 = ghosts[idx[0]].y;
                        double Z0 = ghosts[idx[0]].z;
                        double R0 = ghosts[idx[0]].r;

                        double dx0 = x_coef;
                        double dx1 = x_const - X0;
                        double dy0 = y_coef;
                        double dy1 = y_const - Y0;
                        double dz0 = z_coef;
                        double dz1 = z_const - Z0;

                        double A = dx0*dx0 + dy0*dy0 + dz0*dz0 - 1.0;
                        double B_coef = 2.0 * (dx0*dx1 + dy0*dy1 + dz0*dz1 - R0);
                        double C_coef = dx1*dx1 + dy1*dy1 + dz1*dz1 - R0*R0;

                        vector<double> roots;
                        if (fabs(A) < 1e-12) {
                            if (fabs(B_coef) > 1e-12) {
                                double r0 = -C_coef / B_coef;
                                if (r0 >= -1e-12) {
                                    roots.push_back(r0);
                                }
                            }
                        } else {
                            double disc = B_coef*B_coef - 4.0*A*C_coef;
                            if (disc >= -1e-12) {
                                if (disc < 0) disc = 0;
                                double r1 = (-B_coef + sqrt(disc)) / (2.0*A);
                                double r2 = (-B_coef - sqrt(disc)) / (2.0*A);
                                if (r1 >= -1e-12) {
                                    roots.push_back(r1);
                                }
                                if (r2 >= -1e-12 && fabs(r1 - r2) > 1e-12) {
                                    roots.push_back(r2);
                                }
                            }
                        }

                        for (double r0 : roots) {
                            double x = x_coef * r0 + x_const;
                            double y = y_coef * r0 + y_const;
                            double z = z_coef * r0 + z_const;
                            candidates.push_back({x, y, z});
                        }
                    }
                }
            }
        }
    }

    // Evaluate all candidates
    double best_r = -1.0;
    vector<double> best_point(3);

    for (const auto& cand : candidates) {
        double x = cand[0];
        double y = cand[1];
        double z = cand[2];
        double r = 1e30;

        // Constraints from the asylum walls
        r = min(r, x);
        r = min(r, W - x);
        r = min(r, y);
        r = min(r, H - y);
        r = min(r, z);
        r = min(r, D - z);

        // Constraints from the ghosts
        for (int i = 0; i < N; i++) {
            double dx = x - ghosts[i].x;
            double dy = y - ghosts[i].y;
            double dz = z - ghosts[i].z;
            double dist = sqrt(dx*dx + dy*dy + dz*dz);
            double candidate_r = dist - ghosts[i].r;
            if (candidate_r < 0) candidate_r = 0;
            r = min(r, candidate_r);
        }

        if (r > best_r) {
            best_r = r;
            best_point[0] = x;
            best_point[1] = y;
            best_point[2] = z;
        }
    }

    cout << fixed << setprecision(20);
    cout << best_point[0] << " " << best_point[1] << " " << best_point[2] << endl;

    return 0;
}
