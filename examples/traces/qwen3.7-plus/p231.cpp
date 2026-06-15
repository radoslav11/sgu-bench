#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    if (!(cin >> n)) return 0;

    vector<char> is_prime(n + 1, 1);
    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;
    for (int p = 2; p * p <= n; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= n; i += p)
                is_prime[i] = 0;
        }
    }

    vector<pair<int, int>> ans;
    // Since A and B are primes and A <= B, if both are odd (>= 3), their sum is even and >= 6, which cannot be prime.
    // Thus, A must be exactly 2. Then B must be an odd prime such that B + 2 is also prime and <= n.
    for (int b = 3; b + 2 <= n; b += 2) {
        if (is_prime[b] && is_prime[b + 2]) {
            ans.push_back({2, b});
        }
    }

    cout << ans.size() << "\n";
    for (auto p : ans) {
        cout << p.first << " " << p.second << "\n";
    }

    return 0;
}
