#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

// Use long double for high precision
typedef long double ld;

struct Point {
    ld x, y;
    ld angle;
    int id;
};

struct Edge {
    int u, v;
    ld dist;
};

struct Road {
    Point p1, p2;
};

const ld EPS = 1e-10;

// Calculate distance between two points
ld get_dist(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Find intersection of two lines containing segments (a,b) and (c,d)
Point intersect(Point a, Point b, Point c, Point d) {
    ld a1 = b.y - a.y;
    ld b1 = a.x - b.x;
    ld c1 = a1 * a.x + b1 * a.y;
    ld a2 = d.y - c.y;
    ld b2 = c.x - d.x;
    ld c2 = a2 * c.x + b2 * c.y;
    ld det = a1 * b2 - a2 * b1;
    if (abs(det) < 1e-11) return {1e18, 1e18, 0, -1};
    return {(b2 * c1 - b1 * c2) / det, (a1 * c2 - a2 * c1) / det, 0, -1};
}

// Check if a point is within the bounding box of a segment
bool on_segment(Point p, Point a, Point b) {
    return p.x >= min(a.x, b.x) - EPS && p.x <= max(a.x, b.x) + EPS &&
           p.y >= min(a.y, b.y) - EPS && p.y <= max(a.y, b.y) + EPS;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<Road> roads(N);
    vector<Point> all_points;
    for (int i = 0; i < N; ++i) {
        cin >> roads[i].p1.x >> roads[i].p1.y >> roads[i].p2.x >> roads[i].p2.y;
        all_points.push_back(roads[i].p1);
        all_points.push_back(roads[i].p2);
    }

    // Collect all intersection points of all road segments
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            Point p = intersect(roads[i].p1, roads[i].p2, roads[j].p1, roads[j].p2);
            if (p.x < 1e17 && on_segment(p, roads[i].p1, roads[i].p2) && on_segment(p, roads[j].p1, roads[j].p2)) {
                all_points.push_back(p);
            }
        }
    }

    // Remove duplicates from the points list
    sort(all_points.begin(), all_points.end(), [](const Point& a, const Point& b) {
        if (abs(a.x - b.x) > 1e-9) return a.x < b.x;
        if (abs(a.y - b.y) > 1e-9) return a.y < b.y;
        return false;
    });
    vector<Point> unique_points;
    for (const auto& p : all_points) {
        if (unique_points.empty() || abs(p.x - unique_points.back().x) > 1e-9 || abs(p.y - unique_points.back().y) > 1e-9) {
            unique_points.push_back(p);
        }
    }

    for (auto& p : unique_points) {
        p.angle = atan2(p.y, p.x);
    }

    vector<Edge> e1, e2;
    for (int i = 0; i < N; ++i) {
        vector<int> on_road;
        for (int j = 0; j < (int)unique_points.size(); ++j) {
            if (on_segment(unique_points[j], roads[i].p1, roads[i].p2)) {
                ld cross = abs((unique_points[j].y - roads[i].p1.y) * (roads[i].p2.x - roads[i].p1.x) - 
                               (unique_points[j].x - roads[i].p1.x) * (roads[i].p2.y - roads[i].p1.y));
                if (cross < 1e-6) on_road.push_back(j);
            }
        }
        sort(on_road.begin(), on_road.end(), [&](int a, int b) {
            ld da = abs(unique_points[a].x - roads[i].p1.x) + abs(unique_points[a].y - roads[i].p1.y);
            ld db = abs(unique_points[b].x - roads[i].p1.x) + abs(unique_points[b].y - roads[i].p1.y);
            return da < db;
        });

        // Create atomic segments from the points on the road
        for (int j = 0; j + 1 < (int)on_road.size(); ++j) {
            int u = on_road[j], v = on_road[j + 1];
            if (abs(unique_points[u].x) < EPS && abs(unique_points[u].y) < EPS) continue;
            if (abs(unique_points[v].x) < EPS && abs(unique_points[v].y) < EPS) continue;
            
            // Check if the origin lies on the segment
            ld cross = abs(unique_points[u].x * unique_points[v].y - unique_points[u].y * unique_points[v].x);
            if (cross < 1e-8 && (unique_points[u].x * unique_points[v].x < 0 || unique_points[u].y * unique_points[v].y < 0)) continue;

            auto process_edge = [&](int a, int b) {
                ld diff = unique_points[b].angle - unique_points[a].angle;
                while (diff <= -M_PI) diff += 2.0 * M_PI;
                while (diff > M_PI) diff -= 2.0 * M_PI;
                if (diff > 1e-11 && diff < M_PI - 1e-11) {
                    if (unique_points[a].angle < unique_points[b].angle)
                        e1.push_back({a, b, get_dist(unique_points[a], unique_points[b])});
                    else
                        e2.push_back({a, b, get_dist(unique_points[a], unique_points[b])});
                }
            };
            process_edge(u, v);
            process_edge(v, u);
        }
    }

    int M = unique_points.size();
    vector<int> sorted_idx(M);
    for (int i = 0; i < M; ++i) sorted_idx[i] = i;
    sort(sorted_idx.begin(), sorted_idx.end(), [&](int a, int b) {
        return unique_points[a].angle < unique_points[b].angle;
    });

    vector<vector<pair<int, ld>>> adj(M);
    for (const auto& e : e1) adj[e.u].push_back({e.v, e.dist});

    ld max_len = 0;
    vector<ld> dp(M);

    // For each potential 'wrap-around' edge (Type 2), find the longest cycle.
    // The Type 2 edge is u -> v, so the path is v -> (Type 1 edges) -> u.
    // We iterate through all possible starting points v.
    for (int v_idx = 0; v_idx < M; ++v_idx) {
        bool is_target = false;
        for (const auto& e : e2) if (e.v == v_idx) { is_target = true; break; }
        if (!is_target) continue;

        fill(dp.begin(), dp.end(), -1e18);
        dp[v_idx] = 0;

        int start_pos = 0;
        for (int i = 0; i < M; ++i) if (sorted_idx[i] == v_idx) { start_pos = i; break; }

        // Standard DP on DAG to find the longest path using only Type 1 edges
        for (int i = start_pos; i < M; ++i) {
            int u = sorted_idx[i];
            if (dp[u] < -1e17) continue;
            for (auto& edge : adj[u]) {
                if (dp[edge.first] < dp[u] + edge.second) {
                    dp[edge.first] = dp[u] + edge.second;
                }
            }
        }

        for (const auto& e : e2) {
            if (e.v == v_idx) {
                if (dp[e.u] > -1e17) {
                    max_len = max(max_len, dp[e.u] + e.dist);
                }
            }
        }
    }

    cout << fixed << setprecision(7) << (double)max_len << endl;

    return 0;
}

