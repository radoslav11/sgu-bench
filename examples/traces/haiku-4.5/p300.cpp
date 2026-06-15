#include<bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

long long dist(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool segmentsIntersect(Point p1, Point p2, Point p3, Point p4) {
    // Check if segment p1-p2 intersects with p3-p4
    auto onSegment = [](Point p, Point q, Point r) {
        return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
               q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
    };
    
    auto orientation = [](Point p, Point q, Point r) {
        long long val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0;
        return (val > 0) ? 1 : 2;
    };
    
    int o1 = orientation(p1, p2, p3);
    int o2 = orientation(p1, p2, p4);
    int o3 = orientation(p3, p4, p1);
    int o4 = orientation(p3, p4, p2);
    
    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && onSegment(p1, p3, p2)) return true;
    if (o2 == 0 && onSegment(p1, p4, p2)) return true;
    if (o3 == 0 && onSegment(p3, p1, p4)) return true;
    if (o4 == 0 && onSegment(p3, p2, p4)) return true;
    
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<Point> v(n);
    
    for(int i = 0; i < n; i++) {
        cin >> v[i].x >> v[i].y;
    }
    
    long long maxLen = 0;
    
    for(int i = 0; i < n; i++) {
        for(int j = i; j < n; j++) {
            // Check if segment from i to j intersects with other parts
            bool valid = true;
            
            for(int a = 0; a < n-1 && valid; a++) {
                for(int b = a+1; b < n; b++) {
                    // Check segment a-b against train segment i-j
                    if((a >= i && a <= j) || (b >= i && b <= j)) continue;
                    if(b == a + 1) { // consecutive segments
                        // only skip if both are in range
                        if(!((a >= i && a < j) || (b > i && b <= j))) {
                            if(segmentsIntersect(v[i], v[j], v[a], v[b])) {
                                valid = false;
                                break;
                            }
                        }
                    }
                }
            }
            
            if(valid) {
                long long len = 0;
                for(int k = i; k < j; k++) {
                    len += dist(v[k], v[k+1]);
                }
                maxLen = max(maxLen, len);
            }
        }
    }
    
    cout << maxLen << endl;
    
    return 0;
}
