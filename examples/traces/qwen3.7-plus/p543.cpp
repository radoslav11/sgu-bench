#include <iostream>

using namespace std;

bool check(long long T, long long C3, long long C2, long long r) {
    long long X_max = r / 3;
    if (C3 > T * X_max) return false;
    
    long long q = C3 / T;
    long long rem = C3 % T;
    long long min_drop = 0;
    
    if (r % 2 == 0) {
        min_drop = rem * ((3 * (q + 1) + 1) / 2) + (T - rem) * ((3 * q + 1) / 2);
    } else {
        min_drop = rem * (q + 1 + (q + 1) / 2) + (T - rem) * (q + q / 2);
    }
    
    long long max_2s = T * (r / 2) - min_drop;
    return max_2s >= C2;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    long long r;
    if (!(cin >> n >> r)) return 0;
    
    long long C2 = 0, C3 = 0;
    long long sum_a = 0;
    for (int i = 0; i < n; ++i) {
        long long a;
        cin >> a;
        sum_a += a;
        if (a % 2 == 0) {
            C2 += a / 2;
        } else {
            C2 += (a - 3) / 2;
            C3 += 1;
        }
    }
    
    long long low = 1, high = sum_a;
    long long ans = high;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (check(mid, C3, C2, r)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    cout << ans << "\n";
    return 0;
}
