#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int N, M;
  cin >> N >> M;
  
  vector<int> window_free(M + 1, 0);  // when each window becomes free (1-indexed)
  
  for(int i = 0; i < N; i++){
    int k, t;
    cin >> k >> t;
    
    // Count queue size: people still in queue (window_free > current arrival time)
    int best_window = 1;
    int min_queue_size = (window_free[1] > k) ? 1 : 0;
    
    for(int w = 2; w <= M; w++){
      int queue_size_w = (window_free[w] > k) ? 1 : 0;
      if(queue_size_w < min_queue_size){
        min_queue_size = queue_size_w;
        best_window = w;
      }
    }
    
    // Calculate when they leave
    int start_service = max(k, window_free[best_window]);
    int leave_time = start_service + t;
    
    // Update window free time
    window_free[best_window] = leave_time;
    
    cout << best_window << " " << leave_time << "\n";
  }
  
  return 0;
}
