#include <bits/stdc++.h>
using namespace std;
int n;
vector<int> adj[105];
int val[105];
long long curR;

// compute contribution of edges incident to p or q
long long contrib(int p,int q){
  long long s=0;
  for(int x:adj[p]){ long long d=val[p]-val[x]; s+=d*d; }
  for(int x:adj[q]){ if(x==p) continue; long long d=val[q]-val[x]; s+=d*d; }
  return s;
}
