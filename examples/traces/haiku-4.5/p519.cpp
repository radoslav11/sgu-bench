#include<bits/stdc++.h>
using namespace std;

int main(){
  int n, m;
  cin >> n >> m;
  
  vector<string> grid(n);
  for(int i = 0; i < n; i++){
    cin >> grid[i];
  }
  
  long long surface = 0;
  
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      int h = grid[i][j] - '0';
      
      if(h == 0) continue;
      
      // Top and bottom
      surface += 2;
      
      // North
      int h_north = (i > 0) ? (grid[i-1][j] - '0') : 0;
      surface += max(0, h - h_north);
      
      // South
      int h_south = (i < n-1) ? (grid[i+1][j] - '0') : 0;
      surface += max(0, h - h_south);
      
      // West
      int h_west = (j > 0) ? (grid[i][j-1] - '0') : 0;
      surface += max(0, h - h_west);
      
      // East
      int h_east = (j < m-1) ? (grid[i][j+1] - '0') : 0;
      surface += max(0, h - h_east);
    }
  }
  
  cout << surface << endl;
  
  return 0;
}
