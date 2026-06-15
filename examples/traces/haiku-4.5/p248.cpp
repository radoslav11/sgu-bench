#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int N;
  cin >> N;
  
  vector<long long> c(N);
  for(int i = 0; i < N; i++){
    cin >> c[i];
  }
  
  long long V;
  cin >> V;
  
  long long minSum = LLONG_MAX;
  
  if(N == 1){
    if(V % c[0] == 0){
      minSum = V / c[0];
    }
  }
  else if(N == 2){
    for(long long x1 = 0; x1 * c[0] <= V; x1++){
      long long remaining = V - x1 * c[0];
      if(remaining % c[1] == 0){
        long long x2 = remaining / c[1];
        minSum = min(minSum, x1 + x2);
      }
    }
  }
  else if(N == 3){
    for(long long x1 = 0; x1 * c[0] <= V; x1++){
      for(long long x2 = 0; x1 * c[0] + x2 * c[1] <= V; x2++){
        long long remaining = V - x1 * c[0] - x2 * c[1];
        if(remaining % c[2] == 0){
          long long x3 = remaining / c[2];
          minSum = min(minSum, x1 + x2 + x3);
        }
      }
    }
  }
  
  if(minSum == LLONG_MAX){
    cout << -1 << endl;
  }
  else{
    cout << minSum << endl;
  }
  
  return 0;
}
