#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> A(N+1, vector<int>(M+1));
    vector<vector<int>> B(N+1, vector<int>(M+1));
    
    // Read matrix A (1-indexed)
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= M; j++){
            cin >> A[i][j];
        }
    }
    
    // Compute matrix B
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= M; j++){
            int minVal = INT_MAX;
            
            // Iterate through all valid (x, y) pairs
            for(int y = j; y <= M; y++){
                int minX = max(1, i + j - y);
                int maxX = N;
                for(int x = minX; x <= maxX; x++){
                    minVal = min(minVal, A[x][y]);
                }
            }
            
            B[i][j] = minVal;
        }
    }
    
    // Output matrix B
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= M; j++){
            if(j > 1) cout << " ";
            cout << B[i][j];
        }
        cout << "\n";
    }
    
    return 0;
}
