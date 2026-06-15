#include <bits/stdc++.h>
using namespace std;
int main(){
  int n; 
  if(scanf("%d",&n)!=1) return 0;
  vector<long long> a(n+1);
  for(int i=1;i<=n;i++) scanf("%lld",&a[i]);
  vector<long long> s(a.begin()+1, a.end());
  sort(s.begin(), s.end());
  vector<long long> b(n+1);
  for(int i=1;i<=n;i++) b[i]=s[i-1];
  bool issorted=true;
  for(int i=1;i<=n;i++) if(a[i]!=b[i]) issorted=false;
  if(issorted){ printf("0\n"); return 0;}
  // 1-step check
  map<pair<long long,long long>, vector<int>> grp;
  for(int i=1;i<=n;i++) grp[{a[i],b[i]}].push_back(i);
  bool one=true;
  for(auto &kv: grp){
    long long c=kv.first.first, t=kv.first.second;
    if(c==t) continue;
    auto it=grp.find({t,c});
    int other = (it==grp.end())?0:(int)it->second.size();
    if((int)kv.second.size()!=other){ one=false; break;}
  }
  if(one){
    vector<pair<int,int>> pairs;
    for(auto &kv: grp){
      long long c=kv.first.first, t=kv.first.second;
      if(c>=t) continue;
      auto it=grp.find({t,c});
      auto &v1=kv.second; auto &v2=it->second;
      for(size_t k=0;k<v1.size();k++) pairs.push_back({v1[k],v2[k]});
    }
    printf("1\n");
    printf("%d", (int)pairs.size());
    for(auto &p:pairs) printf(" %d %d", p.first,p.second);
    printf("\n");
    return 0;
  }
  // 2-step
  map<long long, vector<int>> cur, tar;
  for(int i=1;i<=n;i++){ cur[a[i]].push_back(i); tar[b[i]].push_back(i);}
  vector<int> dest(n+1,0);
  for(auto &kv: cur){
    auto &cv=kv.second; auto &tv=tar[kv.first];
    for(size_t k=0;k<cv.size();k++) dest[cv[k]]=tv[k];
  }
  vector<char> vis(n+1,0);
  vector<pair<int,int>> step1, step2;
  for(int i=1;i<=n;i++){
    if(vis[i]) continue;
    vector<int> cyc;
    int x=i;
    while(!vis[x]){ vis[x]=1; cyc.push_back(x); x=dest[x]; }
    int m=cyc.size();
    if(m==1) continue;
    vector<char> done(m,0);
    for(int t=0;t<m;t++){
      if(done[t]) continue;
      int tt=( (m-t)%m );
      if(tt!=t){ step1.push_back({cyc[t],cyc[tt]}); done[t]=1; done[tt]=1;}
      else done[t]=1;
    }
    vector<char> done2(m,0);
    for(int t=0;t<m;t++){
      if(done2[t]) continue;
      int tt=(((1-t)%m)+m)%m;
      if(tt!=t){ step2.push_back({cyc[t],cyc[tt]}); done2[t]=1; done2[tt]=1;}
      else done2[t]=1;
    }
  }
  printf("2\n");
  printf("%d",(int)step1.size());
  for(auto &p:step1) printf(" %d %d",p.first,p.second);
  printf("\n");
  printf("%d",(int)step2.size());
  for(auto &p:step2) printf(" %d %d",p.first,p.second);
  printf("\n");
  return 0;
}
