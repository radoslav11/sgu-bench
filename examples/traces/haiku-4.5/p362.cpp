#include <bits/stdc++.h>
using std::cout;
using std::cin;

int main() {
    int N, M;
    cin >> N >> M;
    
    int i, j;
    cin >> i >> j;
    
    // Track destroyed buildings
    bool destroyed[11][11];
    memset(destroyed, false, sizeof(destroyed));
    
    string result = "";
    
    while (true) {
        // Mark current building as destroyed
        destroyed[i][j] = true;
        
        // Try to find next building in priority order: D, L, U, R
        int next_i = -1, next_j = -1;
        char direction = ' ';
        
        // Try down (i+1, j)
        if (i + 1 <= N && !destroyed[i + 1][j]) {
            next_i = i + 1;
            next_j = j;
            direction = 'D';
        }
        // Try left (i, j-1)
        else if (j - 1 >= 1 && !destroyed[i][j - 1]) {
            next_i = i;
            next_j = j - 1;
            direction = 'L';
        }
        // Try up (i-1, j)
        else if (i - 1 >= 1 && !destroyed[i - 1][j]) {
            next_i = i - 1;
            next_j = j;
            direction = 'U';
        }
        // Try right (i, j+1)
        else if (j + 1 <= M && !destroyed[i][j + 1]) {
            next_i = i;
            next_j = j + 1;
            direction = 'R';
        }
        
        // If no valid next building, robot stops
        if (next_i == -1) {
            break;
        }
        
        // Move to next building
        i = next_i;
        j = next_j;
        result += direction;
    }
    
    cout << result;
    
    return 0;
}
