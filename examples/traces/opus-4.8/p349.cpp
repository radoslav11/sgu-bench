#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> P;

int main(){
    int N,M;
    if(scanf("%d %d",&N,&M)!=2) return 0;
    vector<array<P,2>> segs(N+M);
    vector<P> dirs;
    dirs.reserve(2*(N+M));
    for(int i=0;i<N+M;i++){
        ll x1,y1,x2,y2;
        scanf("%lld %lld %lld %lld",&x1,&y1,&x2,&y2);
        P a={x1,y1}, b={x2,y2};
        segs[i]={a,b};
        dirs.push_back(a);
        dirs.push_back(b);
    }
    auto cross=[](const P&a,const P&b)->ll{ return a.first*b.second - a.second*b.first; };
    auto cmp=[&](const P&a,const P&b){ return cross(a,b)>0; };
    auto eq=[&](const P&a,const P&b){ return cross(a,b)==0; };
    sort(dirs.begin(),dirs.end(),cmp);
    dirs.erase(unique(dirs.begin(),dirs.end(),eq),dirs.end());
    auto getRank=[&](const P&v)->int{
        int lo=0, hi=(int)dirs.size()-1, res=0;
        while(lo<=hi){
            int mid=(lo+hi)/2;
            if(eq(dirs[mid],v)){res=mid;break;}
            if(cmp(dirs[mid],v)){lo=mid+1;}
            else hi=mid-1;
        }
        return res;
    };
    vector<pair<ll,ll>> wolves;
    vector<pair<ll,ll>> sheep;
    for(int i=0;i<N+M;i++){
        int r1=getRank(segs[i][0]);
        int r2=getRank(segs[i][1]);
        ll a=2LL*min(r1,r2);
        ll b=2LL*max(r1,r2);
        if(i<N) wolves.push_back({a,b});
        else sheep.push_back({a,b});
    }
    sort(sheep.begin(),sheep.end());
    vector<pair<ll,ll>> forb;
    for(auto&s:sheep){
        if(!forb.empty() && s.first<=forb.back().second+1){
            forb.back().second=max(forb.back().second,s.second);
        } else forb.push_back(s);
    }
    auto rightmostAllowed=[&](ll b)->ll{
        int lo=0,hi=(int)forb.size()-1, idx=-1;
        while(lo<=hi){
            int mid=(lo+hi)/2;
            if(forb[mid].first<=b){idx=mid;lo=mid+1;} else hi=mid-1;
        }
        if(idx>=0 && forb[idx].second>=b){
            return forb[idx].first-1;
        }
        return b;
    };
    sort(wolves.begin(),wolves.end(),[](const pair<ll,ll>&A,const pair<ll,ll>&B){
        if(A.second!=B.second) return A.second<B.second;
        return A.first<B.first;
    });
    ll lastP=LLONG_MIN;
    int shots=0;
    bool ok=true;
    for(auto&w:wolves){
        ll a=w.first,b=w.second;
        if(lastP>=a && lastP<=b) continue;
        ll p=rightmostAllowed(b);
        if(p<a){ ok=false; break; }
        lastP=p;
        shots++;
    }
    if(!ok) printf("No solution\n");
    else printf("%d\n",shots);
    return 0;
}
