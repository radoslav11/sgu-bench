#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

struct Hull {
    map<ll,ll> m;
    int sign;
    lll under=0; // sum of (x2-x1)*(y1+y2) over consecutive edges
    
    lll edge(map<ll,ll>::iterator a, map<ll,ll>::iterator b){
        return (lll)(b->first-a->first)*(a->second+b->second);
    }
    bool bad(map<ll,ll>::iterator it){
        auto a=prev(it),b=it,c=next(it);
        lll cr=(lll)(b->first-a->first)*(c->second-a->second)-(lll)(b->second-a->second)*(c->first-a->first);
        return sign==1? cr>=0 : cr<=0;
    }
    bool inside(ll x,ll y){
        auto it=m.lower_bound(x);
        if(it==m.end())return false;
        if(it->first==x)return sign==1? y>=it->second : y<=it->second;
        if(it==m.begin())return false;
        auto a=prev(it),b=it;
        lll cr=(lll)(b->first-a->first)*(y-a->second)-(lll)(b->second-a->second)*(x-a->first);
        return sign==1? cr>=0 : cr<=0;
    }
    void erasePt(map<ll,ll>::iterator it){
        auto a=prev(it),c=next(it);
        under-=edge(a,it); under-=edge(it,c);
        ll x=it->first; m.erase(it);
        auto na=m.find(a->first), nc=m.find(c->first);
        under+=edge(na,nc);
    }
    void insertPt(ll x,ll y){
        // assume not present, will be between neighbors
        auto it=m.lower_bound(x);
        if(it!=m.end() && it!=m.begin()){
            auto a=prev(it); under-=edge(a,it);
        }
        m[x]=y; it=m.find(x);
        if(it!=m.begin()) under+=edge(prev(it),it);
        if(next(it)!=m.end()) under+=edge(it,next(it));
    }
    void insert(ll x,ll y){
        if(inside(x,y))return;
        auto ex=m.find(x);
        if(ex!=m.end()){
            // same x, replace if better
            if(sign==1){ if(y>=ex->second) return; } else { if(y<=ex->second) return; }
            erasePt2(ex);
        }
        insertPt(x,y);
        auto it=m.find(x);
        while(it!=m.begin()){
            auto l=prev(it);
            if(l==m.begin())break;
            if(bad(l)) erasePt(l); else break;
        }
        while(next(it)!=m.end()){
            auto r=next(it);
            if(next(r)==m.end())break;
            if(bad(r)) erasePt(r); else break;
        }
    }
    void erasePt2(map<ll,ll>::iterator it){
        if(it==m.begin()||next(it)==m.end()){
            auto a=(it!=m.begin())?prev(it):m.end();
            auto c=next(it);
            if(it!=m.begin()) under-=edge(prev(it),it);
            if(c!=m.end()) under-=edge(it,c);
            m.erase(it);
            return;
        }
        erasePt(it);
    }
};

int main(){
    ll x1,y1,x2,y2,x3,y3;
    scanf("%lld %lld %lld %lld %lld %lld",&x1,&y1,&x2,&y2,&x3,&y3);
    Hull up,lo; up.sign=-1; lo.sign=1;
    auto add=[&](ll x,ll y){ up.insert(x,y); lo.insert(x,y); };
    add(x1,y1);add(x2,y2);add(x3,y3);
    int n; scanf("%d",&n);
    while(n--){
        ll x,y; scanf("%lld %lld",&x,&y);
        add(x,y);
        lll a = up.under - lo.under;
        if(a<0)a=-a;
        // print __int128
        if(a==0){printf("0\n");continue;}
        char buf[64]; int p=0;
        while(a>0){buf[p++]='0'+(int)(a%10);a/=10;}
        while(p)putchar(buf[--p]);
        putchar('\n');
    }
}
