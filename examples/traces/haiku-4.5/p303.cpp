#include<bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;
};

struct Segment {
    Point p1, p2;
    int passability;
    int id;
};

double cross(Point O, Point A, Point B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

int pointInPolygon(Point p, vector<Segment>& poly) {
    int n = poly.size();
    int cnt = 0;
    for(int i = 0; i < n; i++) {
        Point a = poly[i].p1;
        Point b = poly[i].p2;
        if(a.y > b.y) swap(a, b);
        if(p.y < a.y || p.y >= b.y) continue;
        double t = (p.y - a.y) / (b.y - a.y);
        double x = a.x + t * (b.x - a.x);
        if(x > p.x) cnt++;
    }
    return cnt & 1;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Segment> segments(n);
    map<pair<double,double>, vector<int>> graph;
    
    for(int i = 0; i < n; i++) {
        cin >> segments[i].p1.x >> segments[i].p1.y >> segments[i].p2.x >> segments[i].p2.y >> segments[i].passability;
        segments[i].id = i + 1;
        graph[{segments[i].p1.x, segments[i].p1.y}].push_back(i);
        graph[{segments[i].p2.x, segments[i].p2.y}].push_back(i);
    }
    
    Point h1, h2;
    cin >> h1.x >> h1.y >> h2.x >> h2.y;
    
    int minCost = INT_MAX;
    vector<int> bestPath;
    
    // Try to find cycles using DFS from each segment
    for(int start = 0; start < n; start++) {
        // DFS to find cycles
        function<void(int, vector<int>&, set<int>&, int, int)> dfs = 
            [&](int segIdx, vector<int>& path, set<int>& used, int cost, int startPoint) {
            if(path.size() > 2) {
                Point endP = segments[segIdx].p2;
                if(endP.x == segments[start].p1.x && endP.y == segments[start].p1.y) {
                    // Found a cycle
                    vector<Segment> cycle;
                    for(int id : path) {
                        cycle.push_back(segments[id]);
                    }
                    
                    int in1 = pointInPolygon(h1, cycle);
                    int in2 = pointInPolygon(h2, cycle);
                    
                    if(in1 != in2 && cost < minCost) {
                        minCost = cost;
                        bestPath = path;
                    }
                    return;
                }
            }
            
            if(path.size() > n) return;
            
            Point nextStart = segments[segIdx].p2;
            for(int nextSeg : graph[{nextStart.x, nextStart.y}]) {
                if(used.find(nextSeg) == used.end()) {
                    used.insert(nextSeg);
                    path.push_back(nextSeg);
                    dfs(nextSeg, path, used, cost + segments[nextSeg].passability, startPoint);
                    path.pop_back();
                    used.erase(nextSeg);
                }
            }
        };
        
        vector<int> path = {start};
        set<int> used = {start};
        dfs(start, path, used, segments[start].passability, 0);
    }
    
    cout << minCost << "\n";
    cout << bestPath.size() << "\n";
    for(int id : bestPath) {
        cout << segments[id].id << " ";
    }
    cout << "\n";
    
    return 0;
}
