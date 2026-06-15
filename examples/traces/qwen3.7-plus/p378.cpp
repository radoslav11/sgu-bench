#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const double EPS = 1e-9;
const double INF = 1e18;

struct Segment {
    double t1, t2;
    double x1, y1, x2, y2;
    double vx, vy;
};

struct State {
    double t;
    int x, y;
    bool operator>(const State& other) const {
        return t > other.t;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<string> grid(N);
    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
    }

    int px, py, bx, by;
    cin >> px >> py >> bx >> by;

    vector<Segment> boat_segs;
    vector<vector<bool>> boat_visited(M + 1, vector<bool>(N + 1, false));

    double cx = bx, cy = by;
    double ct = 0.0;
    boat_visited[bx][by] = true;

    while (true) {
        int cell_x = round(cx);
        int cell_y = round(cy);

        if (cell_x < 1 || cell_x > M || cell_y < 1 || cell_y > N) {
            boat_segs.push_back({ct, 1e9, cx, cy, cx, cy, 0.0, 0.0});
            break;
        }

        char c = grid[cell_y - 1][cell_x - 1];
        double vx = 0.0, vy = 0.0;
        if (c == '1') vy = -1.0;
        else if (c == '2') vy = 1.0;
        else if (c == '3') vx = 1.0;
        else if (c == '4') vx = -1.0;

        if (vx == 0.0 && vy == 0.0) {
            boat_segs.push_back({ct, 1e9, cx, cy, cx, cy, 0.0, 0.0});
            break;
        }

        double dt_x = (vx > 0) ? (1.0 / vx) : ((vx < 0) ? (1.0 / -vx) : 1e9);
        double dt_y = (vy > 0) ? (1.0 / vy) : ((vy < 0) ? (1.0 / -vy) : 1e9);
        double dt = min(dt_x, dt_y);

        double n_cx = cx, n_cy = cy;
        if (abs(dt - dt_x) < EPS) {
            n_cx += (vx > 0 ? 1.0 : -1.0);
        }
        if (abs(dt - dt_y) < EPS) {
            n_cy += (vy > 0 ? 1.0 : -1.0);
        }

        boat_segs.push_back({ct, ct + dt, cx, cy, n_cx, n_cy, vx, vy});
        ct += dt;
        cx = n_cx;
        cy = n_cy;

        int n_cell_x = round(cx);
        int n_cell_y = round(cy);

        if (n_cell_x < 1 || n_cell_x > M || n_cell_y < 1 || n_cell_y > N) {
            boat_segs.push_back({ct, 1e9, cx, cy, cx, cy, 0.0, 0.0});
            break;
        }

        if (boat_visited[n_cell_x][n_cell_y]) {
            break;
        }
        boat_visited[n_cell_x][n_cell_y] = true;
    }

    vector<vector<double>> dist(M + 1, vector<double>(N + 1, INF));
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[px][py] = 0.0;
    pq.push({0.0, px, py});

    double ans = INF;

    int dx[5] = {0, 0, 1, -1, 0};
    int dy[5] = {-1, 1, 0, 0, 0};

    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();

        double t = curr.t;
        int x = curr.x;
        int y = curr.y;

        if (t > dist[x][y] + EPS) continue;

        // Check if we can meet the boat by waiting at (x, y)
        for (const auto& seg : boat_segs) {
            if (abs(seg.vx) < EPS && abs(seg.vy) < EPS) {
                if (abs(seg.x1 - x) < EPS && abs(seg.y1 - y) < EPS) {
                    if (t <= seg.t2 + EPS) {
                        ans = min(ans, max(t, seg.t1));
                    }
                }
            }
        }

