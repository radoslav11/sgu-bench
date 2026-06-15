#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n;
  cin >> n;
  
  vector<vector<int>> l(n+1, vector<int>(n+1));
  
  // Read the adjacency matrix (1-indexed)
  for(int i = 1; i <= n; i++){
    for(int j = 1; j <= n; j++){
      cin >> l[i][j];
    }
  }
  
  int k;
  cin >> k;
  
  vector<int> roads(k);
  for(int i = 0; i < k; i++){
    cin >> roads[i];
  }
  
  // BFS/DP approach: track possible current positions
  set<int> current;
  current.insert(1);  // Start at intersection 1
  
  // Process each road segment
  for(int i = 0; i < k; i++){
    int road_length = roads[i];
    set<int> next;
    
    // For each possible current position
    for(int pos : current){
      // Find all neighbors connected by the required road length
      for(int neighbor = 1; neighbor <= n; neighbor++){
        if(l[pos][neighbor] == road_length){
          next.insert(neighbor);
        }
      }
    }
    
    current = next;
  }
  
  // Output results
  if(current.empty()){
    cout << 0 << endl;
  } else {
    cout << current.size() << endl;
    bool first = true;
    for(int pos : current){
      if(!first) cout << " ";
      cout << pos;
      first = false;
    }
    cout << endl;
  }
  
  return 0;
}
