#include <bits/stdc++.h>
using namespace std;

int main(){
    int K;
    cin >> K;
    
    vector<int> x1v(K), y1v(K), x2v(K), y2v(K);
    for(int i = 0; i < K; i++){
        cin >> x1v[i] >> y1v[i] >> x2v[i] >> y2v[i];
    }
    
    int X0, Y0;
    cin >> X0 >> Y0;
    
    // Check BORDER first
    for(int i = 0; i < K; i++){
        int ax = x1v[i], ay = y1v[i], bx = x2v[i], by = y2v[i];
        if(ay == by){
            // horizontal segment
            if(Y0 == ay){
                int minx = min(ax, bx), maxx = max(ax, bx);
                if(X0 >= minx && X0 <= maxx){
                    cout << "BORDER" << endl;
                    return 0;
                }
            }
        } else {
            // vertical segment
            if(X0 == ax){
                int miny = min(ay, by), maxy = max(ay, by);
                if(Y0 >= miny && Y0 <= maxy){
                    cout << "BORDER" << endl;
                    return 0;
                }
            }
        }
    }
    
    // Ray casting: shoot ray in +X direction from (X0, Y0)
    // Count intersections with vertical segments
    // Use half-open interval: [miny, maxy) to handle corners
    int count = 0;
    for(int i = 0; i < K; i++){
        int ax = x1v[i], ay = y1v[i], bx = x2v[i], by = y2v[i];
        if(ay == by){
            // horizontal, skip
            continue;
        }
        // vertical segment at x = ax
        if(ax <= X0) continue; // must be strictly to the right
        int miny = min(ay, by), maxy = max(ay, by);
        // Use half-open [miny, maxy)
        if(Y0 >= miny && Y0 < maxy){
            count++;
        }
    }
    
    if(count % 2 == 1){
        cout << "INSIDE" << endl;
    } else {
        cout << "OUTSIDE" << endl;
    }
    
    return 0;
}
