#include <bits/stdc++.h>
using namespace std;

int main() {
    int K;
    cin >> K;
    while (K--) {
        int N;
        cin >> N;
        vector<long long> A(N);
        for (int i = 0; i < N; i++) cin >> A[i];
        
        long long prod_mod = A[0] % 9;
        long long sum_mod = prod_mod;
        
        for (int i = 1; i < N; i++) {
            prod_mod = (prod_mod * (A[i] % 9)) % 9;
            sum_mod = (sum_mod + prod_mod) % 9;
        }
        
        if (sum_mod == 0) {
            if (A[0] == 0) {
                cout << 0 << "\n";
            } else {
                cout << 9 << "\n";
            }
        } else {
            cout << sum_mod << "\n";
        }
    }
    return 0;
}
