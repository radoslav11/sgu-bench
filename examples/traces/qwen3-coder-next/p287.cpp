#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    long long q;
    int c;
    while (cin >> q >> c) {
        if (q == 1) {
            cout << c + 1 << '\n';
        } else if (c == 0) {
            int n = 0;
            while ((1LL << n) <= q) {
                n++;
            }
            cout << n << '\n';
        } else {
            vector<long long> dp;
            dp.push_back(1);
            int n = 0;
            while (dp[n] < q) {
                n++;
                if (n <= c) {
                    dp.push_back(1);
                } else {
                    long long next_val = dp[n-1] + dp[n - c - 1];
                    dp.push_back(next_val);
                }
            }
            cout << n << '\n';
        }
    }
    return 0;
}
