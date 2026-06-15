#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct State {
    long long cost;
    long long time;
    int i;
    int x;
    int dir;
    bool operator>(const State& other) const {
        if (cost != other.cost) return cost > other.cost;
        return time > other.time;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    long long s;
    if (!(cin >> N >> s)) return 0;
    
    int x_start, h, a, b;
    cin >> x_start >> h >> a >> b;
    
    int M;
    cin >> M;
    vector<int> L(M), R(M), Y(M);
    vector<int> xs;
    xs.push_back(x_start);
    for (int i = 0; i < M; ++i) {
        cin >> L[i] >> R[i] >> Y[i];
        xs.push_back(L[i]);
        xs.push_back(R[i]);
    }
    
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    
    auto get_x_idx = [&](int x) {
        return lower_bound(xs.begin(), xs.end(), x) - xs.begin();
    };
    
    int num_x = xs.size();
    vector<vector<vector<long long>>> min_cost(M, vector<vector<long long>>(num_x, vector<long long>(2, INF)));
    vector<vector<vector<long long>>> min_time(M, vector<vector<long long>>(num_x, vector<long long>(2, INF)));
    
    int start_i = -1;
    int max_y = -1;
    for (int i = 0; i < M; ++i) {
        if (Y[i] < h && L[i] < x_start && x_start < R[i]) {
            if (Y[i] > max_y) {
                max_y = Y[i];
                start_i = i;
            }
        }
    }
    
    if (start_i == -1) {
        cout << "0 0\n";
        return 0;
    }
    
    priority_queue<State, vector<State>, greater<State>> pq;
    long long initial_time = h - max_y;
    int start_x_idx = get_x_idx(x_start);
    
    min_cost[start_i][start_x_idx][1] = 0;
    min_time[start_i][start_x_idx][1] = initial_time;
    pq.push({0, initial_time, start_i, x_start, 1});
    
    long long best_cost = INF;
    long long best_time = INF;
    
    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();
        
        int cx_idx = get_x_idx(curr.x);
        
        if (curr.cost > min_cost[curr.i][cx_idx][curr.dir]) continue;
        if (curr.cost == min_cost[curr.i][cx_idx][curr.dir] && curr.time > min_time[curr.i][cx_idx][curr.dir]) continue;
        
        if (Y[curr.i] == b) {
            long long reach_time = -1;
            if (curr.dir == 1 && curr.x <= a && a <= R[curr.i]) {
                reach_time = curr.time + (a - curr.x);
            } else if (curr.dir == -1 && L[curr.i] <= a && a <= curr.x) {
                reach_time = curr.time + (curr.x - a);
            }
            
            if (reach_time != -1) {
                if (curr.cost < best_cost) {
                    best_cost = curr.cost;
                    best_time = reach_time;
                } else if (curr.cost == best_cost && reach_time < best_time) {
                    best_time = reach_time;
                }
                continue;
            }
        }
        
        // Option 1: Place a blocker
        long long ncost1 = curr.cost + 1;
        long long ntime1 = curr.time;
        int ndir1 = -curr.dir;
        if (ncost1 < min_cost[curr.i][cx_idx][ndir1] || 
           (ncost1 == min_cost[curr.i][cx_idx][ndir1] && ntime1 < min_time[curr.i][cx_idx][ndir1])) {
            min_cost[curr.i][cx_idx][ndir1] = ncost1;
            min_time[curr.i][cx_idx][ndir1] = ntime1;
            pq.push({ncost1, ntime1, curr.i, curr.x, ndir1});
        }
        
        // Option 2: Walk to end and fall
        int nx = (curr.dir == 1) ? R[curr.i] : L[curr.i];
        long long ntime2 = curr.time + (curr.dir == 1 ? (nx - curr.x) : (curr.x - nx));
        
        int fall_j = -1;
        int fall_max_y = -1;
        for (int j = 0; j < M; ++j) {
            if (Y[j] < Y[curr.i] && L[j] < nx && nx < R[j]) {
                if (Y[j] > fall_max_y) {
                    fall_max_y = Y[j];
                    fall_j = j;
                }
            }
        }
        
        if (fall_j != -1) {
            long long ntime3 = ntime2 + (Y[curr.i] - fall_max_y);
            long long ncost2 = curr.cost;
            int nx_idx = get_x_idx(nx);
            if (ncost2 < min_cost[fall_j][nx_idx][curr.dir] || 
               (ncost2 == min_cost[fall_j][nx_idx][curr.dir] && ntime3 < min_time[fall_j][nx_idx][curr.dir])) {
                min_cost[fall_j][nx_idx][curr.dir] = ncost2;
                min_time[fall_j][nx_idx][curr.dir] = ntime3;
                pq.push({ncost2, ntime3, fall_j, nx, curr.dir});
            }
        }
    }
    
    if (best_cost == INF || (N - best_cost <= 0)) {
        cout << "0 0\n";
    } else {
        long long ans_lemmings = N - best_cost;
        long long ans_time = (long long)(N - 1) * s + best_time;
        cout << ans_lemmings << " " << ans_time << "\n";
    }
    
    return 0;
}
