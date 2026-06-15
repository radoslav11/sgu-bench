#include <iostream>
#include <cstdio>
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;

const double INF = 1e9;
const int dx[5] = {0, 0, 0, 1, -1};
const int dy[5] = {0, -1, 1, 0, 0};

struct State {
    int x, y;
    double dist;
    State(int x, int y, double dist) : x(x), y(y), dist(dist) {}
    bool operator<(const State& other) const {
        return dist > other.dist;
    }
};

int n, m;
vector<string> grid;
vector<vector<double>> dist;

int main() {
    cin >> n >> m;
    grid.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }
    int px, py, bx, by;
    cin >> px >> py;
    cin >> bx >> by;

    px--; py--; bx--; by--;

    if (px < 0) px = 0;
    if (px >= n) px = n-1;
    if (py < 0) py = 0;
    if (py >= m) py = m-1;
    if (bx < 0) bx = 0;
    if (bx >= n) bx = n-1;
    if (by < 0) by = 0;
    if (by >= m) by = m-1;

    dist = vector<vector<double>>(n, vector<double>(m, INF));
    priority_queue<State> pq;
    dist[px][py] = 0;
    pq.push(State(px, py, 0));

    while (!pq.empty()) {
        State state = pq.top(); pq.pop();
        int x = state.x, y = state.y;
        double d = state.dist;
        if (d != dist[x][y]) continue;

        if (x == bx && y == by) {
            cout << fixed << setprecision(2) << d << endl;
            return 0;
        }

        if (x == 0 || x == n-1 || y == 0 || y == m-1) {
            cout << fixed << setprecision(2) << d << endl;
            return 0;
        }

        int flow = grid[x][y] - '0';
        int dfx, dfy;
        if (flow == 0) { dfx = 0; dfy = 0; }
        else if (flow == 1) { dfx = 0; dfy = -1; }
        else if (flow == 2) { dfx = 0; dfy = 1; }
        else if (flow == 3) { dfx = 1; dfy = 0; }
        else { dfx = -1; dfy = 0; }

        for (int mv = 0; mv < 5; mv++) {
            int vx = dx[mv], vy = dy[mv];
            int tx = x + dfx + vx;
            int ty = y + dfy + vy;

            if (tx < 0 || tx >= n || ty < 0 || ty >= m) 
                continue;

            int dx_total = dfx + vx;
            int dy_total = dfy + vy;

            double time = 0;
            if (dx_total == 0 && dy_total == 0) 
                continue;
            if (dx_total == 0) {
                time = 1.0 / abs(dy_total);
            } else if (dy_total == 0) {
                time = 1.0 / abs(dx_total);
            } else {
                time = 1.0 / max(abs(dx_total), abs(dy_total));
            }

            if (dist[tx][ty] > d + time) {
                dist[tx][ty] = d + time;
                pq.push(State(tx, ty, d + time));
            }
        }
    }

    cout << "SOS" << endl;
    return 0;
}
