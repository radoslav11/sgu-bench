#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<pair<long long, long long>> segments;
    segments.reserve(1000);

    char op;
    long long L, R;

    while (cin >> op >> L >> R) {
        if (op == '+') {
            int ans = 0;
            for (const auto& seg : segments) {
                if (L <= seg.first && seg.second <= R) {
                    ++ans;
                }
            }

            cout << ans << '\n';
            segments.push_back({L, R});
        } else if (op == '-') {
            for (auto it = segments.begin(); it != segments.end(); ++it) {
                if (it->first == L && it->second == R) {
                    segments.erase(it);
                    break;
                }
            }
        }
    }

    return 0;
}
