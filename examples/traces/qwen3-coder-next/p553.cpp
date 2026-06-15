#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<ll> pw(n + 1, 0);
    vector<ll> pc(n + 1, 0);
    
    for (int i = 1; i <= n; i++) {
        ll w, c;
        scanf("%lld %lld", &w, &c);
        pw[i] = pw[i - 1] + w;
        pc[i] = pc[i - 1] + c;
    }
    
    ll best_sum = 0;
    int best_count = 0;
    int best_a = 0;
    int best_b = 0;
    
    for (int r = n; r >= m + 1; r--) {
        int R = r - m;
        ll X = (ll)(k + 1) * pw[R] - pw[r];
        if (X < 0) continue;
        
        int lo = 0;
        int hi = R - 1;
        int i0 = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if ((ll)k * pw[mid] <= X) {
                i0 = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        
        if (i0 == -1) continue;
        ll cost_here = pc[i0] + (pc[n] - pc[r]);
        int count_here = i0 + (n - r);
        if (cost_here > best_sum) {
            best_sum = cost_here;
            best_count = count_here;
            best_a = i0;
            best_b = n - r;
        }
    }
    
    if (best_count == 0) {
        printf("0 0\n");
    } else {
        printf("%d %lld\n", best_count, best_sum);
        string seq = string(best_a, 'T') + string(best_b, 'H');
        printf("%s\n", seq.c_str());
    }
    
    return 0;
}
