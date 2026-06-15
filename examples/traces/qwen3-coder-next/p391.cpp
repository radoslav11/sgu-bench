#include <iostream>
#include <set>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, k;
    cin >> n >> m >> k;

    set<int> rows, cols;
    for (int i = 0; i < k; i++) {
        int x, y;
        cin >> x >> y;
        rows.insert(x);
        cols.insert(y);
    }

    // Check row symmetry
    if (!rows.empty()) {
        int minR = *rows.begin();
        int maxR = *rows.rbegin();
        for (int r : rows) {
            if (rows.find(minR + maxR - r) == rows.end()) {
                cout << "NO" << endl;
                return 0;
            }
        }
    }

    // Check column symmetry
    if (!cols.empty()) {
        int minC = *cols.begin();
        int maxC = *cols.rbegin();
        for (int c : cols) {
            if (cols.find(minC + maxC - c) == cols.end()) {
                cout << "NO" << endl;
                return 0;
            }
        }
    }

    if (k == (int)rows.size() * (int)cols.size()) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

    return 0;
}
