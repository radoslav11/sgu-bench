#include <bits/stdc++.h>
using std::cin; using std::cout;

int main(){
    int N, M, Y;
    cin >> N >> M >> Y;
    
    std::vector<int> X(N);
    for(int i = 0; i < N; i++) cin >> X[i];
    
    std::vector<long long> f(N), r(N);
    long long S = 0;
    for(int i = 0; i < N; i++){
        long long val = (long long)X[i] * M;
        f[i] = val / Y;
        r[i] = val % Y;
        S += f[i];
    }
    
    long long R = M - S; // extra coins to distribute
    
    // Sort indices by r descending
    std::vector<int> idx(N);
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&](int a, int b){
        return r[a] > r[b];
    });
    
    std::vector<long long> K(N);
    for(int i = 0; i < N; i++) K[i] = f[i];
    for(int i = 0; i < R; i++) K[idx[i]]++;
    
    for(int i = 0; i < N; i++){
        if(i) cout << ' ';
        cout << K[i];
    }
    cout << '\n';
    
    return 0;
}
