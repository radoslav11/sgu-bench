#include<bits/stdc++.h>
using namespace std;

int main(){
    int H, W;
    cin >> H >> W;
    vector<string> grid(H);
    
    for(int i = 0; i < H; i++) cin >> grid[i];
    
    cout << "1\n";
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cout << (grid[i][j] == 'X' ? 'A' : '.');
        }
        cout << "\n";
    }
    cout << "\n";
    cout << "AAA\nAAA\nAAA\n";
    
    return 0;
}
