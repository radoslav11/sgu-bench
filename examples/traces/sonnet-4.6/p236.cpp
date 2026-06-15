#include <bits/stdc++.h>
using namespace std;
const double NEG_INF = -1e15;
int N, M;
double C[55][55], T_[55][55];
bool hasEdge[55][55];
double dist[55][55];
int nxt[55][55];

bool solve(double lam, vector<int>& cycle) {
    for(int i=1;i<=N;i++) for(int j=1;j<=N;j++){
        if(i==j){dist[i][j]=0;nxt[i][j]=i;}
        else if(hasEdge[i][j]){dist[i][j]=C[i][j]-lam*T_[i][j];nxt[i][j]=j;}
        else{dist[i][j]=NEG_INF;nxt[i][j]=-1;}
    }
    for(int k=1;k<=N;k++) for(int i=1;i<=N;i++) for(int j=1;j<=N;j++){
        if(dist[i][k]>NEG_INF/2&&dist[k][j]>NEG_INF/2&&dist[i][k]+dist[k][j]>dist[i][j]+1e-12){
            dist[i][j]=dist[i][k]+dist[k][j];
            nxt[i][j]=nxt[i][k];
        }
    }
    for(int i=1;i<=N;i++) if(dist[i][i]>1e-9){
        cycle.clear();
        cycle.push_back(i);
        int cur=nxt[i][i];
        int steps=0;
        while(cur!=i&&steps<N){
            cycle.push_back(cur);
            cur=nxt[cur][i];
            steps++;
        }
        return true;
    }
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin>>N>>M;
    memset(hasEdge,false,sizeof(hasEdge));
    for(int i=0;i<M;i++){
        int a,b,c,t;cin>>a>>b>>c>>t;
        hasEdge[a][b]=true;C[a][b]=c;T_[a][b]=t;
    }
    double lo=0,hi=100;
    vector<int> ans;
    for(int iter=0;iter<200;iter++){
        double mid=(lo+hi)/2;
        vector<int> cycle;
        if(solve(mid,cycle)){lo=mid;ans=cycle;}
        else hi=mid;
    }
    if(ans.empty()) cout<<0<<endl;
    else{
        cout<<ans.size()<<"\n";
        for(int x:ans) cout<<x<<" ";
        cout<<"\n";
    }
}
