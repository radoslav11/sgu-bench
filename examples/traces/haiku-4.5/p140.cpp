#include<bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, P, B;
    cin >> N >> P >> B;
    
    vector<long long> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
        A[i] %= P;
    }
    
    // Calculate gcd of all A[i] and P
    long long g = P;
    for(int i = 0; i < N; i++) {
        g = gcd(g, A[i]);
    }
    
    if(B % g != 0) {
        cout << "NO\n";
        return 0;
    }
    
    // Try to find a solution - use first non-zero A[i] if exists
    vector<long long> X(N, 0);
    for(int i = 0; i < N; i++) {
        if(A[i] % P != 0) {
            // Try to solve A[i] * X[i] ≡ B (mod P)
            for(long long val = 0; val < P; val++) {
                if((A[i] * val) % P == B) {
                    X[i] = val;
                    cout << "YES\n";
                    for(int j = 0; j < N; j++) {
                        if(j > 0) cout << " ";
                        cout << X[j];
                    }
                    cout << "\n";
                    return 0;
                }
            }
        }
    }
    
    cout << "NO\n";
    return 0;
}
