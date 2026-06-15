#include<bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
};

int n;
vector<Point> points;

bool canCoverWithSquare(vector<int>& uncovered, long long x0, long long y0, long long d) {
    for(int idx : uncovered) {
        if(points[idx].x < x0 || points[idx].x > x0 + d ||
           points[idx].y < y0 || points[idx].y > y0 + d) {
            return false;
        }
    }
    return true;
}

bool canCoverWith3Squares(long long d) {
    // Try first square at various positions
    for(int i = 0; i < n; i++) {
        // Position first square so point i is at bottom-left corner
        long long x1 = points[i].x;
        long long y1 = points[i].y;
        
        vector<int> uncovered1;
        for(int j = 0; j < n; j++) {
            if(points[j].x < x1 || points[j].x > x1 + d ||
               points[j].y < y1 || points[j].y > y1 + d) {
                uncovered1.push_back(j);
            }
        }
        
        if(uncovered1.empty()) return true;
        
        // Try second square at various positions
        for(int j : uncovered1) {
            long long x2 = points[j].x;
            long long y2 = points[j].y;
            
            vector<int> uncovered2;
            for(int k : uncovered1) {
                if(points[k].x < x2 || points[k].x > x2 + d ||
                   points[k].y < y2 || points[k].y > y2 + d) {
                    uncovered2.push_back(k);
                }
            }
            
            if(uncovered2.empty()) return true;
            
            // Try third square
            for(int k : uncovered2) {
                long long x3 = points[k].x;
                long long y3 = points[k].y;
                
                bool ok = true;
                for(int m : uncovered2) {
                    if(points[m].x < x3 || points[m].x > x3 + d ||
                       points[m].y < y3 || points[m].y > y3 + d) {
                        ok = false;
                        break;
                    }
                }
                if(ok) return true;
            }
        }
    }
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    points.resize(n);
    
    for(int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;
    }
    
    long long left = 0, right = 2e9;
    long long answer = right;
    
    while(left <= right) {
        long long mid = left + (right - left) / 2;
        if(canCoverWith3Squares(mid)) {
            answer = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}
