#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n,m;
vector<ll> P,Q;

vector<pair<ll,ll>> buildIntervals(const vector<ll>& pos, ll t){
    vector<pair<ll,ll>> res;
    ll s=pos[0], last=pos[0];
    for(size_t i=1;i<pos.size();i++){
        if(pos[i]-last <= 2*t){
            last=pos[i];
        } else {
            res.push_back({s-t, last+t});
            s=pos[i]; last=pos[i];
        }
    }
    res.push_back({s-t,last+t});
    return res;
}

ll maxIntersect(const vector<pair<ll,ll>>& A, const vector<pair<ll,ll>>& B){
    ll best=-1;
    size_t i=0,j=0;
    while(i<A.size() && j<B.size()){
        ll lo=max(A[i].first,B[j].first);
        ll hi=min(A[i].second,B[j].second);
        if(hi>lo) best=max(best,hi-lo);
        if(A[i].second < B[j].second) i++; else j++;
    }
    return best;
}

int countFor(const vector<ll>& gapsSorted, ll t){
    ll x=2*t;
    int gt = (int)(gapsSorted.end()-upper_bound(gapsSorted.begin(),gapsSorted.end(),x));
    return 1+gt;
}

int main(){
    ll d;
    if(scanf("%d %d %lld",&n,&m,&d)!=3) return 0;
    P.resize(n); Q.resize(m);
    for(auto&x:P)scanf("%lld",&x);
    for(auto&x:Q)scanf("%lld",&x);
    set<ll> c1set, c2set;
    c1set.insert(1); c2set.insert(1);
    vector<ll> g1,g2;
    for(int i=1;i<n;i++){ll g=P[i]-P[i-1]; g1.push_back(g); c1set.insert((g+1)/2);}
    for(int i=1;i<m;i++){ll g=Q[i]-Q[i-1]; g2.push_back(g); c2set.insert((g+1)/2);}
    sort(g1.begin(),g1.end()); sort(g2.begin(),g2.end());
    vector<ll> T1(c1set.begin(),c1set.end());
    vector<ll> T2(c2set.begin(),c2set.end());
    vector<int> cnt1(T1.size());
    for(size_t i=0;i<T1.size();i++) cnt1[i]=countFor(g1,T1[i]);
    
    long long bestSum=LLONG_MAX;
    ll ansT1=-1,ansT2=-1;
    int idx=-1;
    for(int ti=(int)T2.size()-1; ti>=0; ti--){
        ll t2=T2[ti];
        int c2=countFor(g2,t2);
        vector<pair<ll,ll>> I2=buildIntervals(Q,t2);
        while(idx+1 < (int)T1.size()){
            vector<pair<ll,ll>> I1=buildIntervals(P,T1[idx+1]);
            ll ov=maxIntersect(I1,I2);
            if(ov<=d){ idx++; }
            else break;
        }
        if(idx>=0){
            long long s=(long long)cnt1[idx]+c2;
            if(s<bestSum){ bestSum=s; ansT1=T1[idx]; ansT2=t2; }
        }
    }
    if(bestSum==LLONG_MAX) printf("No solution\n");
    else printf("%lld %lld\n",ansT1,ansT2);
    return 0;
}
