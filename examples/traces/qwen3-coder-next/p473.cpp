#include <iostream>
#include <vector>
using namespace std;

vector<long long> primes = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
long long K;
long long ans = 1e18;
vector<int> factors;

void dfs_fac(int rem, int max_factor) {
    if (rem == 1) {
        long long cur = 1;
        for (int i = 0; i < (int)factors.size(); i++) {
            long long p = 1;
            int exp = factors[i] - 1;
            for (int j = 0; j < exp; j++) {
                if (cur > ans / primes[i]) {
                    cur = ans + 1;
                    break;
                }
                cur *= primes[i];
            }
            if (cur > ans) 
                break;
        }
        if (cur < ans) {
            ans = cur;
        }
        return;
    }

    for (int f = max_factor; f >= 2; f--) {
        if (rem % f == 0) {
            factors.push_back(f);
            dfs_fac(rem / f, f);
            factors.pop_back();
        }
    }
}

int main() {
    cin >> K;
    dfs_fac(K, K);
    if (ans == (long long)1e18) {
        cout << 0 << endl;
    } else {
        cout << ans << endl;
    }
    return 0;
}
