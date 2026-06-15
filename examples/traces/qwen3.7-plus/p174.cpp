#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

struct Point {
    long long x, y;
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct RawEdge {
    long long x1, y1, x2, y2;
};

struct Edge {
    int u, v;
};

const int MAX_PTS = 400010;
int parent_node[MAX_PTS];
int rank_node[MAX_PTS];

int find_set(int v) {
    if (v == parent_node[v])
        return v;
    return parent_node[v] = find_set(parent_node[v]);
}

bool union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank_node[a] < rank_node[b])
            swap(a, b);
        parent_node[b] = a;
        if (rank_node[a] == rank_node[b])
            rank_node[a]++;
        return true;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int M;
    if (!(cin >> M)) return 0;
    
    vector<RawEdge> raw_edges(M);
    vector<Point> pts;
    pts.reserve(2 * M);
    
    for (int i = 0; i < M; ++i) {
        cin >> raw_edges[i].x1 >> raw_edges[i].y1 >> raw_edges[i].x2 >> raw_edges[i].y2;
        pts.push_back({raw_edges[i].x1, raw_edges[i].y1});
        pts.push_back({raw_edges[i].x2, raw_edges[i].y2});
    }
    
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    
    vector<Edge> edges(M);
    for (int i = 0; i < M; ++i) {
        Point p1 = {raw_edges[i].x1, raw_edges[i].y1};
        Point p2 = {raw_edges[i].x2, raw_edges[i].y2};
        edges[i].u = lower_bound(pts.begin(), pts.end(), p1) - pts.begin();
        edges[i].v = lower_bound(pts.begin(), pts.end(), p2) - pts.begin();
    }
    
    raw_edges.clear();
    raw_edges.shrink_to_fit();
    
    int n = pts.size();
    for (int i = 0; i < n; ++i) {
        parent_node[i] = i;
        rank_node[i] = 0;
    }
    
    for (int i = 0; i < M; ++i) {
        if (!union_sets(edges[i].u, edges[i].v)) {
            cout << i + 1 << "\n";
            return 0;
        }
    }
    
    cout << 0 << "\n";
    return 0;
}
