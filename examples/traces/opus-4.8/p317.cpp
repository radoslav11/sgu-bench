#include <bits/stdc++.h>
using namespace std;
int main(){
  long long B; int N;
  scanf("%lld %d",&B,&N);
  vector<long long> X(N);
  vector<vector<long long>> hd(N), hv(N);
  for(int i=0;i<N;i++){
    long long x; int m;
    scanf("%lld %d",&x,&m);
    X[i]=x;
    vector<pair<long long,long long>> hs(m);
    for(int j=0;j<m;j++){
      long long v,d; scanf("%lld %lld",&v,&d);
      hs[j]={d,v};
    }
    sort(hs.begin(),hs.end()); // by d ascending
    hd[i].resize(m);
    vector<long long> sv(m);
    for(int j=0;j<m;j++){hd[i][j]=hs[j].first; sv[j]=hs[j].second;}
    for(int j=m-1;j>=0;j--){ sv[j]=max(sv[j], (j+1<m?sv[j+1]:0LL)); }
    hv[i]=sv;
  }
  vector<int> idx(N);
  for(int i=0;i<N;i++) idx[i]=i;
  sort(idx.begin(),idx.end(),[&](int a,int b){return X[a]<X[b];});
  vector<long long> pos(N);
  vector<vector<long long>> Hd(N), Hv(N);
  for(int k=0;k<N;k++){
    int i=idx[k];
    pos[k]=X[i];
    Hd[k]=move(hd[i]);
    Hv[k]=move(hv[i]);
  }
  const double INF=1e18;
  vector<double> dist(N, INF);
  priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
  for(int k=0;k<N;k++) if(pos[k]==0){ dist[k]=0; pq.push({0.0,k}); }
  double ans=INF;
  while(!pq.empty()){
    auto pr=pq.top(); pq.pop();
    double dk=pr.first; int k=pr.second;
    if(dk>dist[k]+1e-12) continue;
    if(dk>=ans) continue;
    long long Xk=pos[k];
    int m=Hd[k].size();
    {
      long long L = B - Xk;
      if(L>=0){
        int p = lower_bound(Hd[k].begin(),Hd[k].end(),L)-Hd[k].begin();
        if(p<m){
          double v=Hv[k][p];
          double cand=dk + (double)L/v;
          if(cand<ans) ans=cand;
        }
      }
    }
    int start = lower_bound(pos.begin(),pos.end(),Xk)-pos.begin();
    int p=0;
    for(int j=start;j<N;j++){
      long long L=pos[j]-Xk;
      while(p<m && Hd[k][p]<L) p++;
      if(p>=m) break;
      double v=Hv[k][p];
      double cand=dk + (double)L/v;
      if(cand<dist[j]){ dist[j]=cand; pq.push({cand,j}); }
    }
  }
  if(ans>=INF/2) printf("-1\n");
  else printf("%.8f\n", ans);
  return 0;
}
