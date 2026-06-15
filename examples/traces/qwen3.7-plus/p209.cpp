#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Point {
    double x, y;
};

struct Line {
    double A, B, C;
};

const double EPS = 1e-11;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Line> L(N);
    for (int i = 0; i < N; ++i) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        double A = y2 - y1;
        double B = x1 - x2;
        double C = x2 * y1 - x1 * y2;
        double len = hypot(A, B);
        L[i] = {A / len, B / len, C / len};
    }

    vector<Point> pts;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double det = L[i].A * L[j].B - L[j].A * L[i].B;
            if (abs(det) < EPS) continue;
            double x = (L[i].B * L[j].C - L[j].B * L[i].C) / det;
            double y = (L[i].C * L[j].A - L[j].C * L[i].A) / det;
            pts.push_back({x, y});
        }
    }

    vector<Point> unique_pts;
    for (const auto& p : pts) {
        bool found = false;
        for (const auto& q : unique_pts) {
            if (hypot(p.x - q.x, p.y - q.y) < EPS) {
                found = true;
                break;
            }
        }
        if (!found) {
            unique_pts.push_back(p);
        }
    }

    int M = unique_pts.size();
    if (M == 0) {
        cout << 0 << "\n";
        return 0;
    }

    vector<vector<int>> pts_on_line(N);
    vector<vector<int>> line_ids_per_pt(M);
    for (int i = 0; i < M; ++i) {
        for (int k = 0; k < N; ++k) {
            if (abs(L[k].A * unique_pts[i].x + L[k].B * unique_pts[i].y + L[k].C) < EPS) {
                pts_on_line[k].push_back(i);
                line_ids_per_pt[i].push_back(k);
            }
        }
    }

    vector<vector<int>> N_vec(M);
    for (int i = 0; i < M; ++i) {
        vector<int> neighbors;
        for (int k : line_ids_per_pt[i]) {
            double dx = -L[k].B;
            double dy = L[k].A;
            double len = hypot(dx, dy);
            dx /= len; dy /= len;

            double min_pos_t = 1e9, min_neg_t = 1e9;
            int pos_v = -1, neg_v = -1;

            for (int j : pts_on_line[k]) {
                if (i == j) continue;
                double t = (unique_pts[j].x - unique_pts[i].x) * dx + (unique_pts[j].y - unique_pts[i].y) * dy;
                if (t > EPS && t < min_pos_t) {
                    min_pos_t = t;
                    pos_v = j;
                }
                if (t < -EPS && -t < min_neg_t) {
                    min_neg_t = -t;
                    neg_v = j;
                }
            }
            if (pos_v != -1) neighbors.push_back(pos_v);
            if (neg_v != -1) neighbors.push_back(neg_v);
        }

        sort(neighbors.begin(), neighbors.end(), [&](int a, int b) {
            return atan2(unique_pts[a].y - unique_pts[i].y, unique_pts[a].x - unique_pts[i].x) <
                   atan2(unique_pts[b].y - unique_pts[i].y, unique_pts[b].x - unique_pts[i].x);
        });

        neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
        N_vec[i] = neighbors;
    }

    vector<vector<bool>> visited(M, vector<bool>(M, false));
    vector<double> areas;

    for (int i = 0; i < M; ++i) {
        for (int j : N_vec[i]) {
            if (visited[i][j]) continue;

            vector<int> cycle;
            int curr = i, next = j;
            bool valid_cycle = true;

            while (true) {
                visited[curr][next] = true;
                cycle.push_back(curr);

                int idx = -1;
                for (size_t k = 0; k < N_vec[next].size(); ++k) {
                    if (N_vec[next][k] == curr) {
                        idx = k;
                        break;
                    }
                }

                if (idx == -1) {
                    valid_cycle = false;
                    break;
                }

                int nxt_node = N_vec[next][(idx + 1) % N_vec[next].size()];
                if (nxt_node == i) {
                    cycle.push_back(next);
                    break;
                }

                curr = next;
                next = nxt_node;

                if (cycle.size() > M + 5) {
                    valid_cycle = false;
                    break;
                }
            }

            if (!valid_cycle || cycle.size() < 3) continue;

            double area = 0;
            int sz = cycle.size();
            for (int k = 0; k < sz; ++k) {
                int u = cycle[k];
                int v = cycle[(k + 1) % sz];
                area += unique_pts[u].x * unique_pts[v].y - unique_pts[v].x * unique_pts[u].y;
            }
            area = abs(area) / 2.0;

            if (area <= 1e-8) continue;

            bool valid = true;
            for (int k = 0; k < N; ++k) {
                bool has_pos = false, has_neg = false;
                for (int u : cycle) {
                    double val = L[k].A * unique_pts[u].x + L[k].B * unique_pts[u].y + L[k].C;
                    if (val > EPS) { has_pos = true; if (has_neg) break; }
                    if (val < -EPS) { has_neg = true; if (has_pos) break; }
                }
                if (has_pos && has_neg) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                areas.push_back(area);
            }
        }
    }

    sort(areas.begin(), areas.end());
    cout << areas.size() << "\n";
    cout << fixed << setprecision(4);
    for (double a : areas) {
        cout << a << "\n";
    }

    return 0;
}
