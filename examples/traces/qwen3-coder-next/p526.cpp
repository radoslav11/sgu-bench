#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <tuple>
#include <map>
#include <cctype>
using namespace std;

const double EPS = 1e-9;

vector<vector<double>> merge_intervals(vector<vector<double>>& intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end(), [](const vector<double>& a, const vector<double>& b) {
        return a[0] < b[0];
    });
    vector<vector<double>> merged;
    for (auto& interval : intervals) {
        if (merged.empty() || interval[0] > merged.back()[1] + EPS) {
            merged.push_back(interval);
        } else {
            merged.back()[1] = max(merged.back()[1], interval[1]);
        }
    }
    return merged;
}

vector<vector<double>> build_safe_set_after_removal(vector<pair<int, int>>& unsafe_list, int v, double t_i) {
    double bound = (double)v * t_i;
    vector<pair<double, double>> unsafe_clipped;
    for (auto p : unsafe_list) {
        int x1 = p.first;
        int x2 = p.second;
        double u1 = (double)x1, u2 = (double)x2;
        double u1_c = max(u1, -bound);
        double u2_c = min(u2, bound);
        if (u1_c < u2_c - EPS) {
            unsafe_clipped.push_back({u1_c, u2_c});
        }
    }
    if (unsafe_clipped.empty()) {
        return {{-bound, bound}};
    }
    sort(unsafe_clipped.begin(), unsafe_clipped.end());
    vector<pair<double, double>> merged_unsafe;
    for (auto seg : unsafe_clipped) {
        if (merged_unsafe.empty()) {
            merged_unsafe.push_back(seg);
        } else {
            auto& last = merged_unsafe.back();
            if (seg.first <= last.second + EPS) {
                last.second = max(last.second, seg.second);
            } else {
                merged_unsafe.push_back(seg);
            }
        }
    }
    vector<vector<double>> res;
    if (-bound <= merged_unsafe[0].first + EPS) {
        res.push_back({-bound, merged_unsafe[0].first});
    }
    for (int j = 0; j < (int)merged_unsafe.size() - 1; j++) {
        if (merged_unsafe[j].second <= merged_unsafe[j+1].first + EPS) {
            res.push_back({merged_unsafe[j].second, merged_unsafe[j+1].first});
        }
    }
    if (merged_unsafe.back().second <= bound + EPS) {
        res.push_back({merged_unsafe.back().second, bound});
    }
    return res;
}

vector<vector<double>> interval_intersection(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    vector<vector<double>> res;
    int i = 0, j = 0;
    while (i < (int)A.size() && j < (int)B.size()) {
        double a1 = A[i][0], a2 = A[i][1];
        double b1 = B[j][0], b2 = B[j][1];
        double l = max(a1, b1);
        double r = min(a2, b2);
        if (l <= r + EPS) {
            res.push_back({l, r});
            if (a2 < b2 - EPS) {
                i++;
            } else if (b2 < a2 - EPS) {
                j++;
            } else {
                i++;
                j++;
            }
        } else {
            if (a2 < b1 - EPS) {
                i++;
            } else {
                j++;
            }
        }
    }
    return res;
}

