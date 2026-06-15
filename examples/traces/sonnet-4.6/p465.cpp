#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M,R; cin>>N>>M>>R;
    vector<double> p(N);
    for(int i=0;i<N;i++){cin>>p[i];p[i]/=10000.0;}
    const double INF=1e18;
    vector<vector<double>> d(N,vector<double>(N,INF));
    for(int i=0;i<N;i++) d[i][i]=0;
    struct Edge{int u,v;double L;};
    vector<Edge> edges(M);
    for(int i=0;i<M;i++){int a,b,l;cin>>a>>b>>l;a--;b--;edges[i]={a,b,(double)l};d[a][b]=min(d[a][b],(double)l);d[b][a]=min(d[b][a],(double)l);}
    for(int k=0;k<N;k++)for(int i=0;i<N;i++)for(int j=0;j<N;j++)if(d[i][k]<INF&&d[k][j]<INF)d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)if(d[i][j]>=INF){cout<<-1<<endl;return 0;}
    auto eval=[&](int u,int v,double L,double t)->double{double res=0;for(int c=0;c<N;c++){double du=t+d[u][c],dv=(L-t)+d[v][c];res+=p[c]*min(du,dv);}return res;};
    double best=INF;bool found=false;
    for(auto&e:edges){int u=e.u,v=e.v;double L=e.L;if(L<2.0*R)continue;found=true;vector<double> cands={(double)R,L-(double)R};for(int c=0;c<N;c++){double t=(L+d[v][c]-d[u][c])/2.0;if(t>=(double)R&&t<=L-(double)R)cands.push_back(t);}for(double t:cands)best=min(best,eval(u,v,L,t));}
    if(R==0){for(int i=0;i<N;i++){found=true;double res=0;for(int c=0;c<N;c++)res+=p[c]*d[i][c];best=min(best,res);}}
    if(!found){cout<<-1<<endl;return 0;}
    cout<<fixed<<setprecision(5)<<best<<endl;
}
