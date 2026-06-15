#include<bits/stdc++.h>
using namespace std;

int grayCode(int n) {
    return n ^ (n >> 1);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    int rows = 1 << N;  // 2^N
    int cols = 1 << M;  // 2^M
    
    vector<vector<int>> matrix(rows, vector<int>(cols));
    
    // For each position (i, j), compute the value
    // We use Gray code for both dimensions
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            // Gray code of row index contributes first N bits
            int grayRow = grayCode(i);
            // Gray code of col index contributes last M bits
            int grayCol = grayCode(j);
            
            // Combine: shift grayRow left by M bits and OR with grayCol
            int value = (grayRow << M) | grayCol;
            matrix[i][j] = value;
        }
    }
    
    // Output
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(j > 0) cout << " ";
            cout << matrix[i][j];
        }
        cout << "\n";
    }
    
    return 0;
}
