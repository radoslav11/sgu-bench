#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        if (a[i] < 0 || a[i] >= n) {
            cout << "NO" << endl;
            return 0;
        }
    }

    vector<int> d(n + 1);
    for (int i = 1; i <= n; i++) {
        d[i] = n - 1 - a[i];
    }

    set<int> s;
    for (int i = 1; i <= n; i++) {
        s.insert(i);
    }

    vector<int> res;
    for (int i = n; i >= 1; i--) {
        if (d[i] > (int)s.size()) {
            cout << "NO" << endl;
            return 0;
        }
        for (int j = 0; j < d[i]; j++) {
            res.push_back(-(*s.begin()));
            s.erase(s.begin());
        }
        res.push_back(i);
        s.erase(i);
    }

    if (!s.empty()) {
        cout << "NO" << endl;
        return 0;
    }

    cout << "YES" << endl;
    for (int i = 0; i < res.size(); i++) {
        if (i > 0) cout << " ";
        cout << res[i];
    }
    cout << endl;

    return 0;
}
