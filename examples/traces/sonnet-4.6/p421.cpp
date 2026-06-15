#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

int n, m, k;
vector<ll> a;
vector<vector<lll>> suf_max_p, suf_min_p;
const lll NEG_INF = (lll)(-1) * (lll)(1000000000000000000LL) * (lll)(1000000000000000000LL);
const lll POS_INF = (lll)(1000000000000000000LL) * (lll)(1000000000000000000LL);

priority_queue<lll, vector<lll>, greater<lll>> pq;

void dfs(int idx, int cnt, lll prod){
    if(cnt == m){
        if((int)pq.size() < k) pq.push(prod);
        else if(prod > pq.top()){ pq.pop(); pq.push(prod); }
        return;
    }
    int need = m - cnt;
    if(n - idx < need) return;
    
    if((int)pq.size() == k){
        lll best = max(prod * suf_max_p[idx][need], prod * suf_min_p[idx][need]);
        if(best <= pq.top()) return;
    }
    
    for(int i = idx; i <= n-need; i++){
        dfs(i+1, cnt+1, prod*(lll)a[i]);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n >> m >> k;
    a.resize(n);
    for(auto& x: a) cin >> x;
    sort(a.begin(), a.end(), greater<ll>());
    
    suf_max_p.assign(n+1, vector<lll>(m+1, NEG_INF));
    suf_min_p.assign(n+1, vector<lll>(m+1, POS_INF));
    for(int i = 0; i <= n; i++) { suf_max_p[i][0] = suf_min_p[i][0] = 1; }
    
    for(int i = n-1; i >= 0; i--){
        for(int j = 1; j <= min(m, n-i); j++){
            suf_max_p[i][j] = suf_max_p[i+1][j];
            suf_min_p[i][j] = suf_min_p[i+1][j];
            if(suf_max_p[i+1][j-1] != NEG_INF){
                lll v = (lll)a[i] * suf_max_p[i+1][j-1];
                suf_max_p[i][j] = max(suf_max_p[i][j], v);
                suf_min_p[i][j] = min(suf_min_p[i][j], v);
            }
            if(suf_min_p[i+1][j-1] != POS_INF){
                lll v = (lll)a[i] * suf_min_p[i+1][j-1];
                suf_max_p[i][j] = max(suf_max_p[i][j], v);
                suf_min_p[i][j] = min(suf_min_p[i][j], v);
            }
        }
    }
    
    dfs(0, 0, (lll)1);
    
    ll ans = (ll)pq.top();
    cout << ans << "\n";
}
