#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    long long B;
    cin >> B;

    if (B == 1) {
        cout << s << "\n";
        return 0;
    }

    vector<uint32_t> a;
    for (size_t i = 0; i < s.length(); i += 9) {
        size_t len = min((size_t)9, s.length() - i);
        uint32_t val = 0;
        for (size_t j = i; j < i + len; ++j) {
            val = val * 10 + (s[j] - '0');
        }
        a.push_back(val);
    }

    vector<uint32_t> digits;
    int start = 0;
    while (true) {
        uint64_t rem = 0;
        for (int i = start; i < (int)a.size(); ++i) {
            uint64_t cur = rem * 1000000000ULL + a[i];
            a[i] = static_cast<uint32_t>(cur / B);
            rem = cur % B;
        }
        digits.push_back(static_cast<uint32_t>(rem));
        
        bool all_zero = true;
        for (int i = start; i < (int)a.size(); ++i) {
            if (a[i] != 0) {
                start = i;
                all_zero = false;
                break;
            }
        }
        if (all_zero) break;
    }

    long long dp0_next = 0;
    long long dp1_next = 1;

    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        long long v0 = digits[i];
        long long cost0_0 = v0 + dp0_next;
        long long cost0_1 = (B - v0) + dp1_next;
        long long dp0_curr = min(cost0_0, cost0_1);
        
        long long v1 = digits[i] + 1;
        long long cost1_0 = v1 + dp0_next;
        long long cost1_1 = (B - v1) + dp1_next;
        long long dp1_curr = min(cost1_0, cost1_1);
        
        dp0_next = dp0_curr;
        dp1_next = dp1_curr;
    }

    cout << dp0_next << "\n";

    return 0;
}
