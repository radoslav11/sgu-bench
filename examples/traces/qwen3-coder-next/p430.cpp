#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Point {
    double x, y;
    Point(double x=0, double y=0) : x(x), y(y) {}
};

vector<vector<bool>> adj;
vector<Point> p;
int n_global;

bool is_valid(int i, const Point& cand) {
    for (int j = 0; j < i; j++) {
        double dx = cand.x - p[j].x;
        double dy = cand.y - p[j].y;
        double d = sqrt(dx*dx + dy*dy);
        if (adj[i][j]) {
            if (fabs(d - 1.0) > 1e-5) {
                return false;
            }
        } else {
            if (fabs(d - 1.0) < 0.009) {
                return false;
            }
        }
    }
    return true;
}

bool dfs(int i) {
    if (i == n_global) {
        return true;
    }

    vector<Point> candidates;

    vector<int> neighbors;
    for (int j = 0; j < i; j++) {
        if (adj[i][j]) {
            neighbors.push_back(j);
        }
    }

    if (neighbors.empty()) {
        for (int x_int = -4; x_int <= 4; x_int++) {
            for (int y_int = -4; y_int <= 4; y_int++) {
                double x = x_int * 0.5;
                double y = y_int * 0.5;
                Point cand(x, y);
                if (is_valid(i, cand)) {
                    candidates.push_back(cand);
                }
            }
        }
        if (candidates.empty()) {
            for (int try_idx = 0; try_idx < 300; try_idx++) {
                double x = (rand() % 20000 - 10000) / 10000.0 * 3.0;
                double y = (rand() % 20000 - 10000) / 10000.0 * 3.0;
                Point cand(x, y);
                if (is_valid(i, cand)) {
                    candidates.push_back(cand);
                }
            }
        }
    } else if (neighbors.size() == 1) {
        int j = neighbors[0];
        for (int deg = 0; deg < 360; deg += 5) {
            double rad = deg * M_PI / 180.0;
            Point cand;
            cand.x = p[j].x + cos(rad);
            cand.y = p[j].y + sin(rad);
            if (is_valid(i, cand)) {
                candidates.push_back(cand);
            }
        }
    } else {
        for (int a = 0; a < neighbors.size(); a++) {
            for (int b = a + 1; b < neighbors.size(); b++) {
                int j = neighbors[a];
                int k = neighbors[b];
                double xj = p[j].x, yj = p[j].y;
                double xk = p[k].x, yk = p[k].y;
                double dx = xk - xj;
                double dy = yk - yj;
                double d_sq = dx*dx + dy*dy;
                double d = sqrt(d_sq);
                if (d > 2.0 + 1e-8) {
                    continue;
                }
                double h = sqrt(max(0.0, 1.0 - (d/2.0)*(d/2.0)));
                double midx = (xj + xk) / 2.0;
                double midy = (yj + yk) / 2.0;
                double nx = -dy / d;
                double ny = dx / d;
                Point cand1 = Point(midx + h * nx, midy + h * ny);
                Point cand2 = Point(midx - h * nx, midy - h * ny);
                if (is_valid(i, cand1)) {
                    candidates.push_back(cand1);
                }
                if (is_valid(i, cand2)) {
                    candidates.push_back(cand2);
                }
            }
        }
    }

    for (const Point& cand : candidates) {
        p[i] = cand;
        if (dfs(i + 1)) {
            return true;
        }
    }

    return false;
}

int main() {
    srand(time(0));
    int n, m;
    cin >> n >> m;
    n_global = n;
    adj = vector<vector<bool>>(n, vector<bool>(n, false));
    p = vector<Point>(n, Point(0, 0));

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u][v] = true;
        adj[v][u] = true;
    }

    if (n == 1) {
        cout << "Yes" << endl;
        cout << fixed << setprecision(15) << 0.0 << " " << 0.0 << endl;
        return 0;
    }

    p[0] = Point(0, 0);

    if (adj[0][1]) {
        p[1] = Point(1, 0);
        if (dfs(2)) {
            cout << "Yes" << endl;
            for (int i = 0; i < n; i++) {
                cout << fixed << setprecision(15) << p[i].x << " " << p[i].y << endl;
            }
            return 0;
        }
    } else {
        vector<Point> cand1 = {
            Point(0.5, 0), Point(1.5, 0), Point(2, 0), Point(3, 0),
            Point(0, 0.5), Point(0, 1.5), Point(0, 2), Point(0, 3),
            Point(2, 2), Point(3, 3), Point(1, 1), Point(1, 2)
        };
        for (const Point& p1 : cand1) {
            p[1] = p1;
            if (dfs(2)) {
                cout << "Yes" << endl;
                for (int i = 0; i < n; i++) {
                    cout << fixed << setprecision(15) << p[i].x << " " << p[i].y << endl;
                }
                return 0;
            }
        }
    }

    cout << "No" << endl;
    return 0;
}
