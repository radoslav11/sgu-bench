#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    vector<vector<int>> l(n+1, vector<int>(n+1));
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            scanf("%d",&l[i][j]);
    int k;
    scanf("%d",&k);
    vector<int> r(k+1);
    for(int i=1;i<=k;i++) scanf("%d",&r[i]);
    
    vector<char> cur(n+1,0), nxt(n+1,0);
    cur[1]=1;
    for(int step=1;step<=k;step++){
        fill(nxt.begin(),nxt.end(),0);
        int len=r[step];
        for(int u=1;u<=n;u++){
            if(!cur[u]) continue;
            for(int v=1;v<=n;v++){
                if(l[u][v]==len && l[u][v]!=0){
                    nxt[v]=1;
                }
            }
        }
        cur=nxt;
    }
    
    vector<int> res;
    for(int v=1;v<=n;v++) if(cur[v]) res.push_back(v);
    
    printf("%d\n",(int)res.size());
    for(size_t i=0;i<res.size();i++){
        printf("%d%c",res[i], i+1<res.size()?' ':'\n');
    }
    if(res.empty()) printf("\n");
    return 0;
}
