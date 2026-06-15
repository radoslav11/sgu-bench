#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n;
  cin >> n;
  vector<int> y(n);
  for(int i = 0; i < n; i++){
    cin >> y[i];
  }
  
  // Check all heights that appear in the polyline
  set<int> heights;
  for(int i = 0; i < n; i++){
    heights.insert(y[i]);
  }
  
  // For each segment, add intermediate heights
  for(int i = 0; i < n-1; i++){
    int minH = min(y[i], y[i+1]);
    int maxH = max(y[i], y[i+1]);
    for(int h = minH; h <= maxH; h++){
      heights.insert(h);
    }
  }
  
  // Check if there exists a height h where both paths can meet
  for(int h : heights){
    // Check if height h can be reached from both sides
    bool leftReach = false, rightReach = false;
    
    for(int i = 0; i < n-1; i++){
      int minH = min(y[i], y[i+1]);
      int maxH = max(y[i], y[i+1]);
      if(h >= minH && h <= maxH) leftReach = true;
    }
    
    for(int i = n-1; i > 0; i--){
      int minH = min(y[i], y[i-1]);
      int maxH = max(y[i], y[i-1]);
      if(h >= minH && h <= maxH) rightReach = true;
    }
    
    if(leftReach && rightReach){
      cout << "YES\n";
      return 0;
    }
  }
  
  cout << "NO\n";
  return 0;
}
