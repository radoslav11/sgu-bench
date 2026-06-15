#include <bits/stdc++.h>
using namespace std;

map<long long, pair<int,vector<int>>> memo;

pair<int,vector<int>> solve(long long n) {
    if (n == 1) return {0, {}};
    if (n % 2 == 0) return {INT_MAX, {}};
    if (memo.count(n)) return memo[n];
    
    // reverse type1: n = 2x-1 => x = (n+1)/2, then forward was type1
    // reverse type2: n = 2x+1 => x = (n-1)/2, then forward was type2
    auto r1 = solve((n+1)/2);
    auto r2 = solve((n-1)/2);
    
    pair<int,vector<int>> res;
    if (r1.first == INT_MAX && r2.first == INT_MAX) res = {INT_MAX, {}};
    else if (r1.first <= r2.first && r1.first != INT_MAX) {
        res = r1; res.first++; res.second.push_back(1);
    } else {
        res = r2; res.first++; res.second.push_back(2);
    }
    return memo[n] = res;
}

int main(){
    long long n; cin >> n;
    if(n==1){cout<<0<<endl<<endl;return 0;}
    auto [cnt, seq] = solve(n);
    if(cnt == INT_MAX){cout<<"No solution"<<endl;return 0;}
    cout << cnt << "\n";
    for(int x: seq) cout << x << " ";
    cout << "\n";
}
