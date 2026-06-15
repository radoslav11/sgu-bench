#include <iostream>
using namespace std;

int main() {
    long long N, q;
    cin >> N >> q;

    long long ans = 0;
    long long n = N;
    long long p = q;

    while (n > 1) {
        long long k = n / 2; // floor division
        if (p > k) {
            p = n - p + 1;
            n = n - k;
        } else {
            ans += n - k;
            p = k - p + 1;
            n = k;
        }
    }

    ans += 1;
    cout << ans << endl;

    return 0;
}
