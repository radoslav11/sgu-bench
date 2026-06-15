#include <bits/stdc++.h>
using namespace std;

struct P{ long long x,y; };
int n;
vector<P> pts;
const long long BIG = LLONG_MAX/4;

bool can2(vector<P>&a, long long d){
    int m=a.size();
    if(m<=1) return true;
    // vertical: sort by x
    sort(a.begin(),a.end(),[](const P&p,const P&q){return p.x<q.x;});
    {
        long long mnx=BIG,mxx=-BIG,mny=BIG,mxy=-BIG;
        int L=0;
        for(int i=0;i<m;i++){
            mnx=min(mnx,a[i].x);mxx=max(mxx,a[i].x);mny=min(mny,a[i].y);mxy=max(mxy,a[i].y);
            if(mxx-mnx<=d && mxy-mny<=d) L=i+1; else break;
        }
        long long Mnx=BIG,Mxx=-BIG,Mny=BIG,Mxy=-BIG;
        int S=m;
        for(int i=m-1;i>=0;i--){
            Mnx=min(Mnx,a[i].x);Mxx=max(Mxx,a[i].x);Mny=min(Mny,a[i].y);Mxy=max(Mxy,a[i].y);
            if(Mxx-Mnx<=d && Mxy-Mny<=d) S=i; else break;
        }
        if(S<=L) return true;
    }
    // horizontal: sort by y
    sort(a.begin(),a.end(),[](const P&p,const P&q){return p.y<q.y;});
    {
        long long mnx=BIG,mxx=-BIG,mny=BIG,mxy=-BIG;
        int L=0;
        for(int i=0;i<m;i++){
            mnx=min(mnx,a[i].x);mxx=max(mxx,a[i].x);mny=min(mny,a[i].y);mxy=max(mxy,a[i].y);
            if(mxx-mnx<=d && mxy-mny<=d) L=i+1; else break;
        }
        long long Mnx=BIG,Mxx=-BIG,Mny=BIG,Mxy=-BIG;
        int S=m;
        for(int i=m-1;i>=0;i--){
            Mnx=min(Mnx,a[i].x);Mxx=max(Mxx,a[i].x);Mny=min(Mny,a[i].y);Mxy=max(Mxy,a[i].y);
            if(Mxx-Mnx<=d && Mxy-Mny<=d) S=i; else break;
        }
        if(S<=L) return true;
    }
    return false;
}

bool can3(long long d){
    int m=pts.size();
    // orientation by x
    {
        vector<P> a=pts;
        sort(a.begin(),a.end(),[](const P&p,const P&q){return p.x<q.x;});
        // longest prefix can1
        long long mnx=BIG,mxx=-BIG,mny=BIG,mxy=-BIG;
        int L=0;
        for(int i=0;i<m;i++){
            mnx=min(mnx,a[i].x);mxx=max(mxx,a[i].x);mny=min(mny,a[i].y);mxy=max(mxy,a[i].y);
            if(mxx-mnx<=d && mxy-mny<=d) L=i+1; else break;
        }
        {
            vector<P> right(a.begin()+L, a.end());
            if(can2(right,d)) return true;
        }
        // longest suffix can1
        long long Mnx=BIG,Mxx=-BIG,Mny=BIG,Mxy=-BIG;
        int S=m;
        for(int i=m-1;i>=0;i--){
            Mnx=min(Mnx,a[i].x);Mxx=max(Mxx,a[i].x);Mny=min(Mny,a[i].y);Mxy=max(Mxy,a[i].y);
            if(Mxx-Mnx<=d && Mxy-Mny<=d) S=i; else break;
        }
        {
            vector<P> left(a.begin(), a.begin()+S);
            if(can2(left,d)) return true;
        }
    }
    // orientation by y
    {
        vector<P> a=pts;
        sort(a.begin(),a.end(),[](const P&p,const P&q){return p.y<q.y;});
        long long mnx=BIG,mxx=-BIG,mny=BIG,mxy=-BIG;
        int L=0;
        for(int i=0;i<m;i++){
            mnx=min(mnx,a[i].x);mxx=max(mxx,a[i].x);mny=min(mny,a[i].y);mxy=max(mxy,a[i].y);
            if(mxx-mnx<=d && mxy-mny<=d) L=i+1; else break;
        }
        {
            vector<P> right(a.begin()+L, a.end());
            if(can2(right,d)) return true;
        }
        long long Mnx=BIG,Mxx=-BIG,Mny=BIG,Mxy=-BIG;
        int S=m;
        for(int i=m-1;i>=0;i--){
            Mnx=min(Mnx,a[i].x);Mxx=max(Mxx,a[i].x);Mny=min(Mny,a[i].y);Mxy=max(Mxy,a[i].y);
            if(Mxx-Mnx<=d && Mxy-Mny<=d) S=i; else break;
        }
        {
            vector<P> left(a.begin(), a.begin()+S);
            if(can2(left,d)) return true;
        }
    }
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    if(!(cin>>n)) return 0;
    pts.resize(n);
    for(int i=0;i<n;i++) cin>>pts[i].x>>pts[i].y;
    long long lo=0, hi=2000000000LL;
    while(lo<hi){
        long long mid=(lo+hi)/2;
        if(can3(mid)) hi=mid; else lo=mid+1;
    }
    cout<<lo<<"\n";
    return 0;
}
