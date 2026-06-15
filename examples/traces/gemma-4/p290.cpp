#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * Problem: Defend the Milky Way
 * Goal: Find all star systems that lie on the boundary of the 3D convex hull of a set of star systems.
 * Constraints: n <= 100. Coordinates up to 10000. Time limit 0.25s.
 * Approach: For n <= 100, an O(n^4) algorithm to find points on the boundary of the convex hull is sufficient.
 * A point is on the boundary if it lies on at least one plane such that all other points are on one side of it.
 */

struct Point {
    long long x, y, z;
    string name;
};

struct Vec {
    long long x, y, z;
};

Vec operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec cross(const Vec& a, const Vec& b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

long long dot(const Vec& a, const Vec& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int main() {
    // Standard I/O optimization for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    string dummy;
    getline(cin, dummy); // consume the rest of the line after reading n

    vector<Point> P(n);
    for (int i = 0; i < n; ++i) {
        string line;
        if (!getline(cin, line) || line.empty()) {
            // Handle potential empty lines
            if (line.empty() && i < n) {
                i--;
                continue;
            }
            break;
        }

        // According to the problem, the last three space-separated integers are x, y, z.
        // The star name can contain spaces and is everything before the third-to-last space.
        size_t last_space = line.find_last_of(' ');
        if (last_space == string::npos) continue;
        string z_str = line.substr(last_space + 1);
        
        size_t second_last_space = line.find_last_of(' ', last_space - 1);
        if (second_last_space == string::npos) continue;
        string y_str = line.substr(second_last_space + 1, last_space - second_last_space - 1);
        
        size_t third_last_space = line.find_last_of(' ', second_last_space - 1);
        if (third_last_space == string::npos) continue;
        string x_str = line.substr(third_last_space + 1, second_last_space - third_last_space - 1);
        
        string name = line.substr(0, third_last_space);
        
        P[i].name = name;
        try {
            P[i].x = stoll(x_str);
            P[i].y = stoll(y_str);
            P[i].z = stoll(z_str);
        } catch (...) {
            // In case of unexpected parsing issues
        }
    }

    if (n == 0) {
        cout << 0 << "\n";
        return 0;
    }

    vector<bool> on_boundary(n, false);
    bool all_collinear = true;
    if (n >= 3) {
        for (int i = 2; i < n; ++i) {
            Vec cp = cross(P[1] - P[0], P[i] - P[0]);
            if (cp.x != 0 || cp.y != 0 || cp.z != 0) {
                all_collinear = false;
                break;
            }
        }
    }

    // If all points are collinear, they all lie on the boundary of the convex hull.
    if (all_collinear) {
        for (int i = 0; i < n; ++i) on_boundary[i] = true;
    } else {
        // General case: find all faces of the 3D convex hull.
        // A plane formed by triple (i, j, k) is a boundary plane if all other points lie on one side.
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    Vec n_vec = cross(P[j] - P[i], P[k] - P[i]);
                    if (n_vec.x == 0 && n_vec.y == 0 && n_vec.z == 0) continue;

                    int side = 0; // 0: no points checked yet, 1: positive side, -1: negative side, 2: both sides
                    for (int m = 0; m < n; ++m) {
                        long long d = dot(n_vec, P[m] - P[i]);
                        if (d > 0) {
                            if (side == -1) { side = 2; break; }
                            side = 1;
                        } else if (d < 0) {
                            if (side == 1) { side = 2; break; }
                            side = -1;
                        }
                    }
                    
                    // If all points were on one side or on the plane, this is a face or part of a face.
                    if (side != 2) {
                        for (int m = 0; m < n; ++m) {
                            if (dot(n_vec, P[m] - P[i]) == 0) {
                                on_boundary[m] = true;
                            }
                        }
                    }
                }
            }
        }
    }

    // Collect names of stars on the boundary.
    vector<string> res;
    for (int i = 0; i < n; ++i) {
        if (on_boundary[i]) res.push_back(P[i].name);
    }
    sort(res.begin(), res.end());

    // Output the count and names sorted alphabetically.
    cout << res.size() << "\n";
    for (const string& name : res) {
        cout << name << "\n";
    }

    return 0;
}

