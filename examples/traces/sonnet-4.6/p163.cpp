#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, p;
    cin >> n >> p;
    
    long long total = 0;
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v;
        long long vp = 1;
        for (int j = 0; j < p; j++) vp *= v;
        if (vp > 0) total += vp;
    }
    
    cout << total << endl;
    return 0;
}
