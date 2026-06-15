#include <bits/stdc++.h>
using namespace std;

int main() {
    long long N, H;
    cin >> N >> H;
    
    vector<pair<long long, long long>> res;
    set<pair<long long, long long>> occupied;
    
    for (long long i = 1; i <= N; i++) {
        long long x = 0, y = N;
        while (y > 0) {
            if (occupied.find({x, y-1}) == occupied.end()) {
                y--;
            } else if (occupied.find({x-1, y}) == occupied.end() && 
                       occupied.find({x-1, y-1}) == occupied.end()) {
                x--;
                y--;
            } else if (occupied.find({x+1, y}) == occupied.end() && 
                       occupied.find({x+1, y-1}) == occupied.end()) {
                x++;
                y--;
            } else {
                break;
            }
        }
        if (y == H) {
            res.push_back({x, i});
        }
        occupied.insert({x, y});
    }
    
    sort(res.begin(), res.end());
    for (int i = 0; i < res.size(); i++) {
        if (i > 0) cout << " ";
        cout << res[i].second;
    }
    cout << endl;
    
    return 0;
}
