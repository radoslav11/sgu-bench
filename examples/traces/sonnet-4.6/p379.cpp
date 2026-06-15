#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    ll N,C,P,T;
    cin>>N>>C>>P>>T;
    vector<ll>A(N);
    for(int i=0;i<N;i++) cin>>A[i];
    
    vector<ll> pre(N+1, 0);
    for(int i=0;i<N;i++) pre[i+1]=pre[i]+A[i];
    
    ll total = pre[N];
    
    // Check if we can transport exactly k people within time T
    // Optimal: take k lowest-floor people, assign to trips from highest first
    // Trip positions (1-indexed from bottom): k, k-C, k-2C, ..., last_pos
    // where last_pos = ((k-1) % C) + 1
    // Cost = 2P * sum of floor_of(pos) for each such pos
    auto check=[&](ll k)->bool{
        if(k==0) return true;
        ll last_pos = ((k-1) % C) + 1;
        
        lll cost = 0;
        for(int j=1;j<=N;j++){
            if(A[j-1]==0) continue;
            // Floor j covers positions (pre[j-1], pre[j]]
            // We want positions of form last_pos + i*C in this range, capped at k
            ll cap = min(pre[j], k);
            ll diff_hi = cap - last_pos;
            if(diff_hi < 0) continue;
            ll hi_i = diff_hi / C;
            
            ll diff_lo = pre[j-1] - last_pos;
            ll lo_i;
            if(diff_lo < 0) lo_i = 0;
            else lo_i = diff_lo / C + 1;
            
            if(lo_i > hi_i) continue;
            ll cnt = hi_i - lo_i + 1;
            cost += (lll)cnt * j * 2 * P;
            if(cost > (lll)T) return false;
        }
        return cost <= (lll)T;
    };
    
    ll lo=0, hi=total;
    while(lo<hi){
        ll mid=(lo+hi+1)/2;
        if(check(mid)) lo=mid;
        else hi=mid-1;
    }
    cout<<lo<<endl;
}
