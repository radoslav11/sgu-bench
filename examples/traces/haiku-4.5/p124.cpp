#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int k;
    cin >> k;
    
    vector<array<int,4>> segs(k);
    for(int i = 0; i < k; i++) {
        cin >> segs[i][0] >> segs[i][1] >> segs[i][2] >> segs[i][3];
    }
    
    int x0, y0;
    cin >> x0 >> y0;
    
    // Check if on border
    for(int i = 0; i < k; i++) {
        int x1=segs[i][0], y1=segs[i][1], x2=segs[i][2], y2=segs[i][3];
        int minX = min(x1, x2), maxX = max(x1, x2);
        int minY = min(y1, y2), maxY = max(y1, y2);
        if(x0 >= minX && x0 <= maxX && y0 >= minY && y0 <= maxY) {
            cout << "BORDER" << endl;
            return 0;
        }
    }
    
    // Ray casting
    int cnt = 0;
    for(int i = 0; i < k; i++) {
        int x1=segs[i][0], y1=segs[i][1], x2=segs[i][2], y2=segs[i][3];
        if(x1 == x2 && x1 > x0 && ((y1 <= y0 && y0 < y2) || (y2 <= y0 && y0 < y1))) {
            cnt++;
        }
    }
    
    cout << (cnt % 2 ? "INSIDE" : "OUTSIDE") << endl;
    
    return 0;
}
