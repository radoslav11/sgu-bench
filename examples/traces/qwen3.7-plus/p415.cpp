#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

const int MAX_X = 10005;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, x;
    if (!(cin >> n >> x)) return 0;
    
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    vector<bitset<MAX_X>> pref(n + 1);
    pref[0][0] = 1;
    for (int i = 1; i <= n; ++i) {
        pref[i] = pref[i - 1] | (pref[i - 1] << a[i]);
    }

    vector<bitset<MAX_X>> suff(n + 2);
    suff[n + 1][0] = 1;
    for (int i = n; i >= 1; --i) {
        suff[i] = suff[i + 1] | (suff[i + 1] << a[i]);
    }

    vector<int> result;
    for (int i = 1; i <= n; ++i) {
        bool possible = false;
        for (int v = 0; v <= x; ++v) {
            if (pref[i - 1][v] && suff[i + 1][x - v]) {
                possible = true;
                break;
            }
        }
        if (!possible) {
            result.push_back(a[i]);
        }
    }

    cout << result.size() << "\n";
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << (i + 1 == result.size() ? "" : " ");
    }
    cout << "\n";

    return 0;
}
