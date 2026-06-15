#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Seg {
    double x1, y1, x2, y2;
    double min_x, max_x, min_y, max_y;
    double dy_dx;
    char color;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Seg> segs(N);
    vector<double> X;

    for (int i = 0; i < N; ++i) {
        cin >> segs[i].x1 >> segs[i].y1 >> segs[i].x2 >> segs[i].y2 >> segs[i].color;
        segs[i].min_x = min(segs[i].x1, segs[i].x2);
        segs[i].max_x = max(segs[i].x1, segs[i].x2);
        segs[i].min_y = min(segs[i].y1, segs[i].y2);
        segs[i].max_y = max(segs[i].y1, segs[i].y2);
        if (abs(segs[i].x1 - segs[i].x2) > 1e-7) {
            segs[i].dy_dx = (segs[i].y2 - segs[i].y1) / (segs[i].x2 - segs[i].x1);
        } else {
            segs[i].dy_dx = 0;
        }
        X.push_back(segs[i].x1);
        X.push_back(segs[i].x2);
    }

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double A1 = segs[i].y2 - segs[i].y1;
            double B1 = segs[i].x1 - segs[i].x2;
            double C1 = A1 * segs[i].x1 + B1 * segs[i].y1;

            double A2 = segs[j].y2 - segs[j].y1;
            double B2 = segs[j].x1 - segs[j].x2;
            double C2 = A2 * segs[j].x1 + B2 * segs[j].y1;

            double D = A1 * B2 - A2 * B1;
            if (abs(D) < 1e-7) continue;

            double X_int = (C1 * B2 - C2 * B1) / D;
            double Y_int = (A1 * C2 - A2 * C1) / D;

            if (X_int >= segs[i].min_x - 1e-7 && X_int <= segs[i].max_x + 1e-7 &&
                Y_int >= segs[i].min_y - 1e-7 && Y_int <= segs[i].max_y + 1e-7 &&
                X_int >= segs[j].min_x - 1e-7 && X_int <= segs[j].max_x + 1e-7 &&
                Y_int >= segs[j].min_y - 1e-7 && Y_int <= segs[j].max_y + 1e-7) {
                X.push_back(X_int);
            }
        }
    }

    sort(X.begin(), X.end());
    vector<double> unique_X;
    for (double x : X) {
        if (unique_X.empty() || x - unique_X.back() > 1e-7) {
            unique_X.push_back(x);
        }
    }
    X = unique_X;

    double SR = 0, SG = 0, SB = 0;

    for (size_t k = 0; k + 1 < X.size(); ++k) {
        double mid = (X[k] + X[k+1]) / 2.0;
        double min_y = 1e18;
        char best_color = ' ';

        for (int i = 0; i < N; ++i) {
            if (mid >= segs[i].min_x - 1e-7 && mid <= segs[i].max_x + 1e-7) {
                double y_at_mid = segs[i].y1 + segs[i].dy_dx * (mid - segs[i].x1);
                if (y_at_mid < min_y) {
                    min_y = y_at_mid;
                    best_color = segs[i].color;
                }
            }
        }

        if (best_color != ' ') {
            double len = X[k+1] - X[k];
            if (best_color == 'R') SR += len;
            else if (best_color == 'G') SG += len;
            else if (best_color == 'B') SB += len;
        }
    }

    cout << fixed << setprecision(2);
    cout << "R " << SR << "\n";
    cout << "G " << SG << "\n";
    cout << "B " << SB << "\n";

    return 0;
}
