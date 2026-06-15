#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;

ll cross(pll O, pll A, pll B) {
    return (A.first - O.first) * (B.second - O.second) - (A.second - O.second) * (B.first - O.first);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N, M, K;
    cin >> N >> M >> K;
    vector<pll> poly(N);
    for(int i = 0; i < N; i++) cin >> poly[i].first >> poly[i].second;
    int cnt = 0;
    for(int j = 0; j < M; j++){
        pll q;
        cin >> q.first >> q.second;
        ll c1 = cross(poly[0], poly[1], q);
        if(c1 < 0) continue;
        ll cn = cross(poly[0], poly[N-1], q);
        if(cn > 0) continue;
        int lo = 1, hi = N - 1;
        while(hi - lo > 1){
            int mid = (lo + hi) / 2;
            ll c = cross(poly[0], poly[mid], q);
            if(c >= 0) lo = mid;
            else hi = mid;
        }
        ll c = cross(poly[lo], poly[lo+1], q);
        if(c >= 0) cnt++;
    }
    cout << (cnt >= K ? "YES" : "NO") << endl;
    return 0;
}
