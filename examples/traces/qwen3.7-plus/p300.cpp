#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Segment {
    int x1, y1, x2, y2;
    int start_t, end_t;
};

struct Conflict {
    int x, y;
    bool operator<(const Conflict& o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    if (N == 1) {
        cout << 0 << "\n";
        return 0;
    }

    vector<Segment> segs(N - 1);
    int L_total = 0;
    for (int i = 0; i < N - 1; ++i) {
        cin >> segs[i].x1 >> segs[i].y1;
        if (i > 0) {
            segs[i].x1 = segs[i-1].x2;
            segs[i].y1 = segs[i-1].y2;
        }
        cin >> segs[i].x2 >> segs[i].y2;
        segs[i].start_t = L_total;
        L_total += abs(segs[i].x2 - segs[i].x1) + abs(segs[i].y2 - segs[i].y1);
        segs[i].end_t = L_total;
    }

    vector<Conflict> conflicts;
    conflicts.reserve(1000000);

    for (int i = 0; i < N - 1; ++i) {
        for (int j = i + 1; j < N - 1; ++j) {
            if (abs(i - j) == 1) continue;
            Segment su = segs[i];
            Segment sv = segs[j];

            if (su.y1 == su.y2 && sv.y1 == sv.y2 && su.y1 == sv.y1) {
                int min_x = max(min(su.x1, su.x2), min(sv.x1, sv.x2));
                int max_x = min(max(su.x1, su.x2), max(sv.x1, sv.x2));
                if (min_x <= max_x) {
                    int tu_min = su.start_t + abs(min_x - su.x1);
                    int tu_max = su.start_t + abs(max_x - su.x1);
                    int tv_min = sv.start_t + abs(min_x - sv.x1);
                    int tv_max = sv.start_t + abs(max_x - sv.x1);
                    
                    int t_u = tu_min;
                    int t_v = tv_min;
                    int end_t_u = tu_max;
                    int step_v = (tv_max > tv_min) ? 1 : -1;
                    
                    while (true) {
                        if (t_u < t_v) conflicts.push_back({t_u, t_v});
                        else if (t_u > t_v) conflicts.push_back({t_v, t_u});
                        
                        if (t_u == end_t_u) break;
                        t_u += (tu_max > tu_min) ? 1 : -1;
                        t_v += step_v;
                    }
                }
            } else if (su.x1 == su.x2 && sv.x1 == sv.x2 && su.x1 == sv.x1) {
                int min_y = max(min(su.y1, su.y2), min(sv.y1, sv.y2));
                int max_y = min(max(su.y1, su.y2), max(sv.y1, sv.y2));
                if (min_y <= max_y) {
                    int tu_min = su.start_t + abs(min_y - su.y1);
                    int tu_max = su.start_t + abs(max_y - su.y1);
                    int tv_min = sv.start_t + abs(min_y - sv.y1);
                    int tv_max = sv.start_t + abs(max_y - sv.y1);
                    
                    int t_u = tu_min;
                    int t_v = tv_min;
                    int end_t_u = tu_max;
                    int step_v = (tv_max > tv_min) ? 1 : -1;
                    
                    while (true) {
                        if (t_u < t_v) conflicts.push_back({t_u, t_v});
                        else if (t_u > t_v) conflicts.push_back({t_v, t_u});
                        
                        if (t_u == end_t_u) break;
                        t_u += (tu_max > tu_min) ? 1 : -1;
                        t_v += step_v;
                    }
                }
            } else {
                int hx, vx;
                if (su.y1 == su.y2) { hx = i; vx = j; }
                else { hx = j; vx = i; }
                
                Segment sh = segs[hx];
                Segment svx = segs[vx];
                
                int intersect_y = sh.y1;
                int intersect_x = svx.x1;
                
                if (intersect_x >= min(sh.x1, sh.x2) && intersect_x <= max(sh.x1, sh.x2) &&
                    intersect_y >= min(svx.y1, svx.y2) && intersect_y <= max(svx.y1, svx.y2)) {
                    
                    int tu = sh.start_t + abs(intersect_x - sh.x1);
                    int tv = svx.start_t + abs(intersect_y - svx.y1);
                    
                    if (tu < tv) conflicts.push_back({tu, tv});
                    else if (tv < tu) conflicts.push_back({tv, tu});
                }
            }
        }
    }

    sort(conflicts.begin(), conflicts.end());
    
    vector<int> suff_min_y(conflicts.size());
    if (!conflicts.empty()) {
        suff_min_y.back() = conflicts.back().y;
        for (int i = (int)conflicts.size() - 2; i >= 0; --i) {
            suff_min_y[i] = min(suff_min_y[i + 1], conflicts[i].y);
        }
    }

    vector<int> events;
    events.reserve(conflicts.size() * 2 + N);
    events.push_back(0);
    events.push_back(L_total);
    for (const auto& c : conflicts) {
        events.push_back(c.x);
        events.push_back(c.y);
    }
    for (int i = 0; i < N - 1; ++i) {
        events.push_back(segs[i].start_t);
        events.push_back(segs[i].end_t);
    }
    
    sort(events.begin(), events.end());
    events.erase(unique(events.begin(), events.end()), events.end());

    int max_len = 0;
    for (int t1 : events) {
        int low = 0, high = (int)conflicts.size() - 1;
        int idx = -1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (conflicts[mid].x >= t1) {
                idx = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        if (idx != -1) {
            int y_min = suff_min_y[idx];
            int x_min = conflicts[idx].x;
            if (x_min - t1 > max_len) max_len = x_min - t1;
            if (t1 == x_min && y_min - t1 > max_len) max_len = y_min - t1;
        } else {
            if (L_total - t1 > max_len) max_len = L_total - t1;
        }
    }

    cout << max_len << "\n";
    return 0;
}
