#include <bits/stdc++.h>
using namespace std;

int main() {
    int grid[4][4];
    int blank_row = 0;
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 0)
                blank_row = i + 1; // 1-indexed from top
        }
    
    // Flatten ignoring 0
    vector<int> tiles;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (grid[i][j] != 0)
                tiles.push_back(grid[i][j]);
    
    // Count inversions
    long long inversions = 0;
    for (int i = 0; i < (int)tiles.size(); i++)
        for (int j = i + 1; j < (int)tiles.size(); j++)
            if (tiles[i] > tiles[j])
                inversions++;
    
    if ((inversions + blank_row) % 2 == 0)
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    
    return 0;
}
