#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s1, s2;
    if (!(cin >> s1 >> s2)) return 0;

    if (s1.length() != s2.length()) {
        cout << -1 << "\n";
        return 0;
    }

    vector<int> p1, p2;
    for (int i = 0; i < (int)s1.length(); ++i) {
        if (s1[i] == '+') {
            p1.push_back(i);
        }
    }
    for (int i = 0; i < (int)s2.length(); ++i) {
        if (s2[i] == '+') {
            p2.push_back(i);
        }
    }

    if (p1.size() != p2.size()) {
        cout << -1 << "\n";
        return 0;
    }

    long long ans = 0;
    for (size_t i = 0; i < p1.size(); ++i) {
        ans += abs(p1[i] - p2[i]);
    }

    cout << ans << "\n";
    return 0;
}
