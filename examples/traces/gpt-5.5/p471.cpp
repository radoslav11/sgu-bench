#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N;
    int tc = 1;
    while (cin >> N) {
        long long num, den;

        if (N == 1) {
            num = -1;
            den = 1;
        } else {
            long long gamma;
            if (N % 4 == 0) gamma = N / 2;
            else if (N % 4 == 1) gamma = (N + 1) / 2;
            else if (N % 4 == 2) gamma = (N + 2) / 2;
            else gamma = (N + 1) / 2;

            num = 2 - gamma;
            den = gamma;

            long long g = std::gcd(llabs(num), den);
            num /= g;
            den /= g;
        }

        cout << "Case #" << tc++ << ": " << num << "/" << den << '\n';
    }

    return 0;
}
