#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; double vw,vc;
    cin>>n>>vw>>vc;
    vector<double> x(n),y(n);
    for(int i=0;i<n;i++)cin>>x[i]>>y[i];
    set<double> ys;
    for(int i=0;i<n;i++)ys.insert(y[i]);
    struct P{double x,y;};
    vector<P> pts;
    auto addPoint=[&](double px,double py){pts.push_back({px,py});};
    for(int i=0;i<n;i++)addPoint(x[i],y[i]);
    for(double Y:ys){
        for(int i=0;i+1<n;i++){
            double y1=y[i],y2=y[i+1];
            if((y1-Y)*(y2-Y)<0){
                double t=(Y-y1)/(y2-y1);
                double px=x[i]+t*(x[i+1]-x[i]);
                addPoint(px,Y);
            }
        }
    }
    sort(pts.begin(),pts.end(),[](const P&a,const P&b){if(a.x!=b.x)return a.x<b.x;return a.y<b.y;});
    pts.erase(unique(pts.begin(),pts.end(),[](const P&a,const P&b){return fabs(a.x-b.x)<1e-9&&fabs(a.y-b.y)<1e-9;}),pts.end());
    int m=pts.size();
    auto surf=[&](double px)->double{
        for(int i=0;i+1<n;i++){
            if(px>=x[i]-1e-12&&px<=x[i+1]+1e-12){
                double t=(px-x[i])/(x[i+1]-x[i]);
                return y[i]+t*(y[i+1]-y[i]);
            }
        }
        return y[n-1];
    };
    vector<vector<pair<int,double>>> g(m);
    auto onSurf=[&](const P&p){return fabs(p.y-surf(p.x))<1e-6;};
    vector<int> sp;
    for(int i=0;i<m;i++)if(onSurf(pts[i]))sp.push_back(i);
    sort(sp.begin(),sp.end(),[&](int a,int b){return pts[a].x<pts[b].x;});
    for(int i=0;i+1<(int)sp.size();i++){
        int a=sp[i],b=sp[i+1];
        double d=hypot(pts[a].x-pts[b].x,pts[a].y-pts[b].y);
        g[a].push_back({b,d/vw});g[b].push_back({a,d/vw});
    }
    for(int i=0;i<m;i++)for(int j=i+1;j<m;j++){
        if(fabs(pts[i].y-pts[j].y)>1e-9)continue;
        double xl=min(pts[i].x,pts[j].x),xr=max(pts[i].x,pts[j].x);
        double Y=pts[i].y;
        bool ok=true;
        vector<double> bps={xl,xr};
        for(int k=0;k<n;k++)if(x[k]>xl+1e-9&&x[k]<xr-1e-9)bps.push_back(x[k]);
        sort(bps.begin(),bps.end());
        for(int k=0;k+1<(int)bps.size();k++){
            double mid=(bps[k]+bps[k+1])/2;
            if(surf(mid)<Y-1e-9){ok=false;break;}
        }
        if(ok){
            double d=xr-xl;
            g[i].push_back({j,d/vc});g[j].push_back({i,d/vc});
        }
    }
    int src=-1,dst=-1;
    for(int i=0;i<m;i++){
        if(fabs(pts[i].x-x[0])<1e-9&&fabs(pts[i].y-y[0])<1e-9)src=i;
        if(fabs(pts[i].x-x[n-1])<1e-9&&fabs(pts[i].y-y[n-1])<1e-9)dst=i;
    }
    vector<double> dist(m,1e18);dist[src]=0;
    priority_queue<pair<double,int>,vector<pair<double,int>>,greater<>> pq;
    pq.push({0,src});
    while(!pq.empty()){
        auto[d,u]=pq.top();pq.pop();
        if(d>dist[u]+1e-12)continue;
        for(auto[v,w]:g[u])if(dist[u]+w<dist[v]-1e-12){dist[v]=dist[u]+w;pq.push({dist[v],v});}
    }
    printf("%.6f\n",dist[dst]);
}
