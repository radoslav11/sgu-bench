#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<pair<long long, long long>> points(n);
    for(int i = 0; i < n; i++){
        cin >> points[i].first >> points[i].second;
    }
    
    int friendCount = 0;
    
    // Check all pairs
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            long long x1 = points[i].first;
            long long y1 = points[i].second;
            long long x2 = points[j].first;
            long long y2 = points[j].second;
            
            long long minX = min(x1, x2);
            long long maxX = max(x1, x2);
            long long minY = min(y1, y2);
            long long maxY = max(y1, y2);
            
            // Check if any other point is in this rectangle
            bool hasForeignPoint = false;
            for(int k = 0; k < n; k++){
                if(k == i || k == j) continue;
                
                long long xk = points[k].first;
                long long yk = points[k].second;
                
                if(xk >= minX && xk <= maxX && yk >= minY && yk <= maxY){
                    hasForeignPoint = true;
                    break;
                }
            }
            
            if(!hasForeignPoint){
                friendCount++;
            }
        }
    }
    
    cout << friendCount << endl;
    
    return 0;
}
