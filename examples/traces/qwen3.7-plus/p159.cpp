#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string to_string_b(const vector<int>& x, int b) {
    string res = "";
    for (int i = (int)x.size() - 1; i >= 0; --i) {
        if (x[i] < 10) res += (char)('0' + x[i]);
        else res += (char)('A' + x[i] - 10);
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int b, n;
    if (!(cin >> b >> n)) return 0;

    vector<vector<int>> candidates;
    for (int d = 0; d < b; ++d) {
        if ((d * d) % b == d) {
            candidates.push_back({d});
        }
    }

    for (int step = 1; step < n; ++step) {
        vector<vector<int>> next_candidates;
        for (const auto& x : candidates) {
            long long carry = 0;
            int S_step = 0;
            for (int j = 0; j <= step; ++j) {
                long long sum = carry;
                int start_i = (j >= step) ? j - step + 1 : 0;
                int end_i = (j < step) ? j : step - 1;
                for (int i = start_i; i <= end_i; ++i) {
                    sum += (long long)x[i] * x[j - i];
                }
                if (j == step) {
                    S_step = sum % b;
                }
                carry = sum / b;
            }
            
            int A = (1 - 2 * x[0]) % b;
            if (A < 0) A += b;
            int C = S_step;
            
            int d_val = -1;
            for (int d = 0; d < b; ++d) {
                if ((A * d) % b == C) {
                    d_val = d;
                    break;
                }
            }
            
            if (d_val != -1) {
                vector<int> next_x = x;
                next_x.push_back(d_val);
                next_candidates.push_back(next_x);
            }
        }
        candidates = move(next_candidates);
    }

    vector<string> results;
    for (const auto& x : candidates) {
        if (n == 1) {
            results.push_back(to_string_b(x, b));
        } else {
            if (x[n - 1] != 0) {
                results.push_back(to_string_b(x, b));
            }
        }
    }

    sort(results.begin(), results.end());

    cout << results.size() << "\n";
    for (const string& s : results) {
        cout << s << "\n";
    }

    return 0;
}
