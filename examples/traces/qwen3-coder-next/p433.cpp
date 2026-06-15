#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <cstring>
#include <climits>
#include <cmath>
#include <iomanip>
using namespace std;

int n, m, k;
vector<int> bricks;
vector<string> grid;
vector<int> used;
bool found = false;

// Check if we can place a brick of given length starting at (r, c) horizontally
bool canPlaceHorizontal(int r, int c, int len) {
    if (c + len > m) return false;
    for (int j = c; j < c + len; j++) {
        if (grid[r][j] != '.') return false;
    }
    return true;
}

// Place a horizontal brick of given length starting at (r, c) with character ch
void placeHorizontal(int r, int c, int len, char ch) {
    for (int j = c; j < c + len; j++) {
        grid[r][j] = ch;
    }
}

// Remove a horizontal brick (restore to '.')
void removeHorizontal(int r, int c, int len) {
    for (int j = c; j < c + len; j++) {
        grid[r][j] = '.';
    }
}

// Check if we can place a brick vertically
bool canPlaceVertical(int r, int c, int len) {
    if (r + len > n) return false;
    for (int i = r; i < r + len; i++) {
        if (grid[i][c] != '.') return false;
    }
    return true;
}

// Place a vertical brick
void placeVertical(int r, int c, int len, char ch) {
    for (int i = r; i < r + len; i++) {
        grid[i][c] = ch;
    }
}

// Remove a vertical brick
void removeVertical(int r, int c, int len) {
    for (int i = r; i < r + len; i++) {
        grid[i][c] = '.';
    }
}

// Backtracking function to fill the grid
bool backtrack(int pos, char nextChar) {
    if (found) return true;
    
    // Find next empty cell
    int r = -1, c = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '.') {
                r = i;
                c = j;
                break;
            }
        }
        if (r != -1) break;
    }
    
    // If no empty cell, we've filled the entire grid
    if (r == -1) {
        found = true;
        return true;
    }
    
    // Try each brick type (both orientations if applicable)
    for (int i = 0; i < k; i++) {
        int len = bricks[i];
        
        // Try horizontal placement
        if (canPlaceHorizontal(r, c, len)) {
            placeHorizontal(r, c, len, nextChar);
            if (backtrack(pos + 1, nextChar + 1)) {
                return true;
            }
            removeHorizontal(r, c, len);
        }
        
        // Try vertical placement (only if len > 1, or it's redundant with horizontal)
        if (len > 1 && canPlaceVertical(r, c, len)) {
            placeVertical(r, c, len, nextChar);
            if (backtrack(pos + 1, nextChar + 1)) {
                return true;
            }
            removeVertical(r, c, len);
        }
    }
    
    return false;
}

int main() {
    cin >> n >> m >> k;
    bricks.resize(k);
    for (int i = 0; i < k; i++) {
        cin >> bricks[i];
    }
    
    // Calculate total area
    long long totalArea = (long long)n * m;
    long long brickSum = 0;
    for (int len : bricks) {
        brickSum += len;
    }
    
    // Quick check: if total area is not divisible by GCD or brickSum doesn't divide evenly
    // Actually, we need the sum of bricks used to equal totalArea, but we have infinite bricks
    // So we need to check if there's a linear combination that equals totalArea
    // Since K is small (<=5) and M+N <= 20 (so total area <= 100), we can do a simple check
    
    // First check: total area must be reachable using given brick lengths
    vector<bool> dp(totalArea + 1, false);
    dp[0] = true;
    for (int len : bricks) {
        for (int i = len; i <= totalArea; i++) {
            if (dp[i - len]) dp[i] = true;
        }
    }
    
    if (!dp[totalArea]) {
        cout << "NO" << endl;
        return 0;
    }
    
    // Initialize grid with dots
    grid.assign(n, string(m, '.'));
    
    // Try backtracking to fill the grid
    found = false;
    if (backtrack(0, 'a')) {
        cout << "YES" << endl;
        for (int i = 0; i < n; i++) {
            cout << grid[i] << endl;
        }
    } else {
        cout << "NO" << endl;
    }
    
    return 0;
}
