#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int main(){
    int n, m;
    if(scanf("%d %d",&n,&m)!=2) return 0;
    vector<int> ex(m+1), ey(m+1);
    vector<int> deg(n+1,0);
    for(int i=1;i<=m;i++){
        scanf("%d %d",&ex[i],&ey[i]);
        deg[ex[i]]++; deg[ey[i]]++;
    }
    
    int center = -1;
    if(n>=2){
        for(int v=1;v<=n;v++){
            if(deg[v]==n-1){ center=v; break; }
        }
    }
    // arrays
    vector<char> cl(n+1,0);        // center-leaf edge present
    vector<int> leaf_tri(n+1,-1);  // triangle id of leaf
    vector<int> partner(n+1,-1);   // partner leaf
    // triangles
    vector<char> ab;               // pair edge present
    vector<int> triA, triB;
    
    // edge classification
    vector<int> edgeKind(m+1,-1);  // 0 center-leaf, 1 pair
    vector<int> edgeLeaf(m+1,-1);
    vector<int> edgePair(m+1,-1);
    
    if(n>=3){
        // first process pair edges to assign triangles
        for(int i=1;i<=m;i++){
            int a=ex[i], b=ey[i];
            if(a!=center && b!=center){
                int p = ab.size();
                ab.push_back(1);
                triA.push_back(a);
                triB.push_back(b);
                leaf_tri[a]=p; leaf_tri[b]=p;
                partner[a]=b; partner[b]=a;
                edgeKind[i]=1; edgePair[i]=p;
            }
        }
        // then center edges
        for(int i=1;i<=m;i++){
            int a=ex[i], b=ey[i];
            if(a==center || b==center){
                int leaf = (a==center)? b : a;
                cl[leaf]=1;
                edgeKind[i]=0; edgeLeaf[i]=leaf;
            }
        }
    }
    
    // helper d(leaf): distance from leaf to center within its triangle
    auto dleaf = [&](int l)->int{
        int p = leaf_tri[l];
        if(p<0) return INF;
        int other = partner[l];
        bool e_cl = cl[l];
        bool e_co = cl[other];
        bool e_ab = ab[p];
        int d = INF;
        if(e_cl) d = 1;
        if(e_ab && e_co) d = min(d, 2);
        return d;
    };
    // distance same triangle between l and partner
    auto dsame = [&](int l)->int{
        int p = leaf_tri[l];
        int other = partner[l];
        bool e_cl = cl[l];
        bool e_co = cl[other];
        bool e_ab = ab[p];
        int res = INF;
        if(e_ab) res = 1;
        if(e_cl && e_co) res = min(res, 2);
        return res;
    };
    
    char cmd[20];
    string out;
    while(scanf("%s", cmd)==1){
        if(cmd[0]=='D'){
            int x; scanf("%d",&x);
            if(edgeKind[x]==0){
                cl[edgeLeaf[x]]=0;
            } else if(edgeKind[x]==1){
                ab[edgePair[x]]=0;
            }
        } else {
            int x,y; scanf("%d %d",&x,&y);
            int ans;
            if(x==y){
                ans=0;
            } else if(n<3){
                // n==1 or n==2 with center; only x!=y
                ans = INF; // shouldn't really happen for n==1 with x!=y
                if(n==2){
                    // two vertices, center exists; one is center one is leaf? 
                    // friendship graph not defined for n=2, just fallback
                    ans = INF;
                }
            } else if(x==center){
                ans = dleaf(y);
            } else if(y==center){
                ans = dleaf(x);
            } else {
                if(leaf_tri[x]==leaf_tri[y]){
                    ans = dsame(x);
                } else {
                    int d1 = dleaf(x), d2 = dleaf(y);
                    if(d1>=INF || d2>=INF) ans = INF;
                    else ans = d1+d2;
                }
            }
            if(ans>=INF) out += "-1\n";
            else { out += to_string(ans); out += '\n'; }
        }
    }
    fputs(out.c_str(), stdout);
    return 0;
}
