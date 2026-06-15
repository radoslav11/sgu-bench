#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int N;
struct BIT {
    vector<int> t;
    int n;
    void init(int n_){ n=n_; t.assign(n+1,0); }
    void upd(int i, int v){ for(; i<=n; i+=i&(-i)) t[i]+=v; }
    int qry(int i){ int s=0; for(; i>0; i-=i&(-i)) s+=t[i]; return s; }
};

int main(){
    int K;
    scanf("%d %d",&N,&K);
    vector<ll> ux(N), uy(N), wv(N);
    for(int i=0;i<N;i++){
        ll x,y,w;
        scanf("%lld %lld %lld",&x,&y,&w);
        ux[i]=x+y; uy[i]=x-y; wv[i]=w;
    }
    // compress v coords
    vector<ll> vs;
    vs.reserve(3*N);
    for(int i=0;i<N;i++){
        ll v=uy[i], w=wv[i];
        vs.push_back(v-w);
        vs.push_back(v+w+1);
        vs.push_back(v);
    }
    sort(vs.begin(), vs.end());
    vs.erase(unique(vs.begin(), vs.end()), vs.end());
    auto getV=[&](ll x)->int{
        return (int)(lower_bound(vs.begin(), vs.end(), x)-vs.begin())+1;
    };
    int M = vs.size();
    
    // events: (u, order, vidx, delta) for update; (u,1,vidx,hutidx) for query
    // store separately
    struct Ev { ll u; int order; int a; int b; }; // order 0 update: a=vidx, b=delta ; order1 query: a=vidx, b=hutidx
    vector<Ev> ev;
    ev.reserve(5*N);
    for(int i=0;i<N;i++){
        ll u=ux[i], v=uy[i], w=wv[i];
        ll uL=u-w, uRp=u+w+1;
        int vL=getV(v-w), vRp=getV(v+w+1);
        // at uL: +1 at vL, -1 at vRp
        ev.push_back({uL,0,vL,1});
        ev.push_back({uL,0,vRp,-1});
        // at uRp: -1 at vL, +1 at vRp
        ev.push_back({uRp,0,vL,-1});
        ev.push_back({uRp,0,vRp,1});
        // query
        int vi=getV(v);
        ev.push_back({u,1,vi,i});
    }
    sort(ev.begin(), ev.end(), [](const Ev&x,const Ev&y){
        if(x.u!=y.u) return x.u<y.u;
        return x.order<y.order;
    });
    
    BIT bit; bit.init(M);
    vector<int> result(N,0);
    for(auto &e: ev){
        if(e.order==0){
            bit.upd(e.a, e.b);
        } else {
            int cnt = bit.qry(e.a);
            result[e.b] = cnt - 1; // exclude itself
        }
    }
    
    vector<int> ans;
    for(int i=0;i<N;i++){
        if(result[i] >= K) ans.push_back(i+1);
    }
    printf("%d\n",(int)ans.size());
    for(int i=0;i<(int)ans.size();i++){
        printf("%d%c", ans[i], i+1<(int)ans.size()?' ':'\n');
    }
    if(ans.empty()) printf("\n");
    return 0;
}
