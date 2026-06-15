#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M; long long R;
    cin>>N>>M>>R;
    vector<double> p(N+1);
    for(int i=1;i<=N;i++){ cin>>p[i]; }
    const double INF=1e18;
    vector<vector<double>> d(N+1, vector<double>(N+1, INF));
    for(int i=1;i<=N;i++) d[i][i]=0;
    vector<array<long long,3>> edges;
    for(int i=0;i<M;i++){
        long long a,b,l; cin>>a>>b>>l;
        edges.push_back({a,b,l});
        d[a][b]=min(d[a][b],(double)l);
        d[b][a]=min(d[b][a],(double)l);
    }
    for(int k=1;k<=N;k++)for(int i=1;i<=N;i++)for(int j=1;j<=N;j++)
        if(d[i][k]+d[k][j]<d[i][j]) d[i][j]=d[i][k]+d[k][j];
    bool conn=true;
    for(int i=1;i<=N;i++)for(int j=1;j<=N;j++) if(d[i][j]>=INF/2) conn=false;
    if(!conn){ cout<<-1<<endl; return 0; }
    
    double best=INF;
    if(N==1){
        printf("%.5f\n", 0.0);
        return 0;
    }
    auto evalPos=[&](int a,int b,double L,double x)->double{
        double s=0;
        for(int i=1;i<=N;i++){
            double da=x+d[a][i];
            double db=(L-x)+d[b][i];
            s+=p[i]*min(da,db);
        }
        return s;
    };
    
    for(auto&e:edges){
        long long a=e[0],b=e[1],L=e[2];
        if(L < 2*R) continue;
        double lo=R, hi=L-R;
        vector<double> cands;
        cands.push_back(lo);
        cands.push_back(hi);
        for(int i=1;i<=N;i++){
            double x=(L + d[b][i] - d[a][i])/2.0;
            if(x>=lo && x<=hi) cands.push_back(x);
        }
        for(double x:cands){
            double c=evalPos(a,b,(double)L,x);
            if(c<best) best=c;
        }
    }
    
    if(best>=INF/2){ cout<<-1<<endl; return 0; }
    best/=10000.0;
    printf("%.5f\n", best);
    return 0;
}
