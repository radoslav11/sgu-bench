#include<bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld INF=1e18;
int main(){
    int V,G,n;
    cin>>V>>G>>n;
    struct Stone{int x1,x2,t;};
    vector<Stone> stones(n);
    for(int i=0;i<n;i++) cin>>stones[i].x1>>stones[i].x2>>stones[i].t;
    
    // Nodes: 0=start(pos=0,t=0), 1..2n=stone endpoints(fixed times), 2n+1=goal(pos=G)
    int N=2*n+2;
    vector<ld> npos(N),ntime(N,-1);
    npos[0]=0; ntime[0]=0;
    for(int i=0;i<n;i++){
        npos[2*i+1]=stones[i].x1; ntime[2*i+1]=stones[i].t;
        npos[2*i+2]=stones[i].x2; ntime[2*i+2]=stones[i].t;
    }
    npos[2*n+1]=G;
    
    auto check=[&](ld px,ld pt,ld qx,ld qt)->bool{
        for(int k=0;k<n;k++){
            ld tk=stones[k].t;
            if(tk<pt-1e-12||tk>qt+1e-12) continue;
            ld xk=(qt-pt<1e-15)?px:px+(qx-px)*(tk-pt)/(qt-pt);
            if(xk>(ld)stones[k].x1+1e-12 && xk<(ld)stones[k].x2-1e-12) return false;
        }
        return true;
    };
    
    vector<ld> dist(N,INF);
    vector<int> prev(N,-1);
    priority_queue<pair<ld,int>,vector<pair<ld,int>>,greater<>> pq;
    dist[0]=0; pq.push({0,0});
    
    while(!pq.empty()){
        auto[d,u]=pq.top();pq.pop();
        if(d>dist[u]+1e-12) continue;
        // depart from u at time dep_u
        ld dep_u=(u==0)?0:(u==2*n+1?dist[u]:ntime[u]);
        ld pu=npos[u];
        
        for(int v=1;v<N;v++){
            if(v==u) continue;
            ld pv=npos[v];
            ld arr_v;
            if(v==2*n+1){
                arr_v=dep_u+abs(pv-pu)/V;
            } else {
                ld tv=ntime[v];
                if(tv<dep_u-1e-12) continue;
                if(tv-dep_u<abs(pv-pu)/V-1e-12) continue;
                arr_v=tv;
            }
            if(!check(pu,dep_u,pv,arr_v)) continue;
            if(arr_v<dist[v]-1e-12){
                dist[v]=arr_v;
                prev[v]=u;
                pq.push({arr_v,v});
            }
        }
    }
    
    if(dist[2*n+1]>=INF/2){cout<<-1;return 0;}
    
    vector<int> path;
    for(int cur=2*n+1;cur!=-1;cur=prev[cur]) path.push_back(cur);
    reverse(path.begin(),path.end());
    
    vector<pair<ld,ld>> inst;
    for(int i=0;i+1<(int)path.size();i++){
        int u=path[i],v=path[i+1];
        ld dep_u=(u==0)?0:ntime[u];
        ld arr_v=(v==2*n+1)?dist[v]:ntime[v];
        ld dt=arr_v-dep_u;
        if(dt<1e-15) continue;
        ld spd=(npos[v]-npos[u])/dt;
        inst.push_back({spd,dt});
    }
    
    // merge
    vector<pair<ld,ld>> mg;
    for(auto&p:inst){
        if(!mg.empty()&&abs(mg.back().first-p.first)<1e-12) mg.back().second+=p.second;
        else mg.push_back(p);
    }
    
    cout<<mg.size()<<"\n"<<fixed<<setprecision(15);
    for(auto&p:mg) cout<<p.first<<" "<<p.second<<"\n";
}
