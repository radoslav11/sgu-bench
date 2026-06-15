#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n, k;
  cin >> n >> k;
  
  vector<long long> x(n), y(n), w(n);
  
  for(int i = 0; i < n; i++){
    cin >> x[i] >> y[i] >> w[i];
  }
  
  vector<int> closeable;
  
  // For each hut A
  for(int a = 0; a < n; a++){
    int count = 0;
    
    // Check how many other huts B can reach hut A
    for(int b = 0; b < n; b++){
      if(a == b) continue; // Skip the same hut
      
      long long dist = abs(x[b] - x[a]) + abs(y[b] - y[a]);
      
      // Can pizza boy from hut b reach hut a in 10 minutes?
      if(dist <= w[b]){
        count++;
      }
    }
    
    // If at least K other huts can reach this hut, it can be closed
    if(count >= k){
      closeable.push_back(a + 1); // 1-indexed
    }
  }
  
  cout << closeable.size() << "\n";
  if(closeable.size() > 0){
    for(int i = 0; i < closeable.size(); i++){
      if(i > 0) cout << " ";
      cout << closeable[i];
    }
    cout << "\n";
  }
  
  return 0;
}
