#include <bits/stdc++.h>
using default_random_engine = mt19937;
using namespace std;

map<tuple<int,int,int>, int> memo;

int grundy(int lc, int rc, int n) {
    // lc: 0=free,1=black forbidden,2=white forbidden on left end
    // rc: similar on right end
    if (n == 0) return 0;
    auto key = make_tuple(lc, rc, n);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    
    set<int> reachable;
    for (int i = 1; i <= n; i++) {
        for (int c = 1; c <= 2; c++) {
            // Check if coloring position i with color c is valid
            bool ok = true;
            if (i == 1 && lc == c) ok = false;
            if (i == n && rc == c) ok = false;
            if (!ok) continue;
            // left segment: positions 1..i-1, left constraint lc, right constraint c
            int gl = grundy(lc, c, i-1);
            // right segment: positions i+1..n, left constraint c, right constraint rc
            int gr = grundy(c, rc, n-i);
            reachable.insert(gl ^ gr);
        }
    }
    int mex = 0;
    while (reachable.count(mex)) mex++;
    memo[key] = mex;
    return mex;
}

int main(){
    int n;
    cin >> n;
    string s;
    cin >> s;
    
    int xorSum = 0;
    int i = 0;
    while (i < n) {
        if (s[i] != '0') { i++; continue; }
        // find segment of zeros
        int lc = (i > 0) ? (s[i-1]-'0') : 0;
        int j = i;
        while (j < n && s[j] == '0') j++;
        int rc = (j < n) ? (s[j]-'0') : 0;
        int len = j - i;
        xorSum ^= grundy(lc, rc, len);
        i = j;
    }
    
    cout << (xorSum != 0 ? "FIRST" : "SECOND") << endl;
    return 0;
}
