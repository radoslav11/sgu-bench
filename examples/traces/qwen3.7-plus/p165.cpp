#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

long long parse_height(const string& s) {
    long long res = 0;
    long long frac = 0;
    long long frac_mult = 1;
    bool decimal = false;
    for (char c : s) {
        if (c == '.') {
            decimal = true;
        } else {
            if (!decimal) {
                res = res * 10 + (c - '0');
            } else {
                if (frac_mult < 1000000) {
                    frac = frac * 10 + (c - '0');
                    frac_mult *= 10;
                }
            }
        }
    }
    while (frac_mult < 1000000) {
        frac *= 10;
        frac_mult *= 10;
    }
    return res * 1000000LL + frac;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<pair<long long, int>> neg;
    vector<pair<long long, int>> pos;

    for (int i = 1; i <= n; ++i) {
        string s;
        cin >> s;
        long long h = parse_height(s);
        long long diff = h - 2000000LL;
        if (diff <= 0) {
            neg.push_back({diff, i});
        } else {
            pos.push_back({diff, i});
        }
    }

    sort(neg.begin(), neg.end());
    sort(pos.begin(), pos.end());

    long long S = 0;
    vector<int> order;
    order.reserve(n);

    int neg_idx = (int)neg.size() - 1;
    int pos_idx = 0;

    while (neg_idx >= 0 || pos_idx < (int)pos.size()) {
        if (S >= 0) {
            auto p = neg[neg_idx];
            S += p.first;
            order.push_back(p.second);
            neg_idx--;
        } else {
            auto p = pos[pos_idx];
            S += p.first;
            order.push_back(p.second);
            pos_idx++;
        }
    }

    cout << "yes\n";
    for (int i = 0; i < n; ++i) {
        cout << order[i] << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
