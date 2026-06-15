#include <bits/stdc++.h>
using namespace std;

int targetLen;
unordered_map<string, bool> memo;

int addedDeficit(const string& s, char c) {
    string t = s + c;
    int oldLen = (int)s.size();

    for (int len = oldLen; len >= 1; --len) {
        bool ok = false;

        for (int start = 0; start + len <= oldLen; ++start) {
            bool same = true;
            for (int j = 0; j < len; ++j) {
                if (s[start + j] != t[t.size() - len + j]) {
                    same = false;
                    break;
                }
            }
            if (same) {
                ok = true;
                break;
            }
        }

        if (ok) return len;
    }

    return 0;
}

bool can(const string& s, int deficitLeft) {
    if (deficitLeft == 0) return true;
    if ((int)s.size() == targetLen) return false;
    if (deficitLeft < 0) return false;

    int rem = targetLen - (int)s.size();

    // Crude upper bound: at position i, deficit cannot exceed i - 1.
    int maxPossible = 0;
    int curLen = (int)s.size();
    for (int i = 1; i <= rem; ++i) {
        maxPossible += curLen + i - 1;
    }
    if (deficitLeft > maxPossible) return false;

    string key = s + "#" + to_string(deficitLeft);
    if (memo.count(key)) return memo[key];

    for (char c = 'a'; c <= 'z'; ++c) {
        int d = addedDeficit(s, c);
        if (d <= deficitLeft) {
            if (can(s + c, deficitLeft - d)) {
                return memo[key] = true;
            }
        }
    }

    return memo[key] = false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    targetLen = 1;
    while (targetLen * (targetLen + 1) / 2 < n) {
        ++targetLen;
    }

    int maxSubstrings = targetLen * (targetLen + 1) / 2;
    int deficit = maxSubstrings - n;

    string ans;

    for (int pos = 0; pos < targetLen; ++pos) {
        for (char c = 'a'; c <= 'z'; ++c) {
            int d = addedDeficit(ans, c);
            if (d <= deficit && can(ans + c, deficit - d)) {
                ans += c;
                deficit -= d;
                break;
            }
        }
    }

    cout << ans << '\n';

    return 0;
}
