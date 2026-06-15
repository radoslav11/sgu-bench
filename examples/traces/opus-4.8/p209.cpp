#include <bits/stdc++.h>
using namespace std;
typedef double D;
const D EPS=1e-9;

int main(){
    int n; 
    if(!(cin>>n)) return 0;
    vector<array<D,4>> L(n);
    for(int i=0;i<n;i++) cin>>L[i][0]>>L[i][1]>>L[i][2]>>L[i][3];
    // lines as point+dir
    vector<D> ax(n),ay(n),dx(n),dy(n);
    for(int i=0;i<n;i++){ax[i]=L[i][0];ay[i]=L[i][1];dx[i]=L[i][2]-L[i][0];dy[i]=L[i][3]-L[i][1];}
    // collect intersection points
    vector<pair<D,D>> pts;
    auto addp=[&](D x,D y){pts.push_back({x,y});};
    for(int i=0;i<n;i++)for(int j=i+1;j<n;j++){
        D det=dx[i]*dy[j]-dy[i]*dx[j];
        if(fabs(det)<EPS) continue;
        D t=((ax[j]-ax[i])*dy[j]-(ay[j]-ay[i])*dx[j])/det;
        addp(ax[i]+dx[i]*t, ay[i]+dy[i]*t);
    }
    // dedup
    vector<pair<D,D>> V;
    auto findv=[&](D x,D y)->int{
        for(int k=0;k<(int)V.size();k++) if(fabs(V[k].first-x)<1e-7&&fabs(V[k].second-y)<1e-7) return k;
        V.push_back({x,y}); return (int)V.size()-1;
    };
    for(auto&p:pts) findv(p.first,p.second);
    int m=V.size();
    // for each line, collect vertices on it, sort, make edges
    set<pair<int,int>> edgeset;
    for(int i=0;i<n;i++){
        vector<pair<D,int>> on;
        for(int k=0;k<m;k++){
            // check if V[k] on line i: cross of (V[k]-a) with dir ==0
            D cx=(V[k].first-ax[i])*dy[i]-(V[k].second-ay[i])*dx[i];
            if(fabs(cx)<1e-6){
                D t=(V[k].first-ax[i])*dx[i]+(V[k].second-ay[i])*dy[i];
                on.push_back({t,k});
            }
        }
        sort(on.begin(),on.end());
        for(int k=0;k+1<(int)on.size();k++){
            int u=on[k].second,v=on[k+1].second;
            if(u!=v){edgeset.insert({u,v});edgeset.insert({v,u});}
        }
    }
    // build adjacency with angle sorting
    vector<vector<int>> adj(m);
    map<pair<int,int>,int> heid;
    vector<int> from,to;
    for(auto&e:edgeset){
        heid[e]=from.size();
        from.push_back(e.first);to.push_back(e.second);
    }
    int E=from.size();
    // for each vertex sort outgoing edges by angle
    vector<vector<int>> out(m); // half-edge ids outgoing
    for(int i=0;i<E;i++) out[from[i]].push_back(i);
    vector<int> posInSorted(E);
    for(int v=0;v<m;v++){
        auto&o=out[v];
        sort(o.begin(),o.end(),[&](int a,int b){
            D aa=atan2(V[to[a]].second-V[v].second,V[to[a]].first-V[v].first);
            D bb=atan2(V[to[b]].second-V[v].second,V[to[b]].first-V[v].first);
            return aa<bb;
        });
        for(int k=0;k<(int)o.size();k++) posInSorted[o[k]]=k;
    }
    // next half-edge for face: incoming u->v, find reverse v->u in out[v], take previous (CW)
    vector<int> nxt(E);
    for(int i=0;i<E;i++){
        int v=to[i];
        int rev=heid[{to[i],from[i]}];
        int p=posInSorted[rev];
        int sz=out[v].size();
        int np=(p-1+sz)%sz;
        nxt[i]=out[v][np];
    }
    vector<char> used(E,0);
    vector<D> areas;
    for(int i=0;i<E;i++){
        if(used[i])continue;
        int cur=i; D area=0;
        while(!used[cur]){
            used[cur]=1;
            int a=from[cur],b=to[cur];
            area+=V[a].first*V[b].second-V[b].first*V[a].second;
            cur=nxt[cur];
        }
        area/=2;
        if(area>1e-8) areas.push_back(area);
    }
    sort(areas.begin(),areas.end());
    cout<<areas.size()<<"\n";
    cout<<fixed<<setprecision(4);
    for(D a:areas) cout<<a<<"\n";
    return 0;
}
