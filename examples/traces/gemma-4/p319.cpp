#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

/**
 * Problem: 319. Kalevich Strikes Back
 * Goal: Find the areas of all connected white facets in a canvas containing non-intersecting rectangular frames.
 * Approach: 
 * 1. Model the problem using a containment tree. The canvas is the root (index 0). 
 *    Each frame is a node, and its parent is the smallest frame containing it.
 * 2. Use a sweep-line algorithm on the x-axis to build this containment tree in O(N log N).
 * 3. A frame i's white area is (area of its interior) - (sum of areas of its children).
 *    The canvas's white area is (canvas area) - (sum of areas of its children).
 * 4. All such white areas are the facets. Sort and output them.
 */

struct Frame {
    int x1, x2, y1, y2;
    long long s;
};

struct Event {
    int x;
    int type; // 0 for LEFT, 1 for RIGHT
    int idx;

    bool operator<(const Event& other) const {
        if (x != other.x) return x < other.x;
        return type < other.type;
    }
};

struct Edge {
    int y;
    int type; // 0 for BOTTOM, 1 for TOP
    int idx;

    bool operator<(const Edge& other) const {
        if (y != other.y) return y < other.y;
        if (type != other.type) return type < other.type;
        return idx < other.idx;
    }
};

const int BOTTOM = 0;
const int TOP = 1;
const int LEFT = 0;
const int RIGHT = 1;

int main() {
    // Optimization for faster I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    int W, H;
    cin >> W >> H;

    vector<Frame> frames(N + 1);
    // Frame 0 is the canvas
    frames[0] = {0, W, 0, H, (long long)W * H};

    for (int i = 1; i <= N; ++i) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        frames[i].x1 = min(x1, x2);
        frames[i].x2 = max(x1, x2);
        frames[i].y1 = min(y1, y2);
        frames[i].y2 = max(y1, y2);
        frames[i].s = (long long)(frames[i].x2 - frames[i].x1) * (frames[i].y2 - frames[i].y1);
    }

    vector<Event> events;
    events.reserve(2 * N + 2);
    events.push_back({0, LEFT, 0});
    events.push_back({W, RIGHT, 0});
    for (int i = 1; i <= N; ++i) {
        events.push_back({frames[i].x1, LEFT, i});
        events.push_back({frames[i].x2, RIGHT, i});
    }
    sort(events.begin(), events.end());

    vector<int> parent(N + 1, -1);
    set<Edge> active_edges;

    for (const auto& e : events) {
        if (e.type == LEFT) {
            int idx = e.idx;
            if (idx == 0) {
                // Canvas is the root, its parent is 0 (self-assigned for logic)
                parent[0] = 0;
            } else {
                // Find the smallest y-interval containing this frame's y-range
                auto it = active_edges.lower_bound({frames[idx].y1, BOTTOM, 0});
                if (it == active_edges.end()) {
                    parent[idx] = 0;
                } else {
                    if (it->type == TOP) {
                        // Frame is inside the frame containing this edge
                        parent[idx] = it->idx;
                    } else {
                        // Frame is below the frame containing this edge, shares same parent
                        parent[idx] = parent[it->idx];
                    }
                }
            }
            active_edges.insert({frames[idx].y1, BOTTOM, idx});
            active_edges.insert({frames[idx].y2, TOP, idx});
        } else {
            int idx = e.idx;
            active_edges.erase({frames[idx].y1, BOTTOM, idx});
            active_edges.erase({frames[idx].y2, TOP, idx});
        }
    }

    // Build adjacency list to calculate white areas
    vector<vector<int>> children(N + 1);
    for (int i = 1; i <= N; ++i) {
        children[parent[i]].push_back(i);
    }

    vector<long long> areas;
    areas.reserve(N + 1);
    for (int i = 0; i <= N; ++i) {
        long long sum_s_children = 0;
        for (int child : children[i]) {
            sum_s_children += frames[child].s;
        }
        areas.push_back(frames[i].s - sum_s_children);
    }

    sort(areas.begin(), areas.end());

    for (int i = 0; i < (int)areas.size(); ++i) {
        cout << areas[i] << (i == (int)areas.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

