#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, M;
    cin >> N >> M;
    
    vector<vector<int>> A(N+1, vector<int>(M+1));
    for(int i=1;i<=N;i++)
        for(int j=1;j<=M;j++)
            cin >> A[i][j];
    
    // col_min[i][j] = min of A[i..N][j]
    vector<vector<int>> col_min(N+2, vector<int>(M+1, INT_MAX));
    for(int j=1;j<=M;j++){
        col_min[N][j] = A[N][j];
        for(int i=N-1;i>=1;i--)
            col_min[i][j] = min(A[i][j], col_min[i+1][j]);
    }
    
    vector<vector<int>> B(N+1, vector<int>(M+1, INT_MAX));
    
    // For each diagonal d = i+j
    for(int d=2; d<=N+M; d++){
        // y ranges from max(1,d-N) to min(M, d-1)
        int ylo = max(1, d-N);
        int yhi = min(M, d-1);
        
        vector<int> f(M+2, INT_MAX);
        for(int y=ylo; y<=yhi; y++){
            int x = max(1, d-y);
            if(x >= 1 && x <= N)
                f[y] = col_min[x][y];
        }
        
        // suffix min from right
        vector<int> suf(M+2, INT_MAX);
        suf[yhi+1] = INT_MAX;
        for(int y=yhi; y>=ylo; y--){
            suf[y] = min(f[y], (y+1<=yhi ? suf[y+1] : INT_MAX));
        }
        
        for(int y=ylo; y<=yhi; y++){
            int i = d - y;
            int j = y;
            if(i>=1 && i<=N && j>=1 && j<=M)
                B[i][j] = suf[y];
        }
    }
    
    for(int i=1;i<=N;i++){
        for(int j=1;j<=M;j++){
            if(j>1) cout << ' ';
            cout << B[i][j];
        }
        cout << '\n';
    }
    
    return 0;
}
