#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    cin >> N >> K;
    
    int size = N * N;
    vector<string> grid(size, string(size, '.'));
    
    // Place K blues in each row using a pattern
    for(int i = 0; i < size; i++){
        for(int k = 0; k < K; k++){
            int col = (i + k * N) % size;
            grid[i][col] = '*';
        }
    }
    
    // Verify
    for(int i = 0; i < size; i++){
        int rc = 0, cc = 0;
        for(int j = 0; j < size; j++){
            if(grid[i][j] == '*') rc++;
            if(grid[j][i] == '*') cc++;
        }
        if(rc != K || cc != K){
            cout << "NO SOLUTION\n";
            return 0;
        }
    }
    
    for(int bi = 0; bi < N; bi++){
        for(int bj = 0; bj < N; bj++){
            int bc = 0;
            for(int i = bi*N; i < (bi+1)*N; i++){
                for(int j = bj*N; j < (bj+1)*N; j++){
                    if(grid[i][j] == '*') bc++;
                }
            }
            if(bc != K){
                cout << "NO SOLUTION\n";
                return 0;
            }
        }
    }
    
    for(int i = 0; i < size; i++){
        cout << grid[i] << "\n";
    }
    
    return 0;
}
