#include <bits/stdc++.h>
using namespace std;

int v, e;
bool adj[105][105];
bool comp[105][105];

int main(){
    scanf("%d %d",&v,&e);
    for(int i=0;i<e;i++){
        int a,b; scanf("%d %d",&a,&b); a--;b--;
        adj[a][b]=adj[b][a]=true;
    }
    if(v%3!=0){ printf("NO\n"); return 0; }
    for(int i=0;i<v;i++)for(int j=0;j<v;j++) comp[i][j]= (i!=j)&&!adj[i][j];
    
    // find triple for each vertex
    vector<array<int,3>> tri(v);
    bool ok=true;
    for(int u=0;u<v&&ok;u++){
        vector<int> nb;
        for(int j=0;j<v;j++) if(comp[u][j]) nb.push_back(j);
        int cnt=0, ea=-1, eb=-1;
        for(int x=0;x<(int)nb.size();x++)for(int y=x+1;y<(int)nb.size();y++){
            if(comp[nb[x]][nb[y]]){ cnt++; ea=nb[x]; eb=nb[y]; }
        }
        if(cnt!=1){ ok=false; break; }
        array<int,3> t={u,ea,eb};
        sort(t.begin(),t.end());
        tri[u]=t;
    }
    if(!ok){ printf("NO\n"); return 0; }
    // consistency
    for(int u=0;u<v&&ok;u++){
        auto t=tri[u];
        bool contains=false;
        for(int k=0;k<3;k++) if(t[k]==u) contains=true;
        if(!contains){ ok=false; break; }
        for(int k=0;k<3;k++){
            if(tri[t[k]]!=t){ ok=false; break; }
        }
    }
    if(!ok){ printf("NO\n"); return 0; }
    // assign clause ids
    vector<int> clause(v,-1);
    int cid=0;
    for(int u=0;u<v;u++){
        if(clause[u]==-1){
            auto t=tri[u];
            for(int k=0;k<3;k++){
                if(clause[t[k]]!=-1){ ok=false; }
                clause[t[k]]=cid;
            }
            cid++;
        }
    }
    if(!ok){ printf("NO\n"); return 0; }
    // each clause must have exactly 3 vertices
    {
        vector<int> cntc(cid,0);
        for(int u=0;u<v;u++) cntc[clause[u]]++;
        for(int c=0;c<cid;c++) if(cntc[c]!=3){ ok=false; }
        if(!ok){ printf("NO\n"); return 0; }
    }
    // each clause independent in original
    for(int i=0;i<v&&ok;i++)for(int j=i+1;j<v;j++){
        if(clause[i]==clause[j] && adj[i][j]){ ok=false; break; }
    }
    if(!ok){ printf("NO\n"); return 0; }
    
    // labeling via contradiction graph H: edges = diff clause && !adj
    vector<int> var(v,-1), sgn(v,-1);
    int vid=0;
    for(int s=0;s<v&&ok;s++){
        if(var[s]!=-1) continue;
        // BFS
        queue<int> q; q.push(s); var[s]=vid; sgn[s]=0;
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(int w=0;w<v;w++){
                if(w==u) continue;
                if(clause[u]!=clause[w] && !adj[u][w]){
                    // H edge
                    int wantS = sgn[u]^1;
                    if(var[w]==-1){ var[w]=vid; sgn[w]=wantS; q.push(w); }
                    else {
                        if(var[w]!=vid || sgn[w]!=wantS){ ok=false; }
                    }
                }
            }
        }
        vid++;
    }
    if(!ok){ printf("NO\n"); return 0; }
    
    // distinct variables per clause
    for(int i=0;i<v&&ok;i++)for(int j=i+1;j<v;j++){
        if(clause[i]==clause[j] && var[i]==var[j]){ ok=false; break; }
    }
    if(!ok){ printf("NO\n"); return 0; }
    
    // rebuild and compare
    for(int i=0;i<v&&ok;i++)for(int j=i+1;j<v;j++){
        bool wantEdge;
        if(clause[i]==clause[j]) wantEdge=false;
        else wantEdge = !(var[i]==var[j] && sgn[i]!=sgn[j]);
        if(wantEdge != adj[i][j]){ ok=false; break; }
    }
    
    printf("%s\n", ok?"YES":"NO");
    return 0;
}
