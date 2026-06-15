#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>

using namespace std;

const double INF = 1e18;
const int MAX = 505;

int N, M;
int flow_grid[MAX][MAX];
int dr[] = {0, 0, 0, 1, -1};
int dc[] = {0, -1, 1, 0, 0};

struct Pos {
    int r, c;
};

vector<Pos> boat_path;
int boat_v[MAX * MAX + 10];
int cycle_start = -1;

Pos get_boat_pos_at_int(int m) {
    if (m < (int)boat_path.size()) return boat_path[m];
    if (cycle_start == -1) return boat_path.back();
    int len = (int)boat_path.size() - cycle_start;
    return boat_path[cycle_start + (m - cycle_start) % len];
}

int get_boat_v_at_int(int m) {
    if (m < (int)boat_path.size()) return boat_v[m];
    if (cycle_start == -1) return 0;
    int len = (int)boat_path.size() - cycle_start;
    return boat_v[cycle_start + (m - cycle_start) % len];
}

double dist[MAX][MAX];
int visited_boat[MAX][MAX];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M)) return 0;

    for (int i = 1; i <= N; ++i) {
        string row;
        cin >> row;
        for (int j = 1; j <= M; ++j) {
            flow_grid[i][j] = row[j - 1] - '0';
        }
    }

    int pr, pc, br, bc;
    cin >> pr >> pc >> br >> bc;

    int r = br, c = bc;
    for (int i = 0; i < MAX; ++i)
        for (int j = 0; j < MAX; ++j)
            visited_boat[i][j] = -1;

    for (int m = 0; ; ++m) {
        if (r <= 0 || r > N || c <= 0 || c > M) break;
        if (visited_boat[r][c] != -1) {
            cycle_start = visited_boat[r][c];
            break;
        }
        visited_boat[r][c] = m;
        boat_path.push_back({r, c});
        int v = flow_grid[r][c];
        boat_v[m] = v;
        if (v == 0) break;
        r += dr[v];
        c += dc[v];
    }

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            dist[i][j] = INF;
        }
    }

    priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<pair<double, pair<int, int>>>> pq;

    dist[pr][pc] = 0;
    pq.push({0.0, {pr, pc}});

    double min_save_time = INF;

    while (!pq.empty()) {
        double t = pq.top().first;
        int r = pq.top().second.first;
        int c = pq.top().second.second;
        pq.pop();

        if (t > dist[r][c]) continue;
        if (t >= min_save_time) continue;

        for (int vp = 0; vp < 5; ++vp) {
            int vr = dr[vp], vc = dc[vp];
            int vf = flow_grid[r][c];
            int V_x = dr[vf] + vr;
            int V_y = dc[vf] + vc;

            if (V_x == 0 && V_y == 0) continue;

            int nr = r + (V_x > 0 ? 1 : (V_x < 0 ? -1 : 0));
            int nc = c + (V_y > 0 ? 1 : (V_y < 0 ? -1 : 0));
            double dt = 1.0 / max(abs(V_x), abs(V_y));

            double t_shore = INF;
            if (V_x > 0) t_shore = min(t_shore, t + (double)(N + 1 - r) / V_x);
            if (V_x < 0) t_shore = min(t_shore, t + (double)(0 - r) / V_x);
            if (V_y > 0) t_shore = min(t_shore, t + (double)(M + 1 - c) / V_y);
            if (V_y < 0) t_shore = min(t_shore, t + (double)(0 - c) / V_y);
            
            if (t_shore <= t + dt + 1e-9) {
                min_save_time = min(min_save_time, t_shore);
            }

            int k = (int)floor(t + 1e-11);
            if (t + dt <= k + 1 + 1e-11) {
                // Single interval [t, t + dt]
                double a = t, b = t + dt;
                int m = k;
                Pos Bm = get_boat_pos_at_int(m);
                int bv = get_boat_v_at_int(m);
                int Bvx = dr[bv], Bvy = dc[bv];
                double Pax = r + (a - t) * V_x;
                double Pay = c + (a - t) * V_y;
                double Bmx = Bm.r, Bmy = Bm.c;
                double Ax = V_x - Bvx, Ay = V_y - Bvy;
                double Cx = Pax - Bmx + m * Bvx - a * V_x;
                double Cy = Pay - Bmy + m * Bvy - a * V_y;

                bool possible = false;
                double tau_res = -1;
                if (abs(Ax) > 1e-11 && abs(Ay) > 1e-11) {
                    double tx = -Cx / Ax, ty = -Cy / Ay;
                    if (abs(tx - ty) < 1e-7) { tau_res = tx; possible = true; }
                } else if (abs(Ax) > 1e-11) {
                    double tx = -Cx / Ax;
                    if (abs(Cy + tx * Ay) < 1e-7) { tau_res = tx; possible = true; }
                } else if (abs(Ay) > 1e-11) {
                    double ty = -Cy / Ay;
                    if (abs(Cx + ty * Ax) < 1e-7) { tau_res = ty; possible = true; }
                } else if (abs(Cx) < 1e-7 && abs(Cy) < 1e-7) {
                    tau_res = a; possible = true;
                }
                if (possible && tau_res >= a - 1e-9 && tau_res <= b + 1e-9) min_save_time = min(min_save_time, tau_res);
            } else {
                // Two intervals [t, k+1] and [k+1, t + dt]
                double bounds[2][2] = {{t, (double)k + 1}, {(double)k + 1, t + dt}};
                for (int i = 0; i < 2; ++i) {
                    double a = bounds[i][0], b = bounds[i][1];
                    int m = (i == 0) ? k : k + 1;
                    Pos Bm = get_boat_pos_at_int(m);
                    int bv = get_boat_v_at_int(m);
                    int Bvx = dr[bv], Bvy = dc[bv];
                    double Pax = r + (a - t) * V_x, Pay = c + (a - t) * V_y;
                    double Bmx = Bm.r, Bmy = Bm.c;
                    double Ax = V_x - Bvx, Ay = V_y - Bvy;
                    double Cx = Pax - Bmx + m * Bvx - a * V_x;
                    double Cy = Pay - Bmy + m * Bvy - a * V_y;
                    bool possible = false;
                    double tau_res = -1;
                    if (abs(Ax) > 1e-11 && abs(Ay) > 1e-11) {
                        double tx = -Cx / Ax, ty = -Cy / Ay;
                        if (abs(tx - ty) < 1e-7) { tau_res = tx; possible = true; }
                    } else if (abs(Ax) > 1e-11) {
                        double tx = -Cx / Ax;
                        if (abs(Cy + tx * Ay) < 1e-7) { tau_res = tx; possible = true; }
                    } else if (abs(Ay) > 1e-11) {
                        double ty = -Cy / Ay;
                        if (abs(Cx + ty * Ax) < 1e-7) { tau_res = ty; possible = true; }
                    } else if (abs(Cx) < 1e-7 && abs(Cy) < 1e-7) {
                        tau_res = a; possible = true;
                    }
                    if (possible && tau_res >= a - 1e-9 && tau_res <= b + 1e-9) min_save_time = min(min_save_time, tau_res);
                }
            }

            if (nr >= 1 && nr <= N && nc >= 1 && nc <= M) {
                if (t + dt < dist[nr][nc]) {
                    dist[nr][nc] = t + dt;
                    pq.push({dist[nr][nc], {nr, nc}});
                }
            }
        }
    }

    if (min_save_time == INF) cout << "SOS" << endl;
    else cout << fixed << setprecision(2) << min_save_time << endl;

    return 0;
}

