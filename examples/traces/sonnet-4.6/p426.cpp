#include <bits/stdc++.h>
using namespace std;

int booth(vector<int>& s) {
    int n = s.size();
    vector<int> f(2*n, -1);
    int k = 0;
    for (int j = 1; j < 2*n; j++) {
        int sj = s[j % n];
        int i = f[j - 1 - k];
        while (i != -1 && sj != s[(k + i + 1) % n]) {
            if (sj < s[(k + i + 1) % n]) k = j - i - 1;
            i = f[i];
        }
        if (sj != s[(k + i + 1) % n]) {
            if (sj < s[k % n]) k = j;
            f[j - k] = -1;
        } else {
            f[j - k] = i + 1;
        }
    }
    return k;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m, k;
    cin >> n >> m >> k;
    
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    
    vector<int> s(n);
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) s[i] = (a[i] + j) % m;
        int start = booth(s);
        int idx = (start + k - 1) % n;
        cout << s[idx] << "\n";
    }
    
    return 0;
}
