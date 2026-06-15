#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n, m;
  cin >> n >> m;
  
  vector<vector<int>> children(n + 1);
  
  for(int i = 2; i <= n; i++){
    int parent;
    cin >> parent;
    children[parent].push_back(i);
  }
  
  vector<long long> leafValues(n + 1);
  for(int i = n - m + 1; i <= n; i++){
    cin >> leafValues[i];
  }
  
  function<void(int, vector<long long>&)> collectLeaves = [&](int node, vector<long long>& leaves){
    if(node > n - m){
      leaves.push_back(leafValues[node]);
      return;
    }
    for(int child : children[node]){
      collectLeaves(child, leaves);
    }
  };
  
  for(int i = 1; i <= n - m; i++){
    vector<long long> leaves;
    collectLeaves(i, leaves);
    
    if(leaves.size() == 1){
      cout << 2147483647;
    } else {
      sort(leaves.begin(), leaves.end());
      long long minDiff = LLONG_MAX;
      for(size_t j = 0; j < leaves.size() - 1; j++){
        minDiff = min(minDiff, leaves[j + 1] - leaves[j]);
      }
      cout << minDiff;
    }
    
    if(i < n - m) cout << " ";
  }
  cout << "\n";
  
  return 0;
}
