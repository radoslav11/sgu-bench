#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Point {
    int x, y;
    Point() {}
    Point(int x, int y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct Segment {
    int x1, y1, x2, y2, v;
    int id;
    Segment() {}
    Segment(int x1, int y1, int x2, int y2, int v, int id) 
        : x1(x1), y1(y1), x2(x2), y2(y2), v(v), id(id) {}
};

// Calculate angle of vector (dx, dy)
double angle(int dx, int dy) {
    return atan2(dy, dx);
}

// Cross product of (p1-p0) and (p2-p0)
ll cross(const Point& p0, const Point& p1, const Point& p2) {
    return (ll)(p1.x - p0.x) * (p2.y - p0.y) - (ll)(p1.y - p0.y) * (p2.x - p0.x);
}

// Check if point p is on segment (a, b)
bool onSegment(const Point& a, const Point& b, const Point& p) {
    if (p.x < min(a.x, b.x) || p.x > max(a.x, b.x)) return false;
    if (p.y < min(a.y, b.y) || p.y > max(a.y, b.y)) return false;
    return cross(a, b, p) == 0;
}

// Ray casting: count intersections with ray to the right
bool pointInPolygon(const Point& p, const vector<Point>& poly) {
    int n = poly.size();
    int crossings = 0;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if ((poly[i].y > p.y) != (poly[j].y > p.y)) {
            // Calculate x-coordinate of intersection
            ll dx = poly[j].x - poly[i].x;
            ll dy = poly[j].y - poly[i].y;
            ll slope = dx * (p.y - poly[i].y) - dy * (p.x - poly[i].x);
            // If point is on edge, consider it inside
            if (onSegment(poly[i], poly[j], p)) return true;
            if (slope > 0 == (poly[j].y > poly[i].y)) {
                crossings++;
            }
        }
    }
    return crossings % 2 == 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    
    vector<Segment> segments;
    vector<Point> points;
    map<Point, int> pointToIdx;
    
    // We'll collect all tokens first
    vector<int> tokens;
    tokens.reserve(5 * N + 4);
    
    for (int i = 0; i < 5 * N + 4; i++) {
        int x;
        cin >> x;
        tokens.push_back(x);
    }
    
    int tokenIdx = 0;
    
    // Read N segments
    for (int segId = 0; segId < N; segId++) {
        int x1 = tokens[tokenIdx++];
        int y1 = tokens[tokenIdx++];
        int x2 = tokens[tokenIdx++];
        int y2 = tokens[tokenIdx++];
        int v = tokens[tokenIdx++];
        segments.push_back(Segment(x1, y1, x2, y2, v, segId));
        
        Point p1(x1, y1), p2(x2, y2);
        if (pointToIdx.find(p1) == pointToIdx.end()) {
            pointToIdx[p1] = points.size();
            points.push_back(p1);
        }
        if (pointToIdx.find(p2) == pointToIdx.end()) {
            pointToIdx[p2] = points.size();
            points.push_back(p2);
        }
    }
    
    // Read headquarters
    int X1 = tokens[tokenIdx++];
    int Y1 = tokens[tokenIdx++];
    int X2 = tokens[tokenIdx++];
    int Y2 = tokens[tokenIdx++];
    Point hq1(X1, Y1), hq2(X2, Y2);
    
    // Build adjacency list: for each vertex, list of (neighbor, seg_id, v)
    int numVertices = points.size();
    vector<vector<tuple<int, int, int>>> adj(numVertices); // to, seg_id, v
    
    for (const auto& seg : segments) {
        int u = pointToIdx[Point(seg.x1, seg.y1)];
        int v = pointToIdx[Point(seg.x2, seg.y2)];
        adj[u].emplace_back(v, seg.id, seg.v);
        adj[v].emplace_back(u, seg.id, seg.v);
    }
    
    // For each vertex, sort incident edges by angle
    vector<vector<int>> edgeOrder(numVertices); // for each vertex, the order of neighbors by angle
    
    for (int i = 0; i < numVertices; i++) {
        vector<pair<double, int>> angleList;
        for (auto [neighbor, seg_id, v] : adj[i]) {
            Point p = points[neighbor];
            double angleVal = angle(p.x - points[i].x, p.y - points[i].y);
            angleList.push_back({angleVal, neighbor});
        }
        // Sort by angle
        sort(angleList.begin(), angleList.end());
        for (int j = 0; j < (int)angleList.size(); j++) {
            edgeOrder[i].push_back(angleList[j].second);
        }
    }
    
    // Create directed edges: for each segment, create two directed edges
    // directedEdge[i] = (from, to, seg_id, rev_index)
    vector<tuple<int, int, int>> directedEdges; // from, to, seg_id
    map<tuple<int, int, int>, int> edgeToIdx;
    
    for (int segId = 0; segId < N; segId++) {
        const auto& seg = segments[segId];
        int u = pointToIdx[Point(seg.x1, seg.y1)];
        int v = pointToIdx[Point(seg.x2, seg.y2)];
        
        // directed edge u->v
        directedEdges.push_back({u, v, segId});
        edgeToIdx[{u, v, segId}] = directedEdges.size() - 1;
        
        // directed edge v->u
        directedEdges.push_back({v, u, segId});
        edgeToIdx[{v, u, segId}] = directedEdges.size() - 1;
    }
    
    int numDirectedEdges = directedEdges.size();
    vector<int> nextEdge(numDirectedEdges, -1);
    vector<int> edgeWeight(numDirectedEdges);
    
    for (int i = 0; i < numDirectedEdges; i++) {
        auto [u, v, segId] = directedEdges[i];
        edgeWeight[i] = segments[segId].v;
        
        // At vertex v, find the next edge after u in clockwise order
        // The edge from u to v means at v, the incoming edge is from u
        // We need the next outgoing edge from v after the edge to u, in clockwise order
        
        // Find position of u in edgeOrder[v]
        int pos = -1;
        for (int j = 0; j < (int)edgeOrder[v].size(); j++) {
            if (edgeOrder[v][j] == u) {
                pos = j;
                break;
            }
        }
        if (pos == -1) continue;
        
        // Next in clockwise order is (pos + 1) % adj[v].size()
        int nextNeighbor = edgeOrder[v][(pos + 1) % adj[v].size()];
        
        // Find directed edge v->nextNeighbor with the same seg_id
        // We need to find which segment connects v and nextNeighbor
        int nextSegId = -1;
        for (auto [to, sid, val] : adj[v]) {
            if (to == nextNeighbor) {
                nextSegId = sid;
                break;
            }
        }
        
        auto it = edgeToIdx.find({v, nextNeighbor, nextSegId});
        if (it != edgeToIdx.end()) {
            nextEdge[i] = it->second;
        }
    }
    
    // Now find cycles using the nextEdge mapping
    vector<bool> visited(numDirectedEdges, false);
    vector<bool> inStack(numDirectedEdges, false);
    
    ll minWeight = 1e18;
    vector<int> bestCycleSegIds;
    
    for (int start = 0; start < numDirectedEdges; start++) {
        if (visited[start]) continue;
        
        vector<int> path;
        int cur = start;
        
        // Follow the cycle
        while (!visited[cur]) {
            visited[cur] = true;
            path.push_back(cur);
            cur = nextEdge[cur];
            if (cur == -1) break;
        }
        
        // Check if we closed a cycle
        if (cur != -1) {
            // Find where cur appears in path
            auto it = find(path.begin(), path.end(), cur);
            if (it != path.end()) {
                vector<int> cycle(path.begin() + (it - path.begin()), path.end());
                
                // Build polygon from cycle
                vector<Point> polygon;
                for (int e : cycle) {
                    auto [u, v, segId] = directedEdges[e];
                    polygon.push_back(points[u]);
                }
                // Ensure polygon is simple and closed
                if (!polygon.empty()) {
                    // Check if this cycle separates the headquarters
                    bool in1 = pointInPolygon(hq1, polygon);
                    bool in2 = pointInPolygon(hq2, polygon);
                    
                    if (in1 != in2) {
                        // Calculate total weight
                        ll weight = 0;
                        set<int> segIds;
                        for (int e : cycle) {
                            weight += edgeWeight[e];
                            segIds.insert(directedEdges[e].seg_id);
                        }
                        
                        if (weight < minWeight) {
                            minWeight = weight;
                            bestCycleSegIds = vector<int>(segIds.begin(), segIds.end());
                            sort(bestCycleSegIds.begin(), bestCycleSegIds.end());
                            // Convert to 1-indexed
                            for (int& x : bestCycleSegIds) x++;
                        }
                    }
                }
            }
        }
    }
    
    // If no separating cycle found, try all possible face boundaries or output default
    if (minWeight == 1e18) {
        // Find the minimum weight cycle regardless of separation (shouldn't happen in valid test)
        minWeight = 1e18;
        for (int start = 0; start < numDirectedEdges; start++) {
            vector<bool> visited(numDirectedEdges, false);
            vector<int> path;
            int cur = start;
            
            while (!visited[cur]) {
                visited[cur] = true;
                path.push_back(cur);
                cur = nextEdge[cur];
                if (cur == -1) break;
            }
            
            if (cur != -1) {
                auto it = find(path.begin(), path.end(), cur);
                if (it != path.end()) {
                    vector<int> cycle(path.begin() + (it - path.begin()), path.end());
                    ll weight = 0;
                    for (int e : cycle) {
                        weight += edgeWeight[e];
                    }
                    if (weight < minWeight) {
                        minWeight = weight;
                        set<int> segIds;
                        for (int e : cycle) {
                            segIds.insert(directedEdges[e].seg_id);
                        }
                        bestCycleSegIds = vector<int>(segIds.begin(), segIds.end());
                        sort(bestCycleSegIds.begin(), bestCycleSegIds.end());
                        for (int& x : bestCycleSegIds) x++;
                    }
                }
            }
        }
    }
    
    cout << minWeight << "\n";
    cout << bestCycleSegIds.size() << "\n";
    for (int i = 0; i < (int)bestCycleSegIds.size(); i++) {
        if (i > 0) cout << " ";
        cout << bestCycleSegIds[i];
    }
    cout << "\n";
    
    return 0;
}
