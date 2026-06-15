#include <iostream>
#include <vector>
using namespace std;

int main() {
    long long n_input, m;
    cin >> n_input >> m;

    if (n_input == 1) {
        cout << (1 % m) << '\n';
        return 0;
    }

    long long n = n_input;
    vector<long long> E(n + 1, 0);
    E[0] = 1 % m;
    E[1] = 1 % m;

    vector<long long> prev = {1 % m};

    for (int i = 2; i <= n; i++) {
        vector<long long> curr(i, 1 % m);
        for (int j = 1; j < i - 1; j++) {
            curr[j] = (prev[j - 1] + prev[j]) % m;
        }

        for (int k = 1; k < i; k += 2) {
            long long term = curr[k];
            term = (term * E[k]) % m;
            term = (term * E[i - 1 - k]) % m;
            E[i] = (E[i] + term) % m;
        }

        prev = curr;
    }

    long long ans = (2 * E[n]) % m;
    cout << ans << '\n';

    return 0;
}
