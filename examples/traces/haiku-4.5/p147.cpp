#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    int p1, q1, p2, q2, p3, q3;
    cin >> p1 >> q1 >> p2 >> q2 >> p3 >> q3;
    
    // Chebyshev distance
    auto dist = [](int x1, int y1, int x2, int y2) {
        return max(abs(x1 - x2), abs(y1 - y2));
    };
    
    int d_bw = dist(p1, q1, p2, q2);  // distance between black and white kings
    int d_bk_bwk = dist(p1, q1, p3, q3); // distance from black king to black-white king
    int d_wk_bwk = dist(p2, q2, p3, q3); // distance from white king to black-white king
    
    // Kings become adjacent when distance becomes 1
    // They need d_bw - 1 total moves
    // Move order: white(1), black(1), bwk(1), white(1), black(1), bwk(1), ...
    
    // Simulate the game
    bool bwk_wins = false;
    int moves = 0;
    
    for(int round = 1; round <= 2*N; round++) {
        // White king moves
        if(d_wk_bwk > 0) d_wk_bwk--;
        moves++;
        if(d_wk_bwk == 0) {
            bwk_wins = true;
            break;
        }
        
        // Black king moves
        if(d_bk_bwk > 0) d_bk_bwk--;
        moves++;
        if(d_bk_bwk == 0) {
            bwk_wins = true;
            break;
        }
        
        // Check if they're adjacent now
        if(d_bw <= 1) break;
        
        // Black-white king moves
        if(d_bk_bwk > 0 && d_bk_bwk <= d_wk_bwk) {
            d_bk_bwk--;
        } else if(d_wk_bwk > 0) {
            d_wk_bwk--;
        }
        moves++;
        
        if(d_bk_bwk == 0 || d_wk_bwk == 0) {
            bwk_wins = true;
            break;
        }
        
        // Kings get closer
        if(d_bw > 1) d_bw -= 2;
    }
    
    if(bwk_wins) {
        cout << "YES\n" << moves << "\n";
    } else {
        int total = 2 * (d_bw - 1);
        cout << "NO\n" << total << "\n";
    }
    
    return 0;
}
