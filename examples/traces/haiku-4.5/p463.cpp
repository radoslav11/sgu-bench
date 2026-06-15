#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int n, m;
  cin >> n >> m;
  
  vector<string> grid(n);
  for(int i = 0; i < n; i++){
    cin >> grid[i];
  }
  
  string path;
  cin >> path;
  
  // Track how many times each block has been passed
  map<pair<int,int>, int> passed;
  
  // Position on street grid (0 to n for rows, 0 to m for columns)
  int row = 0, col = 0;
  // Direction: 0=east, 1=south, 2=west, 3=north
  int dir = 0;
  
  // Direction vectors: east, south, west, north
  int dr[] = {0, 1, 0, -1};
  int dc[] = {1, 0, -1, 0};
  
  int satisfaction = 0;
  
  for(char c : path){
    if(c == 'M'){
      // Move forward
      int newRow = row + dr[dir];
      int newCol = col + dc[dir];
      
      // Determine which blocks are passed
      // When moving, we pass blocks adjacent to the movement
      if(dir == 0){ // Moving east
        if(row > 0) {
          auto key = make_pair(row-1, col);
          int beauty = grid[row-1][col] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
        if(row < n) {
          auto key = make_pair(row, col);
          int beauty = grid[row][col] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
      } else if(dir == 1){ // Moving south
        if(col > 0) {
          auto key = make_pair(row, col-1);
          int beauty = grid[row][col-1] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
        if(col < m) {
          auto key = make_pair(row, col);
          int beauty = grid[row][col] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
      } else if(dir == 2){ // Moving west
        if(row > 0) {
          auto key = make_pair(row-1, col-1);
          int beauty = grid[row-1][col-1] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
        if(row < n) {
          auto key = make_pair(row, col-1);
          int beauty = grid[row][col-1] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
      } else { // Moving north
        if(col > 0) {
          auto key = make_pair(row-1, col-1);
          int beauty = grid[row-1][col-1] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
        if(col < m) {
          auto key = make_pair(row-1, col);
          int beauty = grid[row-1][col] - '0';
          satisfaction += (passed[key] == 0) ? beauty : beauty/2;
          passed[key]++;
        }
      }
      
      row = newRow;
      col = newCol;
    } else if(c == 'L'){
      dir = (dir + 3) % 4;
    } else {
      dir = (dir + 1) % 4;
    }
  }
  
  cout << satisfaction << endl;
  
  return 0;
}
