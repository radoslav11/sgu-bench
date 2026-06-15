#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<int> puzzle(16);
    int blank_pos = -1;
    
    for(int i = 0; i < 16; i++){
        cin >> puzzle[i];
        if(puzzle[i] == 0){
            blank_pos = i;
        }
    }
    
    // Count inversions (excluding 0)
    int inversions = 0;
    for(int i = 0; i < 16; i++){
        if(puzzle[i] == 0) continue;
        for(int j = i + 1; j < 16; j++){
            if(puzzle[j] == 0) continue;
            if(puzzle[i] > puzzle[j]){
                inversions++;
            }
        }
    }
    
    // Get row of blank from bottom (1-indexed)
    // blank_pos goes from 0-15
    // Row from top: blank_pos / 4 (0-3)
    // Row from bottom (1-indexed): 4 - (blank_pos / 4)
    int blank_row_from_bottom = 4 - (blank_pos / 4);
    
    // Solvable if (inversions + blank_row_from_bottom) is odd
    if((inversions + blank_row_from_bottom) % 2 == 1){
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
