#include<bits/stdc++.h>
using std;

struct Point {
    long long x, y;
    int id;
};

int N;
vector<Point> points;
vector<int> path;
vector<bool> used;
long long minLen = LLONG_MAX;

bool isAxisAligned(Point a, Point b) {
    return a.x == b.x || a.y == b.y;
}

bool isPerpendicular(Point a, Point b, Point c) {
    // Check if segments ab and bc are perpendicular
    bool ab_horizontal = (a.y == b.y);
    bool bc_horizontal = (b.y == c.y);
    return ab_horizontal != bc_horizontal;
}

long long dist(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void dfs(int depth, long long currentLen) {
    if(depth == N) {
        // Check if we can close the polygon with last point to first
        Point last = points[path[N-1]];
        Point first = points[path[0]];
        
        if(!isAxisAligned(last, first)) return;
        
        // Check perpendicularity at closing point
        Point secondLast = points[path[N-2]];
        if(!isPerpendicular(secondLast, last, first)) return;
        
        // Check perpendicularity at first point
        Point second = points[path[1]];
        if(!isPerpendicular(last, first, second)) return;
        
        long long totalLen = currentLen + dist(last, first);
        minLen = min(minLen, totalLen);
        return;
    }
    
    if(currentLen >= minLen) return; // Pruning
    
    for(int i = 0; i < N; i++) {
        if(used[i]) continue;
        
        used[i] = true;
        path[depth] = i;
        
        bool valid = true;
        long long addLen = 0;
        
        if(depth > 0) {
            Point prev = points[path[depth-1]];
            Point curr = points[points[i]];
            
            if(!isAxisAligned(prev, curr)) {
                valid = false;
            }
            
            if(valid && depth > 1) {
                Point prevPrev = points[path[depth-2]];
                if(!isPerpendicular(prevPrev, prev, curr)) {
                    valid = false;
                }
            }
            
            if(valid) {
                addLen = dist(prev, curr);
            }
        }
        
        if(valid) {
            dfs(depth + 1, currentLen + addLen);
        }
        
        used[i] = false;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    
    if(N % 2 == 1) {
        cout << 0 << endl;
        return 0;
    }
    
    points.resize(N);
    used.resize(N, false);
    path.resize(N);
    
    for(int i = 0; i < N; i++) {
        cin >> points[i].x >> points[i].y;
        points[i].id = i;
    }
    
    dfs(0, 0);
    
    if(minLen == LLONG_MAX) {
        cout << 0 << endl;
    } else {
        cout << minLen << endl;
    }
    
    return 0;
}
