#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int N, M;
  cin >> N >> M;
  
  vector<int> seq(N);
  for(int i = 0; i < N; i++){
    seq[i] = i + 1;
  }
  
  for(int i = 0; i < M; i++){
    int P, Q;
    cin >> P >> Q;
    // Convert to 0-indexed and reverse from P-1 to Q-1 (inclusive)
    reverse(seq.begin() + P - 1, seq.begin() + Q);
  }
  
  for(int i = 0; i < N; i++){
    if(i > 0) cout << " ";
    cout << seq[i];
  }
  cout << "\n";
  
  return 0;
}
