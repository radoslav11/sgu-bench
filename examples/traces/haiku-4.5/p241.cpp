#include<bits/stdc++.h>
using namespace std;

long long fact[45];
long long comb[45][45];

void precompute() {
    for(int i = 0; i <= 40; i++) {
        fact[i] = 1;
        for(int j = 1; j <= i; j++) fact[i] *= j;
    }
    for(int i = 0; i <= 40; i++) {
        for(int j = 0; j <= i; j++) {
            if(j == 0 || j == i) comb[i][j] = 1;
            else comb[i][j] = comb[i-1][j-1] + comb[i-1][j];
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    precompute();
    
    long long N, M, W, H, K;
    cin >> N >> M >> W >> H >> K;
    
    if(K > 40 || K > N + M) {
        cout << 0 << endl;
        return 0;
    }
    
    // Calculate distinct columns
    long long col_min = 0, col_max = max((long long)(N-1), W + M - 1);
    long long cols = col_max - col_min + 1;
    
    // Calculate distinct rows  
    long long row_min = 0, row_max = max((long long)(N-1), H + M - 1);
    long long rows = row_max - row_min + 1;
    
    if(K > cols || K > rows) {
        cout << 0 << endl;
        return 0;
    }
    
    cols = min(cols, (long long)40);
    rows = min(rows, (long long)40);
    
    long long result = comb[cols][K] * comb[rows][K] % 1000000000;
    result = result * fact[K] % 1000000000;
    
    cout << result << endl;
    
    return 0;
}
