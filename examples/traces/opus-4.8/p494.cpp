#include <bits/stdc++.h>
using namespace std;
int main(){
  // fast read
  int T; 
  // read T
  ...
  for each test:
    read N,W,R,C
    read L[0..N-1]
    // P[i] = prefix sum of (L)+ i ... P[0]=0; P[i+1]=P[i]+L[i]+1
    vector<long long> P(N+1);
    P[0]=0;
    for i: P[i+1]=P[i]+L[i]+1;
    // nextF, nextG via two pointer
    vector<int> nextF(N+1), nextG(N+1);
    // full width W
    {
      int e=0;
      for(int s=0;s<=N;s++){ if(e<s) e=s; while(e<N && P[e+1] <= P[s]+W+1) e++; nextF[s]=e; }
    }
    int wg = W - C;
    {
      int e=0;
      for(int s=0;s<=N;s++){ if(e<s)e=s; while(e<N && P[e+1] <= P[s]+wg+1) e++; nextG[s]=e; }
    }
    // rowsFull
    vector<int> rowsFull(N+1);
    rowsFull[N]=0;
    for(int s=N-1;s>=0;s--){ rowsFull[s]=1+rowsFull[nextF[s]]; }
    // binary lifting nextG
    int LOG=1; while((1LL<<LOG) <= R) LOG++;
    // up[k]
    vector<vector<int>> up(LOG, vector<int>(N+1));
    up[0]=nextG;
    for(int k=1;k<LOG;k++) for(int s=0;s<=N;s++) up[k][s]=up[k-1][up[k-1][s]];
    // iterate r
    long long ans=LLONG_MAX;
    int p=0, r=0;
    while(true){
      // jump R from p
      int cur=p;
      long long RR=R;
      for(int k=0;k<LOG;k++) if(RR>>k & 1) cur=up[k][cur];
      long long used=(long long)r + R + rowsFull[cur];
      ans=min(ans,used);
      if(p==N) break;
      p=nextF[p]; r++;
    }
    print ans