bool check(double T, int v, int G, vector<tuple<int, int, int>>& stones) {
    vector<tuple<int, int, int>> valid_stones;
    for (auto stone : stones) {
        int x1 = get<0>(stone);
        int x2 = get<1>(stone);
        int t_i = get<2>(stone);
        if (t_i <= T + EPS) {
            valid_stones.push_back(stone);
        }
    }
    if (valid_stones.empty()) {
        return (abs(G) <= (double)v * T + EPS);
    }

    sort(valid_stones.begin(), valid_stones.end(), [](const auto& a, const auto& b) {
        return get<2>(a) < get<2>(b);
    });

    vector<vector<double>> safe_set;
    safe_set.push_back({0.0, 0.0});

    double current_time = 0.0;
    int idx = 0;
    int n_valid = valid_stones.size();
    while (idx < n_valid) {
        double t_i = get<2>(valid_stones[idx]);
        double dt = t_i - current_time;

        vector<vector<double>> propagated;
        for (auto interval : safe_set) {
            double a = interval[0];
            double b = interval[1];
            propagated.push_back({a - (double)v * dt, b + (double)v * dt});
        }

        vector<vector<double>> merged_propagated = merge_intervals(propagated);

        vector<pair<int, int>> unsafe_list;
        while (idx < n_valid && get<2>(valid_stones[idx]) == t_i) {
            int x1 = get<0>(valid_stones[idx]);
            int x2 = get<1>(valid_stones[idx]);
            unsafe_list.push_back({x1, x2});
            idx++;
        }

        vector<vector<double>> safe_set_after_removal = build_safe_set_after_removal(unsafe_list, v, t_i);

        if (safe_set_after_removal.empty()) {
            return false;
        }

        vector<vector<double>> new_safe = interval_intersection(merged_propagated, safe_set_after_removal);
        if (new_safe.empty()) {
            return false;
        }

        safe_set = new_safe;
        current_time = t_i;
    }

    double last_t = current_time;
    double min_dist = 1e30;
    for (auto& interval : safe_set) {
        double a = interval[0];
        double b = interval[1];
        if (G >= a - EPS && G <= b + EPS) {
            min_dist = 0.0;
            break;
        } else if (G < a) {
            min_dist = min(min_dist, a - G);
        } else {
            min_dist = min(min_dist, G - b);
        }
    }

    double remaining_time = T - last_t;
    if (min_dist <= (double)v * remaining_time + EPS) {
        return true;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int v, G, n;
    cin >> v >> G >> n;

    vector<tuple<int, int, int>> stones;
    for (int i = 0; i < n; i++) {
        int x1, x2, t_i;
        cin >> x1 >> x2 >> t_i;
        stones.push_back(make_tuple(x1, x2, t_i));
    }

    double low = (double)abs(G) / v;
    double high = 1000000.0;

    if (!check(high, v, G, stones)) {
        cout << -1 << endl;
        return 0;
    }

    for (int iter = 0; iter < 100; iter++) {
        double mid = (low + high) / 2.0;
        if (check(mid, v, G, stones)) {
            high = mid;
        } else {
            low = mid;
        }
    }

    double T_min = high;

    vector<tuple<int, int, int>> valid_stones;
    for (auto stone : stones) {
        int x1 = get<0>(stone);
        int x2 = get<1>(stone);
        int t_i = get<2>(stone);
        if (t_i <= T_min + EPS) {
            valid_stones.push_back(stone);
        }
    }

    vector<pair<double, vector<vector<double>>>> events_with_safe_set;
    if (!valid_stones.empty()) {
        sort(valid_stones.begin(), valid_stones.end(), [](const auto& a, const auto& b) {
            return get<2>(a) < get<2>(b);
        });

        vector<vector<double>> safe_set;
        safe_set.push_back({0.0, 0.0});

        double current_time = 0.0;
        int idx = 0;
        int n_valid = valid_stones.size();
        while (idx < n_valid) {
            double t_i = get<2>(valid_stones[idx]);
            double dt = t_i - current_time;

            vector<vector<double>> propagated;
            for (auto interval : safe_set) {
                double a = interval[0];
                double b = interval[1];
                propagated.push_back({a - (double)v * dt, b + (double)v * dt});
            }

            vector<vector<double>> merged_propagated = merge_intervals(propagated);

            vector<pair<int, int>> unsafe_list;
            while (idx < n_valid && get<2>(valid_stones[idx]) == t_i) {
                int x1 = get<0>(valid_stones[idx]);
                int x2 = get<1>(valid_stones[idx]);
                unsafe_list.push_back({x1, x2});
                idx++;
            }

            vector<vector<double>> safe_set_after_removal = build_safe_set_after_removal(unsafe_list, v, t_i);
            if (safe_set_after_removal.empty()) {
                break;
            }

            vector<vector<double>> new_safe = interval_intersection(merged_propagated, safe_set_after_removal);
            if (new_safe.empty()) {
                break;
            }

            safe_set = new_safe;
            events_with_safe_set.push_back({t_i, safe_set});
            current_time = t_i;
        }
    }

    vector<pair<double, double>> instructions;
    map<double, double> positions;

    if (events_with_safe_set.empty()) {
        positions[0.0] = 0.0;
        positions[T_min] = (double)G;
    } else {
        positions[T_min] = (double)G;

        for (int j = events_with_safe_set.size() - 1; j >= 0; j--) {
            double t_i = events_with_safe_set[j].first;
            vector<vector<double>> safe_set_j = events_with_safe_set[j].second;
            double next_time;
            double next_pos;

            if (j == (int)events_with_safe_set.size() - 1) {
                next_time = T_min;
                next_pos = (double)G;
            } else {
                next_time = events_with_safe_set[j+1].first;
                next_pos = positions[next_time];
            }

            double dt = next_time - t_i;
            double candidate = 1e30;
            double min_diff = 1e30;

            for (auto& interval : safe_set_j) {
                double a = interval[0];
                double b = interval[1];
                if (next_pos >= a - EPS && next_pos <= b + EPS) {
                    candidate = next_pos;
                    min_diff = 0;
                    break;
                } else if (next_pos < a) {
                    double diff = a - next_pos;
                    if (diff <= (double)v * dt + EPS && diff < min_diff) {
                        min_diff = diff;
                        candidate = a;
                    }
                } else {
                    double diff = next_pos - b;
                    if (diff <= (double)v * dt + EPS && diff < min_diff) {
                        min_diff = diff;
                        candidate = b;
                    }
                }
            }
            if (min_diff > (double)v * dt + EPS) {
                candidate = next_pos;
                for (auto& interval : safe_set_j) {
                    double a = interval[0];
                    double b = interval[1];
                    if (a > next_pos + EPS) {
                        double diff = a - next_pos;
                        if (diff < min_diff) {
                            min_diff = diff;
                            candidate = a;
                        }
                    } else if (b < next_pos - EPS) {
                        double diff = next_pos - b;
                        if (diff < min_diff) {
                            min_diff = diff;
                            candidate = b;
                        }
                    }
                }
            }

            positions[t_i] = candidate;
        }
        positions[0.0] = 0.0;
    }

    vector<double> times;
    for (auto& p : positions) {
        times.push_back(p.first);
    }
    sort(times.begin(), times.end());

    for (int i = 0; i < (int)times.size() - 1; i++) {
        double t1 = times[i];
        double t2 = times[i+1];
        double pos1 = positions[t1];
        double pos2 = positions[t2];
        double dt = t2 - t1;
        double speed = (pos2 - pos1) / dt;
        instructions.push_back({speed, dt});
    }

    cout << instructions.size() << endl;
    cout << fixed << setprecision(15);
    for (auto& instr : instructions) {
        cout << instr.first << " " << instr.second << endl;
    }

    return 0;
}
