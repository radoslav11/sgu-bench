#include <iostream>

using namespace std;

int v2(long long x) {
    if (x == 0) return 100;
    int res = 0;
    while (x % 2 == 0) {
        res++;
        x /= 2;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long A, B;
    if (cin >> A >> B) {
        if (A == 0 || B == 0) {
            cout << 0 << "\n";
            return 0;
        }
        
        long long S = A + B;
        long long m_S = S;
        while (m_S % 2 == 0) {
            m_S /= 2;
        }
        
        if (A % m_S != 0) {
            cout << -1 << "\n";
        } else {
            int ans = v2(S) - v2(A);
            cout << ans << "\n";
        }
    }
    
    return 0;
}
