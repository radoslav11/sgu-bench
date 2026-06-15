#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Point {
    double x, y;
};

double a_arr[5], b_arr[5], c_arr[5];
int n;
double R;

vector<Point> cut_poly(vector<Point> poly, double a, double b, double c, int sign) {
    vector<Point> res;
    if (poly.empty()) return res;
    int m = poly.size();
    for (int i = 0; i < m; ++i) {
        Point u = poly[i];
        Point v = poly[(i + 1) % m];
        double val_u = a * u.x + b * u.y + c;
        double val_v = a * v.x + b * v.y + c;
        bool ok_u = (sign == 1) ? (val_u >= -1e-9) : (val_u <= 1e-9);
        bool ok_v = (sign == 1) ? (val_v >= -1e-9) : (val_v <= 1e-9);
        
        if (ok_u) res.push_back(u);
        
        if ((val_u > 1e-9 && val_v < -1e-9) || (val_u < -1e-9 && val_v > 1e-9)) {
            double t = val_u / (val_u - val_v);
            double x = u.x + t * (v.x - u.x);
            double y = u.y + t * (v.y - u.y);
            res.push_back({x, y});
        }
    }
    return res;
}

double triangle_circle_intersection_area(Point o, Point u, Point v, double R) {
    auto cross_prod = [](Point a, Point b) { return a.x * b.y - a.y * b.x; };
    auto dot_prod = [](Point a, Point b) { return a.x * b.x + a.y * b.y; };
    
    double lu = hypot(u.x, u.y);
    double lv = hypot(v.x, v.y);
    
    if (lu <= R + 1e-9 && lv <= R + 1e-9) {
        return cross_prod(u, v) / 2.0;
    }
    
    double A = u.y - v.y;
    double B = v.x - u.x;
    double C = u.x * v.y - v.x * u.y;
    double d = abs(C) / hypot(A, B);
    
    if (d >= R - 1e-9) {
        double theta = atan2(cross_prod(u, v), dot_prod(u, v));
        return R * R * theta / 2.0;
    }
    
    Point d_vec = {v.x - u.x, v.y - u.y};
    double a_coef = d_vec.x * d_vec.x + d_vec.y * d_vec.y;
    double b_coef = 2 * (u.x * d_vec.x + u.y * d_vec.y);
    double c_coef = u.x * u.x + u.y * u.y - R * R;
    double delta = b_coef * b_coef - 4 * a_coef * c_coef;
    
    if (delta < 0) {
        double theta = atan2(cross_prod(u, v), dot_prod(u, v));
        return R * R * theta / 2.0;
    }
    
    double t1 = (-b_coef - sqrt(delta)) / (2 * a_coef);
    double t2 = (-b_coef + sqrt(delta)) / (2 * a_coef);
    
    vector<double> ts;
    if (t1 >= -1e-9 && t1 <= 1 + 1e-9) ts.push_back(t1);
    if (t2 >= -1e-9 && t2 <= 1 + 1e-9) ts.push_back(t2);
    
    sort(ts.begin(), ts.end());
    
    vector<Point> pts;
    pts.push_back(u);
    for (double t : ts) {
        pts.push_back({u.x + t * d_vec.x, u.y + t * d_vec.y});
    }
    pts.push_back(v);
    
    double area = 0;
    for (size_t i = 0; i + 1 < pts.size(); ++i) {
        Point p1 = pts[i];
        Point p2 = pts[i+1];
        Point mid = {(p1.x + p2.x)/2, (p1.y + p2.y)/2};
        if (hypot(mid.x, mid.y) <= R + 1e-9) {
            area += cross_prod(p1, p2) / 2.0;
        } else {
            double theta = atan2(cross_prod(p1, p2), dot_prod(p1, p2));
            area += R * R * theta / 2.0;
        }
    }
    return area;
}

double circle_poly_intersection_area(vector<Point> poly, double R) {
    double area = 0;
    int m = poly.size();
    for (int i = 0; i < m; ++i) {
        area += triangle_circle_intersection_area({0, 0}, poly[i], poly[(i + 1) % m], R);
    }
    return abs(area);
}

double A_arr[12];
int num_pieces;

double minimax(int mask, double timeC, double timeW, double areaC, double alpha, double beta) {
    if (mask == 0) return areaC;
    
    vector<pair<double, int>> moves;
    for (int i = 0; i < num_pieces; ++i) {
        if ((mask >> i) & 1) {
            moves.push_back({A_arr[i], i});
        }
    }
    sort(moves.rbegin(), moves.rend());
    
    if (timeC <= timeW) {
        double maxEval = -1e18;
        for (auto& move : moves) {
            int i = move.second;
            double p = A_arr[i];
            double eval = minimax(mask ^ (1 << i), timeC + p, timeW, areaC + p, alpha, beta);
            if (eval > maxEval) {
                maxEval = eval;
                if (maxEval > alpha) alpha = maxEval;
            }
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        double minEval = 1e18;
        for (auto& move : moves) {
            int i = move.second;
            double p = A_arr[i];
            double eval = minimax(mask ^ (1 << i), timeC, timeW + p, areaC, alpha, beta);
            if (eval < minEval) {
                minEval = eval;
                if (minEval < beta) beta = minEval;
            }
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    if (!(cin >> n >> R)) return 0;
    for (int i = 0; i < n; ++i) {
        cin >> a_arr[i] >> b_arr[i] >> c_arr[i];
    }
    
    double B = 100000.0;
    vector<Point> initial_poly = { {-B, -B}, {B, -B}, {B, B}, {-B, B} };
    
    vector<double> areas;
    int total_masks = 1 << n;
    for (int mask = 0; mask < total_masks; ++mask) {
        vector<Point> poly = initial_poly;
        for (int k = 0; k < n; ++k) {
            int sign = (mask >> k) & 1;
            poly = cut_poly(poly, a_arr[k], b_arr[k], c_arr[k], sign);
        }
        if (poly.size() >= 3) {
            double area = circle_poly_intersection_area(poly, R);
            if (area > 1e-5) {
                areas.push_back(area);
            }
        }
    }
    
    sort(areas.rbegin(), areas.rend());
    num_pieces = areas.size();
    for (int i = 0; i < num_pieces; ++i) {
        A_arr[i] = areas[i];
    }
    
    double total_area = 0;
    for (double area : areas) total_area += area;
    
    double ansC = minimax((1 << num_pieces) - 1, 0.0, 0.0, 0.0, -1e18, 1e18);
    double ansW = total_area - ansC;
    
    cout << fixed << setprecision(10) << ansC << " " << ansW << "\n";
    return 0;
}
