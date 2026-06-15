#include <bits/stdc++.h>
using namespace std;
int N,M;
vector<int> adj0[1001];
vector<int> adjFull[1001];
int idxArr[1001], low[1001], comp[1001], compSize[1001];
bool onStk[1001];
int idxCnt=1, compCnt=0;
vector<int> stk;

void tarjan(int u){
    idxArr[u]=low[u]=idxCnt++;
    stk.push_back(u); onStk[u]=true;
    for(int v: adj0[u]){
        if(idxArr[v]==0){ tarjan(v); low[u]=min(low[u],low[v]); }
        else if(onStk[v]) low[u]=min(low[u],idxArr[v]);
    }
    if(low[u]==idxArr[u]){
        int c=++compCnt; int sz=0;
        while(true){ int w=stk.back(); stk.pop_back(); onStk[w]=false; comp[w]=c; sz++; if(w==u)break;}
        compSize[c]=sz;
    }
}

int main(){
    scanf("%d %d",&N,&M);
    vector<bool> seed(N+1,false);
    for(int i=0;i<M;i++){
        int a,b; long long w; scanf("%d %d %lld",&a,&b,&w);
        adjFull[a].push_back(b);
        if(w==0){
            adj0[a].push_back(b);
            if(a==b) seed[a]=true;
        }
    }
    for(int i=1;i<=N;i++) if(idxArr[i]==0) tarjan(i);
    for(int i=1;i<=N;i++) if(compSize[comp[i]]>1) seed[i]=true;
    vector<bool> dead(N+1,false);
    queue<int> q;
    for(int i=1;i<=N;i++) if(seed[i]){dead[i]=true;q.push(i);}
    while(!q.empty()){
        int u=q.front();q.pop();
        for(int v:adjFull[u]) if(!dead[v]){dead[v]=true;q.push(v);}
    }
    for(int i=1;i<=N;i++) printf("%d\n", dead[i]?0:1);
    return 0;
}
