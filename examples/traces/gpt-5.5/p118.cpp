#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K;
    cin >> K;

    while (K--) {
        int N;
        cin >> N;

        long long prod = 1;
        int sum = 0;
        bool zeroExpression = false;

        for (int i = 1; i <= N; ++i) {
            long long a;
            cin >> a;

            if (i == 1 && a == 0) {
                zeroExpression = true;
            }

            prod = (prod * (a % 9)) % 9;
            sum = (sum + prod) % 9;
        }

        if (zeroExpression) {
            cout << 0 << '\n';
        } else {
            cout << (sum == 0 ? 9 : sum) << '\n';
        }
    }

    return 0;
}
