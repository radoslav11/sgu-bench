#include <bits/stdc++.h>
using namespace std;
int main(){
    double xa,ya,za,xb,yb,zb;
    cin>>xa>>ya>>za>>xb>>yb>>zb;
    int NT=360, NZ=160;
    auto P=[&](int it,int iz){
        double z=-1.0+2.0*iz/(NZ-1);
        double r=sqrt(1+z*z);
        double th=2*M_PI*it/NT;
        return array<double,3>{r*cos(th),r*sin(th),z};
    };
    auto findN=[&](double x,double y,double z){
        int bi=0,bj=0; double bd=1e18;
        for(int it=0;it<NT;it++)for(int iz=0;iz<NZ;iz++){
            auto p=P(it,iz);
            double d=(p[0]-x)*(p[0]-x)+(p[1]-y)*(p[1]-y)+(p[2]-z)*(p[2]-z);
            if(d<bd){bd=d;bi=it;bj=iz;}
        }
        return bi*NZ+bj;
    };
    int src=findN(xa,ya,za), dst=findN(xb,yb,zb);
    int N=NT*NZ;
    vector<double> dist(N,1e18);
    priority_queue<pair<double,int>,vector<pair<double,int>>,greater<>> pq;
    dist[src]=0; pq.push({0,src});
    int dt[]={-1,-1,-1,0,0,1,1,1};
    int dz[]={-1,0,1,-1,1,-1,0,1};
    while(!pq.empty()){
        auto[d,u]=pq.top();pq.pop();
        if(d>dist[u])continue;
        int it=u/NZ, iz=u%NZ;
        auto pu=P(it,iz);
        for(int k=0;k<8;k++){
            int nt=(it+dt[k]+NT)%NT, nz=iz+dz[k];
            if(nz<0||nz>=NZ)continue;
            int v=nt*NZ+nz;
            auto pv=P(nt,nz);
            double w=sqrt((pu[0]-pv[0])*(pu[0]-pv[0])+(pu[1]-pv[1])*(pu[1]-pv[1])+(pu[2]-pv[2])*(pu[2]-pv[2]));
            if(dist[u]+w<dist[v]){dist[v]=dist[u]+w;pq.push({dist[v],v});}
        }
    }
    printf("%.10f\n",dist[dst]);
}
