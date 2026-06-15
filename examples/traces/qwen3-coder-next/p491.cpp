#include <iostream>
#include <vector>
using namespace std;

int main() {
    long long N;
    cin >> N;

    if (N < 2) {
        cout << 0 << endl;
        return 0;
    }

    vector<vector<int>> divisors(N + 1);
    for (int i = 1; i <= N; i++) {
        for (int j = i; j <= N; j += i) {
            divisors[j].push_back(i);
        }
    }

    vector<bool> in_set(N + 1, false);
    vector<int> to_clear;
    long long ans = 0;

    for (long long A = 1; A * 2 < N; A++) {
        to_clear.clear();
        for (long long k = 1; k * A < N; k++) {
            long long M = N - k * A;
            for (int d : divisors[M]) {
                if (d > A && d <= M) {
                    if (!in_set[d]) {
                        in_set[d] = true;
                        to_clear.push_back(d);
                        ans++;
                    }
                }
            }
        }
        for (int d : to_clear) {
            in_set[d] = false;
        }
    }

    cout << ans << endl;
    return 0;
}
