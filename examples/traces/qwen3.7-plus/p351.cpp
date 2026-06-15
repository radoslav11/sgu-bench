#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

const double PI = acos(-1.0);

struct Point {
    double x, y;
    Point operator+(const Point& o) const { return {x + o.x, y + o.y}; }
    Point operator-(const Point& o) const { return {x - o.x, y - o.y}; }
    Point operator*(double k) const { return {x * k, y * k}; }
};

double dist(Point a, Point b) {
    return hypot(a.x - b.x, a.y - b.y);
}

double dist2(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

struct Path {
    double length;
    vector<pair<char, pair<Point, Point>>> parts;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    Point A, B, C;
    double R, D;
    if (!(cin >> A.x >> A.y >> B.x >> B.y)) return 0;
    cin >> C.x >> C.y >> R;
    cin >> D;
    
    vector<Path> candidates;
    
    // Candidate 0: Straight line A -> B
    double a = dist2(A, B);
    bool candidate0_valid = false;
    double L0 = 1e18;
    
    if (a > 1e-9) {
        double b = 2.0 * ((A.x - C.x) * (B.x - A.x) + (A.y - C.y) * (B.y - A.y));
        double c = dist2(A, C) - R * R;
        double delta = b * b - 4.0 * a * c;
        
        if (delta < 0) {
            candidate0_valid = true;
            L0 = dist(A, B);
        } else {
            double t1 = (-b - sqrt(delta)) / (2.0 * a);
            double t2 = (-b + sqrt(delta)) / (2.0 * a);
            vector<double> valid_t;
            if (t1 >= -1e-9 && t1 <= 1.0 + 1e-9) valid_t.push_back(t1);
            if (t2 >= -1e-9 && t2 <= 1.0 + 1e-9) valid_t.push_back(t2);
            
            if (valid_t.size() < 2) {
                candidate0_valid = true;
                L0 = dist(A, B);
            } else {
                double intersect_len = abs(t2 - t1) * dist(A, B);
                if (intersect_len <= D + 1e-7) {
                    candidate0_valid = true;
                    L0 = dist(A, B);
                }
            }
        }
    }
    
    if (candidate0_valid) {
        Path p;
        p.length = L0;
        p.parts.push_back({'S', {A, B}});
        candidates.push_back(p);
    }
    
    double dist_A = dist(A, C);
    double dist_B = dist(B, C);
    
    double alpha_A = atan2(A.y - C.y, A.x - C.x);
    if (alpha_A < 0) alpha_A += 2 * PI;
    
    double alpha_B = atan2(B.y - C.y, B.x - C.x);
    if (alpha_B < 0) alpha_B += 2 * PI;
    
    vector<Point> TA_list;
    if (dist_A > R + 1e-9) {
        double angle_offset_A = acos(R / dist_A);
        TA_list.push_back(C + Point(cos(alpha_A - angle_offset_A), sin(alpha_A - angle_offset_A)) * R);
        TA_list.push_back(C + Point(cos(alpha_A + angle_offset_A), sin(alpha_A + angle_offset_A)) * R);
    } else {
        TA_list.push_back(A);
        TA_list.push_back(A);
    }
    
    vector<Point> TB_list;
    if (dist_B > R + 1e-9) {
        double angle_offset_B = acos(R / dist_B);
        TB_list.push_back(C + Point(cos(alpha_B - angle_offset_B), sin(alpha_B - angle_offset_B)) * R);
        TB_list.push_back(C + Point(cos(alpha_B + angle_offset_B), sin(alpha_B + angle_offset_B)) * R);
    } else {
        TB_list.push_back(B);
        TB_list.push_back(B);
    }
    
    auto is_inside = [](double start, double end, double alpha) {
        if (start < end) return alpha > start && alpha < end;
        else return alpha > start || alpha < end;
    };
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            Point TA = TA_list[i];
            Point TB = TB_list[j];
            
            double angle_TA = atan2(TA.y - C.y, TA.x - C.x);
            if (angle_TA < 0) angle_TA += 2 * PI;
            double angle_TB = atan2(TB.y - C.y, TB.x - C.x);
            if (angle_TB < 0) angle_TB += 2 * PI;
            
            bool ccw_valid = !is_inside(angle_TA, angle_TB, alpha_A) && !is_inside(angle_TA, angle_TB, alpha_B);
            bool cw_valid = !is_inside(angle_TB, angle_TA, alpha_A) && !is_inside(angle_TB, angle_TA, alpha_B);
            
            bool use_ccw = ccw_valid; 
            
            double diff_ccw = angle_TB - angle_TA;
            if (diff_ccw < 0) diff_ccw += 2 * PI;
            double diff_cw = angle_TA - angle_TB;
            if (diff_cw < 0) diff_cw += 2 * PI;
            
            double L_arc = R * (use_ccw ? diff_ccw : diff_cw);
            double L_base = dist(A, TA) + dist(TB, B);
            
            double dist_TA_TB = dist(TA, TB);
            Path p;
            
            if (dist_TA_TB <= 1e-7) {
                p.length = L_base;
                p.parts.push_back({'S', {A, TA}});
                p.parts.push_back({'S', {TA, B}});
            } else if (dist_TA_TB <= D + 1e-7) {
                p.length = L_base + dist_TA_TB;
                p.parts.push_back({'S', {A, TA}});
                p.parts.push_back({'S', {TA, TB}});
                p.parts.push_back({'S', {TB, B}});
            } else {
                double val = D / (2.0 * R);
                if (val > 1.0) val = 1.0;
                if (val < -1.0) val = -1.0;
                double delta = 2.0 * asin(val);
                double angle_V;
                if (use_ccw) {
                    angle_V = angle_TA + delta;
                    if (angle_V >= 2 * PI) angle_V -= 2 * PI;
                } else {
                    angle_V = angle_TA - delta;
                    if (angle_V < 0) angle_V += 2 * PI;
                }
                
                Point V = C + Point(cos(angle_V), sin(angle_V)) * R;
                
                double diff_V_TB;
                if (use_ccw) {
                    diff_V_TB = angle_TB - angle_V;
                    if (diff_V_TB < 0) diff_V_TB += 2 * PI;
                } else {
                    diff_V_TB = angle_V - angle_TB;
                    if (diff_V_TB < 0) diff_V_TB += 2 * PI;
                }
                double L_arc_V_TB = R * diff_V_TB;
                
                p.length = L_base + D + L_arc_V_TB;
                p.parts.push_back({'S', {A, TA}});
                p.parts.push_back({'S', {TA, V}});
                p.parts.push_back({'A', {V, TB}});
                p.parts.push_back({'S', {TB, B}});
            }
            candidates.push_back(p);
        }
    }
    
    Path best = candidates[0];
    for (size_t i = 1; i < candidates.size(); ++i) {
        if (candidates[i].length < best.length - 1e-7) {
            best = candidates[i];
        } else if (abs(candidates[i].length - best.length) <= 1e-7) {
            if (candidates[i].parts.size() < best.parts.size()) {
                best = candidates[i];
            }
        }
    }
    
    cout << best.parts.size() << "\n";
    cout << fixed << setprecision(8);
    for (const auto& part : best.parts) {
        cout << part.first << " " 
             << part.second.first.x << " " << part.second.first.y << " "
             << part.second.second.x << " " << part.second.second.y << "\n";
    }
    
    return 0;
}
