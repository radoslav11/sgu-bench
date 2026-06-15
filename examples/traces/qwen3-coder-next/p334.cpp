#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> point;

bool is_connected(vector<point> pts) {
    if (pts.empty()) 
        return false;
    int n = pts.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;
    int count = 1;
    while (!q.empty()) {
        int i = q.front(); q.pop();
        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int dr = abs(pts[i].first - pts[j].first);
                int dc = abs(pts[i].second - pts[j].second);
                if ((dr == 1 && dc == 0) || (dr == 0 && dc == 1)) {
                    visited[j] = true;
                    count++;
                    q.push(j);
                }
            }
        }
    }
    return count == n;
}

vector<point> get_translations(vector<point> part) {
    int rmin = INT_MAX, rmax = INT_MIN;
    int cmin = INT_MAX, cmax = INT_MIN;
    for (auto p : part) {
        rmin = min(rmin, p.first);
        rmax = max(rmax, p.first);
        cmin = min(cmin, p.second);
        cmax = max(cmax, p.second);
    }
    vector<point> trans;
    for (int dx = -rmin; dx <= 2 - rmax; dx++) {
        for (int dy = -cmin; dy <= 2 - cmax; dy++) {
            trans.push_back(make_pair(dx, dy));
        }
    }
    return trans;
}

bool dfs_arrange(int i, vector<string>& grid3, 
                const vector<vector<point>>& parts, 
                const vector<vector<point>>& trans_list,
                vector<point>& trans_used) {
    if (i == (int)parts.size()) {
        return true;
    }
    for (auto trans : trans_list[i]) {
        int dx = trans.first;
        int dy = trans.second;
        bool ok = true;
        vector<point> shifted;
        for (auto p : parts[i]) {
            int nr = p.first + dx;
            int nc = p.second + dy;
            if (nr < 0 || nr >= 3 || nc < 0 || nc >= 3) {
                ok = false;
                break;
            }
            shifted.push_back(make_pair(nr, nc));
        }
        if (!ok) 
            continue;
        for (auto p : shifted) {
            if (grid3[p.first][p.second] != '.') {
                ok = false;
                break;
            }
        }
        if (!ok) 
            continue;
        for (auto p : shifted) {
            grid3[p.first][p.second] = 'A' + i;
        }
        trans_used[i] = trans;
        if (dfs_arrange(i + 1, grid3, parts, trans_list, trans_used)) {
            return true;
        }
        for (auto p : shifted) {
            grid3[p.first][p.second] = '.';
        }
    }
    return false;
}

void generate_partitions(vector<point> points, int K, 
                        vector<vector<point>>& current, 
                        vector<vector<vector<point>>>& all) {
    if (K == 1) {
        if (is_connected(points)) {
            current.push_back(points);
            all.push_back(current);
            current.pop_back();
        }
        return;
    }
    int n = points.size();
    for (int mask = 1; mask < (1 << n); mask++) {
        vector<point> subset;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subset.push_back(points[i]);
            }
        }
        if (find(subset.begin(), subset.end(), points[0]) == subset.end()) 
            continue;
        if (!is_connected(subset)) 
            continue;
        vector<point> rest;
        for (auto p : points) {
            if (find(subset.begin(), subset.end(), p) == subset.end()) {
                rest.push_back(p);
            }
        }
        current.push_back(subset);
        generate_partitions(rest, K - 1, current, all);
        current.pop_back();
    }
}

int main() {
    int H, W;
    cin >> H >> W;
    vector<string> grid(H);
    for (int i = 0; i < H; i++) {
        cin >> grid[i];
    }

    vector<point> points;
    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (grid[r][c] == 'X') {
                points.push_back(make_pair(r, c));
            }
        }
    }

    vector<vector<point>> solution_parts;
    vector<string> solution_grid3(3, string(3, '.'));
    bool found = false;

    for (int K = 1; K <= 9; K++) {
        vector<vector<vector<point>>> all_partitions;
        vector<vector<point>> current;
        generate_partitions(points, K, current, all_partitions);

        for (auto& parts : all_partitions) {
            vector<vector<point>> trans_list;
            for (auto& part : parts) {
                trans_list.push_back(get_translations(part));
            }
            vector<string> grid3(3, string(3, '.'));
            vector<point> trans_used(parts.size());
            if (dfs_arrange(0, grid3, parts, trans_list, trans_used)) {
                found = true;
                solution_parts = parts;
                solution_grid3 = grid3;
                goto output;
            }
        }
    }

output:
    cout << solution_parts.size() << endl;
    vector<string> out_grid = grid;
    for (int i = 0; i < (int)solution_parts.size(); i++) {
        for (auto p : solution_parts[i]) {
            out_grid[p.first][p.second] = 'A' + i;
        }
    }
    for (int i = 0; i < H; i++) {
        cout << out_grid[i] << endl;
    }
    cout << endl;
    for (int i = 0; i < 3; i++) {
        cout << solution_grid3[i] << endl;
    }

    return 0;
}
