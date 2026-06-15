#include<bits/stdc++.h>
using namespace std;

int main(){
  int H, W;
  cin >> H >> W;
  
  vector<string> fingerprint(H-1);
  for(int i = 0; i < H-1; i++){
    cin >> fingerprint[i];
  }
  
  // Try all possible first rows
  for(int mask = 0; mask < (1 << W); mask++){
    vector<vector<int>> key(H, vector<int>(W));
    
    // Set first row based on mask
    for(int j = 0; j < W; j++){
      key[0][j] = (mask >> j) & 1;
    }
    
    bool valid = true;
    
    // Fill the rest of the matrix
    for(int i = 0; i < H-1 && valid; i++){
      for(int j = 0; j < W-1 && valid; j++){
        // B[i][j] = A[i][j] + A[i+1][j] + A[i][j+1] + A[i+1][j+1]
        // A[i+1][j+1] = B[i][j] - A[i][j] - A[i+1][j] - A[i][j+1]
        int needed = (fingerprint[i][j] - '0') - key[i][j] - key[i+1][j] - key[i][j+1];
        if(needed < 0 || needed > 1){
          valid = false;
        } else {
          key[i+1][j+1] = needed;
        }
      }
    }
    
    if(valid){
      for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
          cout << key[i][j];
        }
        cout << "\n";
      }
      return 0;
    }
  }
  
  cout << "CORRUPT\n";
  return 0;
}
