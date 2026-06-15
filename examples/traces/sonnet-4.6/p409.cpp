#include <bits/stdc++.h>
using namespace std;
int main(){
    int N, K;
    cin >> N >> K;
    int N2 = N * N;
    if(K < 0 || K > N2){
        cout << "NO SOLUTION\n";
        return 0;
    }
    vector<string> grid(N2, string(N2, '.'));
    for(int off = 0; off < K; off++){
        for(int i = 0; i < N2; i++){
            int col = (N*(i%N) + i/N + off) % N2;
            grid[i][col] = '*';
        }
    }
    for(auto& row : grid) cout << row << "\n";
}
