#include<bits/stdc++.h>
using namespace std;

int M, N;
map<pair<int,int>, long long> memo;

long long fill(int col, int row, int cur_mask, int next_mask) {
    if (row == M) {
        if (col + 1 == N) {
            return next_mask == 0 ? 1 : 0;
        }
        auto key = make_pair(col + 1, next_mask);
        if (memo.count(key)) return memo[key];
        return memo[key] = fill(col + 1, 0, next_mask, 0);
    }
    
    if (cur_mask & (1 << row)) {
        return fill(col, row + 1, cur_mask, next_mask);
    }
    
    long long result = 0;
    
    // Horizontal 2x1
    if (col + 1 < N) {
        result += fill(col, row + 1, cur_mask | (1 << row), next_mask | (1 << row));
    }
    
    // Vertical 2x1
    if (row + 1 < M && !(cur_mask & (1 << (row + 1)))) {
        result += fill(col, row + 2, cur_mask | (1 << row) | (1 << (row + 1)), next_mask);
    }
    
    // 2x2 corner pieces (3 filled cells in 2x2 area)
    if (row + 1 < M && col + 1 < N && !(cur_mask & (1 << (row + 1)))) {
        result += 4 * fill(col, row + 2, 
                          cur_mask | (1 << row) | (1 << (row + 1)), 
                          next_mask | (1 << row) | (1 << (row + 1)));
    }
    
    return result;
}

int main(){
    int m, n;
    cin >> m >> n;
    M = m; N = n;
    
    long long ans = fill(0, 0, 0, 0);
    cout << ans << endl;
    
    return 0;
}
