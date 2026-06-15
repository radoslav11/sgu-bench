#include <bits/stdc++.h>
using namespace std;
struct Edge{int to,rev,cap;};
vector<Edge>G[16000];
int lv[16000],it[16000];
void ae(int u,int v,int c){G[u].push_back({v,(int)G[v].size(),c});G[v].push_back({u,(int)G[u].size()-1,0});}
bool bfs(int s,int t,int V){fill(lv,lv+V,-1);queue<int>q;lv[s]=0;q.push(s);while(!q.empty()){int v=q.front();q.pop();for(auto&e:G[v])if(e.cap>0&&lv[e.to]<0){lv[e.to]=lv[v]+1;q.push(e.to);}}return lv[t]>=0;}
int dfs(int v,int t,int f){if(v==t)return f;for(int&i=it[v];i<(int)G[v].size();i++){auto&e=G[v][i];if(e.cap>0&&lv[v]<lv[e.to]){int d=dfs(e.to,t,min(f,e.cap));if(d>0){e.cap-=d;G[e.to][e.rev].cap+=d;return d;}}}return 0;}
int mflow(int s,int t,int V){int r=0;while(bfs(s,t,V)){fill(it,it+V,0);int d;while((d=dfs(s,t,1<<30))>0)r+=d;}return r;}
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int N,M,D,W;
    cin>>N>>M>>D>>W;
    vector<int>X(N),Y(N),C(N);
    for(int i=0;i<N;i++)cin>>X[i]>>Y[i]>>C[i];
    long long DD=(long long)D*D;
    auto d2=[&](int i,int j)->long long{return(long long)(X[i]-X[j])*(X[i]-X[j])+(long long)(Y[i]-Y[j])*(Y[i]-Y[j]);};
    auto fromStart=[&](int i)->bool{return(long long)Y[i]*Y[i]<=DD;};
    auto toEnd=[&](int i)->bool{return(long long)(W-Y[i])*(W-Y[i])<=DD;};
    if((long long)W*W<=DD){cout<<1<<endl;return 0;}
    for(int T=1;T<=200;T++){
        int V=2+2*N*T;
        for(int i=0;i<V;i++)G[i].clear();
        int S=0,SK=1;
        auto IN=[&](int i,int t){return 2+2*((t-1)*N+i);};
        auto OUT=[&](int i,int t){return 3+2*((t-1)*N+i);};
        for(int t=1;t<=T;t++)
            for(int i=0;i<N;i++){
                ae(IN(i,t),OUT(i,t),C[i]);
                if(fromStart(i))ae(S,IN(i,t),M);
                if(toEnd(i))ae(OUT(i,t),SK,M);
            }
        for(int t=1;t<T;t++)
            for(int i=0;i<N;i++)
                for(int j=0;j<N;j++)
                    if(d2(i,j)<=DD)
                        ae(OUT(i,t),IN(j,t+1),M);
        if(mflow(S,SK,V)>=M){
            cout<<T+1<<endl;
            return 0;
        }
    }
    cout<<"IMPOSSIBLE"<<endl;
}
