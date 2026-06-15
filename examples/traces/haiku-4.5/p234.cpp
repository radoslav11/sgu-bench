#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int M, N;
  cin >> M >> N;
  
  vector<vector<int>> board(M, vector<int>(N));
  vector<vector<char>> result(M, vector<char>(N));
  
  for(int i = 0; i < M; i++){
    for(int j = 0; j < N; j++){
      cin >> board[i][j];
      if(board[i][j] == 0){
        result[i][j] = '#';
      } else {
        result[i][j] = '.';
      }
    }
  }
  
  // Try checkerboard pattern - place Guardians where (i+j) is even
  int count = 0;
  for(int i = 0; i < M; i++){
    for(int j = 0; j < N; j++){
      if(board[i][j] == 1 && (i + j) % 2 == 0){
        result[i][j] = 'G';
        count++;
      }
    }
  }
  
  // Verify all cells are covered
  bool all_covered = true;
  for(int i = 0; i < M; i++){
    for(int j = 0; j < N; j++){
      if(board[i][j] == 1 && result[i][j] != 'G'){
        // Check if covered by adjacent Guardian
        bool covered = false;
        // Check all 4 directions
        int di[] = {-1, 1, 0, 0};
        int dj[] = {0, 0, -1, 1};
        for(int d = 0; d < 4; d++){
          int ni = i + di[d];
          int nj = j + dj[d];
          if(ni >= 0 && ni < M && nj >= 0 && nj < N && result[ni][nj] == 'G'){
            covered = true;
            break;
          }
        }
        if(!covered){
          all_covered = false;
          break;
        }
      }
    }
    if(!all_covered) break;
  }
  
  cout << count << "\n";
  for(int i = 0; i < M; i++){
    for(int j = 0; j < N; j++){
      cout << result[i][j];
    }
    cout << "\n";
  }
  
  return 0;
}
