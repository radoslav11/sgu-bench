#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

const double EPS = 1e-7;

struct Point {
    double x, y;
};

struct Segment {
    double start_time, end_time;
    int type; // 0 = STRAIGHT, 1 = ARC
    Point start_pos, end_pos;
    double cx, cy, r, start_angle, end_angle;
};

int n, m;
vector<double> x_coor, l_len;
vector<int> active;
vector<Segment> segments;
double current_time = 0.0;
Point P;

bool is_valid(Point p, const vector<int>& active_set) {
    for (int idx : active_set) {
        double dist2 = (p.x - x_coor[idx]) * (p.x - x_coor[idx]) + p.y * p.y;
        if (dist2 > l_len[idx] * l_len[idx] + 1e-6) return false;
    }
    return true;
}

Point find_lowest_point(const vector<int>& active_set) {
    Point best = {0, 1e18};
    for (int idx : active_set) {
        Point C = {x_coor[idx], -l_len[idx]};
        if (is_valid(C, active_set)) {
            if (C.y < best.y) best = C;
        }
    }
    for (size_t i = 0; i < active_set.size(); ++i) {
        for (size_t j = i + 1; j < active_set.size(); ++j) {
            int idx1 = active_set[i], idx2 = active_set[j];
            double d = abs(x_coor[idx1] - x_coor[idx2]);
            if (d > l_len[idx1] + l_len[idx2] || d < abs(l_len[idx1] - l_len[idx2])) continue;
            double a = (l_len[idx1] * l_len[idx1] - l_len[idx2] * l_len[idx2] + d * d) / (2.0 * d);
            double h2 = l_len[idx1] * l_len[idx1] - a * a;
            if (h2 < -1e-7) continue;
            double h = sqrt(max(0.0, h2));
            double x2 = x_coor[idx1] + a * (x_coor[idx2] - x_coor[idx1]) / d;
            Point C = {x2, -h};
            if (is_valid(C, active_set)) {
                if (C.y < best.y) best = C;
            }
        }
    }
    return best;
}

void move_to(Point P_target) {
    if (abs(P.x - P_target.x) < EPS && abs(P.y - P_target.y) < EPS) return;
    
    double y_block = -1e18;
    for (int idx : active) {
        double yl = -sqrt(max(0.0, l_len[idx] * l_len[idx] - (P.x - x_coor[idx]) * (P.x - x_coor[idx])));
        if (yl > y_block + EPS) {
            y_block = yl;
        }
    }
    
    if (y_block <= P_target.y + EPS) {
        double dist = P.y - P_target.y;
        segments.push_back({current_time, current_time + dist, 0, P, P_target, 0, 0, 0, 0, 0});
        current_time += dist;
        P = P_target;
    } else {
        Point P_block = {P.x, y_block};
        double dist_straight = P.y - P_block.y;
        segments.push_back({current_time, current_time + dist_straight, 0, P, P_block, 0, 0, 0, 0, 0});
        current_time += dist_straight;
        P = P_block;
        
        int best_disk = -1;
        double best_start_angle = 0, best_end_angle = 0;
        
        auto dist_to_bottom = [](double th) {
            while (th > M_PI) th -= 2 * M_PI;
            while (th < -M_PI) th += 2 * M_PI;
            return abs(th - (-M_PI / 2.0));
        };
        
        for (int idx : active) {
            double dist_to_center = hypot(P_block.x - x_coor[idx], P_block.y);
            if (abs(dist_to_center - l_len[idx]) > 1e-5) continue;
            
            double theta1 = atan2(P_block.y, P_block.x - x_coor[idx]);
            double theta2 = atan2(P_target.y, P_target.x - x_coor[idx]);
            
            double angle_diff = theta2 - theta1;
            while (angle_diff > M_PI) angle_diff -= 2 * M_PI;
            while (angle_diff < -M_PI) angle_diff += 2 * M_PI;
            
            double mid_th = theta1 + 0.5 * angle_diff;
            if (dist_to_bottom(mid_th) > dist_to_bottom(theta1) + 1e-5) {
                if (angle_diff > 0) angle_diff -= 2 * M_PI;
                else angle_diff += 2 * M_PI;
            }
            
            bool ok = true;
            for (int s = 1; s < 10; ++s) {
                double t = (double)s / 10.0;
                double th = theta1 + t * angle_diff;
                double px = x_coor[idx] + l_len[idx] * cos(th);
                double py = l_len[idx] * sin(th);
                if (!is_valid({px, py}, active)) {
                    ok = false; break;
                }
            }
            
            if (ok) {
                double end_px = x_coor[idx] + l_len[idx] * cos(theta2);
                double end_py = l_len[idx] * sin(theta2);
                if (hypot(end_px - P_target.x, end_py - P_target.y) < 1e-4) {
                    best_disk = idx;
                    best_start_angle = theta1;
                    best_end_angle = theta1 + angle_diff;
                    break;
                }
            }
        }
        
        if (best_disk != -1) {
            double arc_len = l_len[best_disk] * abs(best_end_angle - best_start_angle);
            segments.push_back({current_time, current_time + arc_len, 1, P, P_target, x_coor[best_disk], 0.0, l_len[best_disk], best_start_angle, best_end_angle});
            current_time += arc_len;
            P = P_target;
        }
    }
}

Point get_pos(double t) {
    for (const auto& seg : segments) {
        if (t >= seg.start_time - EPS && t <= seg.end_time + EPS) {
            double frac = (t - seg.start_time) / (seg.end_time - seg.start_time);
            frac = max(0.0, min(1.0, frac));
            if (seg.type == 0) {
                return {seg.start_pos.x + frac * (seg.end_pos.x - seg.start_pos.x),
                        seg.start_pos.y + frac * (seg.end_pos.y - seg.start_pos.y)};
            } else {
                double angle = seg.start_angle + frac * (seg.end_angle - seg.start_angle);
                return {seg.cx + seg.r * cos(angle),
                        seg.cy + seg.r * sin(angle)};
            }
        }
    }
    return P;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> n >> m)) return 0;
    
    x_coor.resize(n + 1);
    l_len.resize(n + 1);
    
    for (int i = 1; i <= n; ++i) cin >> x_coor[i];
    for (int i = 1; i <= n; ++i) cin >> l_len[i];
    
    vector<int> p(n - 1);
    for (int i = 0; i < n - 1; ++i) cin >> p[i];
    
    vector<double> queries(m);
    for (int i = 0; i < m; ++i) cin >> queries[i];
    
    for (int i = 1; i <= n; ++i) active.push_back(i);
    
    P = find_lowest_point(active);
    current_time = 0.0;
    
    for (int i = 0; i < n - 1; ++i) {
        int cut_rope = p[i];
        active.erase(remove(active.begin(), active.end(), cut_rope), active.end());
        Point P_target = find_lowest_point(active);
        move_to(P_target);
    }
    
    cout << fixed << setprecision(10);
    for (double t : queries) {
        Point ans = get_pos(t);
        cout << ans.x << " " << ans.y << "\n";
    }
    
    return 0;
}
