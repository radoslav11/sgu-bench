#include <bits/stdc++.h>
using namespace std;

int n, m;
inline int idx(int i,int j){return i*m+j;}

int main(){
    scanf("%d %d",&n,&m);
    int N=n*m;
    vector<int> val(N);
    for(int i=0;i<n;i++)for(int j=0;j<m;j++){
        int x; scanf("%d",&x); val[idx(i,j)]=x;
    }
    vector<int> deg(N,0), r(N), rc(N,0);
    vector<char> removed(N,0);
    // compute degrees
    int dx[4]={-1,1,0,0}, dy[4]={0,0,-1,1};
    for(int i=0;i<n;i++)for(int j=0;j<m;j++){
        int d=0;
        for(int k=0;k<4;k++){
            int ni=i+dx[k], nj=j+dy[k];
            if(ni>=0&&ni<n&&nj>=0&&nj<m) d++;
        }
        deg[idx(i,j)]=d;
        r[idx(i,j)]=val[idx(i,j)]-1;
    }
    queue<int> q;
    for(int v=0;v<N;v++){
        if(r[v]>deg[v]){
            // infeasible
            printf("No solution\n");
            return 0;
        }
        if(r[v]==0) q.push(v);
    }
    vector<int> removalOrder;
    removalOrder.reserve(N);
    while(!q.empty()){
        int v=q.front(); q.pop();
        if(removed[v]) continue;
        if(rc[v]!=r[v]) continue; // stale
        removed[v]=1;
        removalOrder.push_back(v);
        int i=v/m, j=v%m;
        for(int k=0;k<4;k++){
            int ni=i+dx[k], nj=j+dy[k];
            if(ni>=0&&ni<n&&nj>=0&&nj<m){
                int u=idx(ni,nj);
                if(removed[u]) continue;
                rc[u]++;
                if(rc[u]==r[u]) q.push(u);
            }
        }
    }
    if((int)removalOrder.size()!=N){
        printf("No solution\n");
        return 0;
    }
    // planting order = reverse of removal order (earliest first)
    // build output
    string out;
    out.reserve(N*8);
    char buf[32];
    for(int t=N-1;t>=0;t--){
        int v=removalOrder[t];
        int i=v/m, j=v%m;
        int len=sprintf(buf,"%d %d\n", i+1, j+1);
        out.append(buf,len);
    }
    fputs(out.c_str(), stdout);
    return 0;
}
