#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * Problem: 209. Areas
 * Task: Given N lines, find the area of each finite region they divide the plane into.
 * Constraints: N <= 80, coordinates <= 100, time limit 0.25s.
 * 
 * Approach:
 * 1. Represent each line as ax + by + c = 0.
 * 2. Find all unique intersection points of these lines.
 * 3. For each line, identify all intersection points that lie on it and sort them.
 * 4. Construct a planar graph where vertices are intersection points and edges are segments between consecutive points on a line.
 * 5. For each directed edge, traverse its face using a "left-turn" strategy.
 * 6. Calculate the shoelace area of each face. Finite faces will have a positive area (CCW traversal), while the "infinite" faces will have a negative area (CW traversal).
 * 7. Collect and sort all positive areas.
 */

typedef long double ld;

const ld EPS_POINT = 1e-9;
const ld EPS_LINE = 1e-6;
const ld EPS_ANGLE = 1e-11;
const ld EPS_AREA = 1e-8;

struct Point {
    ld x, y;
    bool operator<(const Point& other) const {
        if (abs(x - other.x) > EPS_POINT) return x < other.x;
        if (abs(y - other.y) > EPS_POINT) return y < other.y;
        return false;
    }
    bool operator==(const Point& other) const {
        return abs(x - other.x) < EPS_POINT && abs(y - other.y) < EPS_POINT;
    }
};

struct Line {
    ld a, b, c;
};

struct Edge {
    int to;
    ld angle;
    int id;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<Line> lines(N);
    for (int i = 0; i < N; ++i) {
        ld x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        lines[i].a = y1 - y2;
        lines[i].b = x2 - x1;
        lines[i].c = -lines[i].a * x1 - lines[i].b * y1;
    }

    vector<Point> intersection_points;
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            ld det = lines[i].a * lines[j].b - lines[j].a * lines[i].b;
            if (abs(det) > 1e-12) {
                ld x = (lines[i].b * lines[j].c - lines[j].b * lines[i].c) / det;
                ld y = (lines[i].c * lines[j].a - lines[j].c * lines[i].a) / det;
                intersection_points.push_back({x, y});
            }
        }
    }

    sort(intersection_points.begin(), intersection_points.end());
    intersection_points.erase(unique(intersection_points.begin(), intersection_points.end()), intersection_points.end());

    int M = intersection_points.size();
    vector<vector<Edge>> adj(M);
    int edge_count = 0;

    for (int i = 0; i < N; ++i) {
        vector<pair<ld, int>> points_on_line;
        for (int j = 0; j < M; ++j) {
            ld val = lines[i].a * intersection_points[j].x + lines[i].b * intersection_points[j].y + lines[i].c;
            if (abs(val) < EPS_LINE) {
                ld coord = (abs(lines[i].b) > abs(lines[i].a)) ? intersection_points[j].x : intersection_points[j].y;
                points_on_line.push_back({coord, j});
            }
        }
        sort(points_on_line.begin(), points_on_line.end());
        for (size_t j = 0; j + 1 < points_on_line.size(); ++j) {
            int u = points_on_line[j].second;
            int v = points_on_line[j + 1].second;
            if (u == v) continue;
            
            ld angle_uv = atan2(intersection_points[v].y - intersection_points[u].y, intersection_points[v].x - intersection_points[u].x);
            ld angle_vu = atan2(intersection_points[u].y - intersection_points[v].y, intersection_points[u].x - intersection_points[v].x);
            
            adj[u].push_back({v, angle_uv, edge_count++});
            adj[v].push_back({u, angle_vu, edge_count++});
        }
    }

    for (int i = 0; i < M; ++i) {
        sort(adj[i].begin(), adj[i].end(), [](const Edge& a, const Edge& b) {
            return a.angle < b.angle;
        });
    }

    vector<bool> visited(edge_count, false);
    vector<ld> areas;

    for (int i = 0; i < M; ++i) {
        for (size_t j = 0; j < adj[i].size(); ++j) {
            if (!visited[adj[i][j].id]) {
                ld area = 0;
                int curr_u = i;
                int curr_edge_idx = (int)j;
                
                while (!visited[adj[curr_u][curr_edge_idx].id]) {
                    visited[adj[curr_u][curr_edge_idx].id] = true;
                    int v = adj[curr_u][curr_edge_idx].to;
                    area += (intersection_points[curr_u].x * intersection_points[v].y - intersection_points[v].x * intersection_points[curr_u].y);
                    
                    ld reverse_angle = atan2(intersection_points[curr_u].y - intersection_points[v].y, intersection_points[curr_u].x - intersection_points[v].x);
                    
                    int rev_idx = -1;
                    // Find the edge in adj[v] with angle equal to reverse_angle
                    int low = 0, high = (int)adj[v].size() - 1;
                    while(low <= high) {
                        int mid = low + (high - low) / 2;
                        if(abs(adj[v][mid].angle - reverse_angle) < EPS_ANGLE) {
                            rev_idx = mid;
                            break;
                        } else if(adj[v][mid].angle < reverse_angle) {
                            low = mid + 1;
                        } else {
                            high = mid - 1;
                        }
                    }
                    // In case binary search fails due to precision, linear search is a fallback
                    if(rev_idx == -1) {
                        for(int k=0; k<(int)adj[v].size(); ++k) {
                            if(abs(adj[v][k].angle - reverse_angle) < EPS_ANGLE) {
                                rev_idx = k;
                                break;
                            }
                        }
                    }

                    int next_edge_idx;
                    if (rev_idx == 0) {
                        next_edge_idx = (int)adj[v].size() - 1;
                    } else {
                        next_edge_idx = rev_idx - 1;
                    }
                    
                    curr_u = v;
                    curr_edge_idx = next_edge_idx;
                }
                
                area /= 2.0;
                if (area > EPS_AREA) {
                    areas.push_back(area);
                }
            }
        }
    }

    sort(areas.begin(), areas.end());
    cout << areas.size() << "\n";
    cout << fixed << setprecision(4);
    for (ld a : areas) {
        cout << (double)a << "\n";
    }

    return 0;
}

