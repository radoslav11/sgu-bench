#include<bits/stdc++.h>
using namespace std;

struct Segment {
    long long x1, y1, x2, y2;
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Segment> segments(n);
    for(int i = 0; i < n; i++){
        cin >> segments[i].x1 >> segments[i].y1 >> segments[i].x2 >> segments[i].y2;
    }
    
    int count = 0;
    
    // Check all pairs
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            Segment& s1 = segments[i];
            Segment& s2 = segments[j];
            
            // Direction vectors
            long long dx1 = s1.x2 - s1.x1;
            long long dy1 = s1.y2 - s1.y1;
            long long dx2 = s2.x2 - s2.x1;
            long long dy2 = s2.y2 - s2.y1;
            
            // Check if perpendicular (dot product = 0)
            long long dotProduct = dx1 * dx2 + dy1 * dy2;
            if(dotProduct != 0) continue;
            
            // Check if they share exactly one endpoint
            vector<pair<long long, long long>> endpoints1 = {{s1.x1, s1.y1}, {s1.x2, s1.y2}};
            vector<pair<long long, long long>> endpoints2 = {{s2.x1, s2.y1}, {s2.x2, s2.y2}};
            
            int sharedCount = 0;
            for(auto& e1 : endpoints1){
                for(auto& e2 : endpoints2){
                    if(e1.first == e2.first && e1.second == e2.second){
                        sharedCount++;
                    }
                }
            }
            
            if(sharedCount == 1){
                count++;
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
