#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<vector<int>> grid(4, vector<int>(4));
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cin >> grid[i][j];
        }
    }
    
    // Flatten the grid into a 1D array of 16 elements
    vector<int> arr;
    int blank_row = -1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            arr.push_back(grid[i][j]);
            if (grid[i][j] == 0) {
                blank_row = i;
            }
        }
    }
    
    // Count inversions among non-zero elements
    int inversions = 0;
    for (int i = 0; i < 16; i++) {
        if (arr[i] == 0) continue;
        for (int j = i + 1; j < 16; j++) {
            if (arr[j] == 0) continue;
            if (arr[i] > arr[j]) {
                inversions++;
            }
        }
    }
    
    // For 4x4 puzzle: solvable iff (inversions % 2) != (blank_row % 2)
    if ((inversions % 2) != (blank_row % 2)) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
    
    return 0;
}
