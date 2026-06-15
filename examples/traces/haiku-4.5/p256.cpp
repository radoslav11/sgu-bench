#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int M, N;
  cin >> M >> N;
  
  vector<int> A(N), B(N);
  for(int i = 0; i < N; i++){
    cin >> A[i] >> B[i];
  }
  
  // Binary search on time
  int left = 1, right = 1000;
  int answer = right;
  
  while(left <= right){
    int mid = (left + right) / 2;
    
    // Check if we can fill M balloons in mid minutes
    int total_balloons = 0;
    for(int i = 0; i < N; i++){
      int cycle_time = 1 + B[i];
      int complete_cycles = mid / cycle_time;
      int remaining_time = mid % cycle_time;
      
      int balloons = complete_cycles * A[i];
      if(remaining_time >= 1){
        balloons += A[i];
      }
      
      total_balloons += balloons;
    }
    
    if(total_balloons >= M){
      answer = mid;
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }
  
  cout << answer << endl;
  
  return 0;
}