        for (int i = 0; i < 5; ++i) {
            double pvx = 0.0, pvy = 0.0;
            char c = grid[y - 1][x - 1];
            double fx = 0.0, fy = 0.0;
            if (c == '1') fy = -1.0;
            else if (c == '2') fy = 1.0;
            else if (c == '3') fx = 1.0;
            else if (c == '4') fx = -1.0;

            pvx = dx[i];
            pvy = dy[i];

            double vx = fx + pvx;
            double vy = fy + pvy;

            if (abs(vx) < EPS && abs(vy) < EPS) continue;

            // Time to hit next cell center
            double dt_cell_x = 1e9, dt_cell_y = 1e9;
            double nx = x, ny = y;
            if (vx > 0) {
                dt_cell_x = 1.0 / vx;
                nx = x + 1;
            } else if (vx < 0) {
                dt_cell_x = 1.0 / (-vx);
                nx = x - 1;
            }

            if (vy > 0) {
                dt_cell_y = 1.0 / vy;
                ny = y + 1;
            } else if (vy < 0) {
                dt_cell_y = 1.0 / (-vy);
                ny = y - 1;
            }

            double dt = min(dt_cell_x, dt_cell_y);
            if (dt > 1e8) continue;

            int next_x = x, next_y = y;
            if (abs(dt - dt_cell_x) < EPS && abs(dt - dt_cell_y) < EPS) {
                next_x = nx;
                next_y = ny;
            } else if (abs(dt - dt_cell_x) < EPS) {
                next_x = nx;
            } else {
                next_y = ny;
            }

            // Check intersection with boat during [t, t + dt]
            auto it = lower_bound(boat_segs.begin(), boat_segs.end(), t + dt,
                [](const Segment& seg, double val) {
                    return seg.t2 < val;
                });
            
            // Check backwards
            auto it2 = it;
            while (it2 != boat_segs.begin()) {
                --it2;
                if (it2->t2 < t - EPS) break;
                
                double start_s = max(t, it2->t1);
                double end_s = min(t + dt, it2->t2);
                if (start_s <= end_s + EPS) {
                    double eq1_A = vx - it2->vx;
                    double eq1_B = (it2->x1 - x) - it2->t1 * it2->vx + t * vx;
                    double eq2_A = vy - it2->vy;
                    double eq2_B = (it2->y1 - y) - it2->t1 * it2->vy + t * vy;

                    double s = -1.0;
                    bool valid = false;
                    if (abs(eq1_A) > EPS) {
                        s = eq1_B / eq1_A;
                        if (s >= start_s - EPS && s <= end_s + EPS && abs(eq2_A * s - eq2_B) < EPS) {
                            valid = true;
                        }
                    } else if (abs(eq1_B) < EPS) {
                        if (abs(eq2_A) > EPS) {
                            s = eq2_B / eq2_A;
                            if (s >= start_s - EPS && s <= end_s + EPS) valid = true;
                        } else if (abs(eq2_B) < EPS) {
                            s = start_s;
                            valid = true;
                        }
                    }
                    if (valid) {
                        ans = min(ans, s);
                    }
                }
            }

            // Time to hit shore
            double dt_shore_x = 1e9, dt_shore_y = 1e9;
            if (vx > 0) dt_shore_x = (M + 0.5 - x) / vx;
            else if (vx < 0) dt_shore_x = (x - 0.5) / (-vx);

            if (vy > 0) dt_shore_y = (N + 0.5 - y) / vy;
            else if (vy < 0) dt_shore_y = (y - 0.5) / (-vy);

            double actual_dt_shore = min(dt_shore_x, dt_shore_y);

            if (actual_dt_shore <= dt + EPS) {
                ans = min(ans, t + actual_dt_shore);
            } else {
                if (next_x >= 1 && next_x <= M && next_y >= 1 && next_y <= N) {
                    if (t + dt < dist[next_x][next_y] - EPS) {
                        dist[next_x][next_y] = t + dt;
                        pq.push({t + dt, next_x, next_y});
                    }
                }
            }
        }
    }

    if (ans >= INF - 1e5) {
        cout << "SOS\n";
    } else {
        cout << fixed << setprecision(2) << ans + 1e-9 << "\n";
    }

    return 0;
}
