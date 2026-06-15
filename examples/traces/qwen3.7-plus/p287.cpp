#include <iostream>

using namespace std;

long long get_S(long long L, long long c, long long target) {
    long long sum = 0;
    long long max_j = L / (c + 1);
    for (long long j = 0; j <= max_j; ++j) {
        long long n = L - j * c;
        long long k = j;
        if (k > n) break;
        long long limit = k;
        if (k > n - k) limit = n - k;
        
        __int128 res = 1;
        for (long long i = 1; i <= limit; ++i) {
            res = res * (n - i + 1);
            res /= i;
        }
        
        if (res > target - sum) {
            return target;
        }
        sum += (long long)res;
        if (sum >= target) return sum;
    }
    return sum;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    long long q, c;
    if (cin >> q >> c) {
        long long target = q + 1;
        long long low = c;
        long long high = q + c + 1;
        long long ans = high;
        
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (get_S(mid, c, target) >= target) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        cout << ans << "\n";
    }
    return 0;
}
