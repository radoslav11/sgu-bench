#include<bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    return b ? gcd(b, a % b) : a;
}

int main(){
    int N, M;
    cin >> N >> M;
    
    long long count = 0;
    
    // Burnside's lemma for cyclic group actions
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            long long g = gcd((long long)(i ? i : N), (long long)N) * 
                         gcd((long long)(j ? j : M), (long long)M);
            count += (1LL << g);
        }
    }
    
    long long result = count / (N * M);
    
    if(N == M) {
        count = 0;
        for(int d = 1; d <= N; d++) {
            if(N % d == 0) {
                count += (1LL << d);
            }
        }
        result = (result + count / N) / 2;
    }
    
    cout << result << endl;
    
    return 0;
}
