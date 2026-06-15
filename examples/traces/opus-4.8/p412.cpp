#include <bits/stdc++.h>
using namespace std;
int N,M;
vector<double> vx,vy,vang;
struct PT{double x,y;};
PT hit(double th){
    double dx=cos(th),dy=sin(th);
    int n=N;
    int idx = upper_bound(vang.begin(),vang.end(),th)-vang.begin();
    int a=(idx-1+n)%n, b=idx%n;
    double ax=vx[a],ay=vy[a],bx=vx[b],by=vy[b];
    double ex=bx-ax,ey=by-ay;
    double denom=dx*ey-dy*ex;
    double t=(ax*ey-ay*ex)/denom;
    return {t*dx,t*dy};
}
int main(){
    scanf("%d %d",&N,&M);
    vx.resize(N);vy.resize(N);vang.resize(N);
    vector<pair<double,int>> tmp(N);
    vector<double> rx(N),ry(N);
    for(int i=0;i<N;i++){double x,y;scanf("%lf %lf",&x,&y);rx[i]=x;ry[i]=y;tmp[i]={atan2(y,x),i};}
    sort(tmp.begin(),tmp.end());
    for(int i=0;i<N;i++){int id=tmp[i].second;vx[i]=rx[id];vy[i]=ry[id];vang[i]=tmp[i].first;}
    vector<pair<double,double>> iv;
    for(int j=0;j<M;j++){
        double x1,y1,x2,y2;scanf("%lf %lf %lf %lf",&x1,&y1,&x2,&y2);
        double a1=atan2(y1,x1),a2=atan2(y2,x2);
        double lo=min(a1,a2),hi=max(a1,a2);
        if(hi-lo>M_PI){iv.push_back({-M_PI,lo});iv.push_back({hi,M_PI});}
        else iv.push_back({lo,hi});
    }
    sort(iv.begin(),iv.end());
    double total=0;
    double cl=-1e18,cr=-1e18;
    auto proc=[&](double a,double b){
        if(b-a<1e-15)return;
        PT p=hit(a);
        double px=p.x,py=p.y;
        int idx=upper_bound(vang.begin(),vang.end(),a)-vang.begin();
        while(idx<N && vang[idx]<b){
            total+=hypot(vx[idx]-px,vy[idx]-py);px=vx[idx];py=vy[idx];idx++;
        }
        PT q=hit(b);
        total+=hypot(q.x-px,q.y-py);
    };
    for(auto&p:iv){
        if(p.first>cr){if(cr>-1e17)proc(cl,cr);cl=p.first;cr=p.second;}
        else cr=max(cr,p.second);
    }
    if(cr>-1e17)proc(cl,cr);
    printf("%.12f\n",total);
}
