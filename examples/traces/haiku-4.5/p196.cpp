#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int N, M;
  cin >> N >> M;
  
  vector<long long> degree(N + 1, 0);
  
  for(int i = 0; i < M; i++){
    int u, v;
    cin >> u >> v;
    degree[u]++;
    degree[v]++;
  }
  
  long long sum = 0;
  for(int i = 1; i <= N; i++){
    sum += degree[i] * degree[i];
  }
  
  cout << sum << endl;
  
  return 0;
}
