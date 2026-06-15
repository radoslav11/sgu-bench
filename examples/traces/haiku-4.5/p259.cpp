#include<bits/stdc++.h>
using namespace std;

int main(){
  int n;
  cin >> n;
  vector<pair<int,int>> leaflets(n);
  
  for(int i = 0; i < n; i++){
    cin >> leaflets[i].first;
  }
  for(int i = 0; i < n; i++){
    cin >> leaflets[i].second;
  }
  
  // Try all permutations and find minimum
  vector<int> order(n);
  for(int i = 0; i < n; i++) order[i] = i;
  
  int minTime = INT_MAX;
  
  do {
    int currentTime = 0;
    int maxDelivery = 0;
    for(int i = 0; i < n; i++){
      currentTime += leaflets[order[i]].first;
      maxDelivery = max(maxDelivery, currentTime + leaflets[order[i]].second);
    }
    minTime = min(minTime, maxDelivery);
  } while(next_permutation(order.begin(), order.end()));
  
  cout << minTime << endl;
  
  return 0;
}
