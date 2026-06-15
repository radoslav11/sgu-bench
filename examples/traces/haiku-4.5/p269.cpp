#include<bits/stdc++.h>
using namespace std;

int n, k;
vector<int> b;
map<pair<int,pair<int,set<int>>>, long long> memo;

long long solve(int row, int rooks_left, set<int> used_cols) {
    if (rooks_left == 0) return 1;
    if (row == n) return 0;
    
    auto key = make_pair(row, make_pair(rooks_left, used_cols));
    if (memo.count(key)) return memo[key];
    
    long long result = 0;
    
    result += solve(row + 1, rooks_left, used_cols);
    
    for (int col = 1; col <= b[row]; col++) {
        if (used_cols.find(col) == used_cols.end()) {
            set<int> new_used = used_cols;
            new_used.insert(col);
            result += solve(row + 1, rooks_left - 1, new_used);
        }
    }
    
    return memo[key] = result;
}

int main(){
    cin >> n >> k;
    b.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }
    
    set<int> empty_set;
    cout << solve(0, k, empty_set) << endl;
    
    return 0;
}
