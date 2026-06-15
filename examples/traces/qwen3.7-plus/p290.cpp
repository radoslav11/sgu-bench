#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    long long x, y, z;
    string name;
};

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z, ""};
}

long long dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point cross(const Point& a, const Point& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        ""
    };
}

struct Point2D {
    long long x, y;
    int id;
};

long long cross2D(const Point2D& a, const Point2D& b, const Point2D& c) {
    long long x1 = b.x - a.x;
    long long y1 = b.y - a.y;
    long long x2 = c.x - a.x;
    long long y2 = c.y - a.y;
    return x1 * y2 - x2 * y1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    
    string line;
    getline(cin, line);
    
    vector<Point> P;
    for (int i = 0; i < n; ) {
        getline(cin, line);
        if (line.empty()) continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        while (!line.empty() && line.back() == ' ') line.pop_back();
        if (line.empty()) continue;
        
        size_t pos1 = line.rfind(' ');
        if (pos1 == string::npos) continue;
        long long z = stoll(line.substr(pos1 + 1));
        string rem = line.substr(0, pos1);
        
        size_t pos2 = rem.rfind(' ');
        if (pos2 == string::npos) continue;
        long long y = stoll(rem.substr(pos2 + 1));
        string rem2 = rem.substr(0, pos2);
        
        size_t pos3 = rem2.rfind(' ');
        if (pos3 == string::npos) continue;
        long long x = stoll(rem2.substr(pos3 + 1));
        string name = rem2.substr(0, pos3);
        
        P.push_back({x, y, z, name});
        i++;
    }
    
    vector<bool> on_boundary(n, false);
    
    int idx0 = 0, idx1 = -1, idx2 = -1;
    for (int i = 1; i < n; ++i) {
        if (P[i].x != P[0].x || P[i].y != P[0].y || P[i].z != P[0].z) {
            idx1 = i;
            break;
        }
    }
    
    if (idx1 == -1) {
        for (int i = 0; i < n; ++i) on_boundary[i] = true;
    } else {
        Point N;
        for (int i = idx1 + 1; i < n; ++i) {
            Point N_temp = cross(P[idx1] - P[idx0], P[i] - P[idx0]);
            if (N_temp.x != 0 || N_temp.y != 0 || N_temp.z != 0) {
                idx2 = i;
                N = N_temp;
                break;
            }
        }
        
        if (idx2 == -1) {
            Point D = P[idx1] - P[idx0];
            long long min_dot = 4000000000000000000LL, max_dot = -4000000000000000000LL;
            for (int i = 0; i < n; ++i) {
                long long d = dot(P[i] - P[idx0], D);
                if (d < min_dot) min_dot = d;
                if (d > max_dot) max_dot = d;
            }
            for (int i = 0; i < n; ++i) {
                long long d = dot(P[i] - P[idx0], D);
                if (d == min_dot || d == max_dot) {
                    on_boundary[i] = true;
                }
            }
        } else {
            bool all_coplanar = true;
            for (int i = 0; i < n; ++i) {
                if (dot(P[i] - P[idx0], N) != 0) {
                    all_coplanar = false;
                    break;
                }
            }
            
            if (all_coplanar) {
                int drop = 2;
                if (abs(N.x) >= abs(N.y) && abs(N.x) >= abs(N.z)) drop = 0;
                else if (abs(N.y) >= abs(N.x) && abs(N.y) >= abs(N.z)) drop = 1;
                
                vector<Point2D> pts2D;
                for (int i = 0; i < n; ++i) {
                    Point2D p2;
                    p2.id = i;
                    if (drop == 0) { p2.y = P[i].y; p2.x = P[i].z; }
                    else if (drop == 1) { p2.y = P[i].z; p2.x = P[i].x; }
                    else { p2.y = P[i].y; p2.x = P[i].x; }
                    pts2D.push_back(p2);
                }
                
                for (int i = 0; i < n; ++i) {
                    for (int j = i + 1; j < n; ++j) {
                        if (pts2D[i].x == pts2D[j].x && pts2D[i].y == pts2D[j].y) continue;
                        bool all_left = true, all_right = true;
                        for (int k = 0; k < n; ++k) {
                            long long c = cross2D(pts2D[i], pts2D[j], pts2D[k]);
                            if (c > 0) all_right = false;
                            if (c < 0) all_left = false;
                        }
                        if (all_left || all_right) {
                            on_boundary[pts2D[i].id] = true;
                            on_boundary[pts2D[j].id] = true;
                        }
                    }
                }
            } else {
                for (int i = 0; i < n; ++i) {
                    for (int j = i + 1; j < n; ++j) {
                        for (int k = j + 1; k < n; ++k) {
                            Point diff1 = {P[j].x - P[i].x, P[j].y - P[i].y, P[j].z - P[i].z, ""};
                            Point diff2 = {P[k].x - P[i].x, P[k].y - P[i].y, P[k].z - P[i].z, ""};
                            Point N_temp = cross(diff1, diff2);
                            if (N_temp.x == 0 && N_temp.y == 0 && N_temp.z == 0) continue;
                            
                            bool all_front = true, all_back = true;
                            for (int m = 0; m < n; ++m) {
                                long long dx = P[m].x - P[i].x;
                                long long dy = P[m].y - P[i].y;
                                long long dz = P[m].z - P[i].z;
                                long long d = dx * N_temp.x + dy * N_temp.y + dz * N_temp.z;
                                if (d > 0) all_back = false;
                                else if (d < 0) all_front = false;
                                
                                if (!all_front && !all_back) break;
                            }
                            if (all_front || all_back) {
                                for (int m = 0; m < n; ++m) {
                                    long long dx = P[m].x - P[i].x;
                                    long long dy = P[m].y - P[i].y;
                                    long long dz = P[m].z - P[i].z;
                                    if (dx * N_temp.x + dy * N_temp.y + dz * N_temp.z == 0) {
                                        on_boundary[m] = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    vector<string> result;
    for (int i = 0; i < n; ++i) {
        if (on_boundary[i]) {
            result.push_back(P[i].name);
        }
    }
    
    sort(result.begin(), result.end());
    
    cout << result.size() << "\n";
    for (const string& name : result) {
        cout << name << "\n";
    }
    
    return 0;
}
