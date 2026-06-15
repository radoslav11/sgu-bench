#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    
    bool first = true;
    while(T--){
        if(!first) cout << "\n";
        first = false;
        
        int H, W, N;
        cin >> H >> W >> N;
        
        // Maximum spots: checkerboard-like pattern
        int maxSpots = (H + 1) / 2 * ((W + 1) / 2) + H / 2 * (W / 2);
        
        if(N > maxSpots){
            cout << "Impossible\n";
            continue;
        }
        
        vector<string> grid(H, string(W, '.'));
        
        // Place N white spots in a grid pattern, spaced 2 apart
        int spotCount = 0;
        for(int i = 0; i < H && spotCount < N; i += 2){
            for(int j = 0; j < W && spotCount < N; j += 2){
                grid[i][j] = '#';
                spotCount++;
            }
        }
        
        for(int i = 0; i < H; i++){
            cout << grid[i] << "\n";
        }
    }
    
    return 0;
}
