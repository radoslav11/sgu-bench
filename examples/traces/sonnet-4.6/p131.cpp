#include <bits/stdc++.h>
using namespace std;
int M, N;
map<tuple<int,int,int,int>, long long> memo;
long long solve(int col, int row, int cur_mask, int next_mask) {
    if (col == N) return (cur_mask == 0) ? 1 : 0;
    if (row == M) {
        if (cur_mask != 0) return 0;
        return solve(col + 1, 0, next_mask, 0);
    }
    auto key = make_tuple(col, row, cur_mask, next_mask);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    long long res = 0;
    if (cur_mask & (1 << row)) {
        res = solve(col, row + 1, cur_mask, next_mask);
    } else {
        if (col + 1 < N && !(next_mask & (1 << row)))
            res += solve(col, row + 1, cur_mask, next_mask | (1 << row));
        if (row + 1 < M && !(cur_mask & (1 << (row+1))))
            res += solve(col, row + 2, cur_mask, next_mask);
        if (col + 1 < N) {
            if (row+1<M && !(cur_mask&(1<<(row+1))) && !(next_mask&(1<<(row+1))))
                res += solve(col, row+1, cur_mask|(1<<(row+1)), next_mask|(1<<(row+1)));
            if (row+1<M && !(next_mask&(1<<row)) && !(next_mask&(1<<(row+1))))
                res += solve(col, row+1, cur_mask, next_mask|(1<<row)|(1<<(row+1)));
            if (row+1<M && !(cur_mask&(1<<(row+1))) && !(next_mask&(1<<row)))
                res += solve(col, row+2, cur_mask, next_mask|(1<<row));
        }
    }
    memo[key] = res;
    return res;
}
int main() {
    cin >> M >> N;
    cout << solve(0, 0, 0, 0) << endl;
}
