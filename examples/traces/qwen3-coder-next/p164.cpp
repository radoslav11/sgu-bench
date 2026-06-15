#include <iostream>
#include <vector>
#include <set>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> g[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        set<int> airlines;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            airlines.insert(g[i][j]);
        }
        if (static_cast<int>(airlines.size()) <= (m + 1) / 2) {
            cout << airlines.size() << endl;
            bool first = true;
            for (int a : airlines) {
                if (!first) cout << " ";
                cout << a;
                first = false;
            }
            cout << endl;
            return 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            set<int> airlines;
            for (int k = 0; k < n; k++) {
                if (k != i) airlines.insert(g[i][k]);
                if (k != j) airlines.insert(g[j][k]);
            }
            if (static_cast<int>(airlines.size()) <= (m + 1) / 2) {
                cout << airlines.size() << endl;
                bool first = true;
                for (int a : airlines) {
                    if (!first) cout << " ";
                    cout << a;
                    first = false;
                }
                cout << endl;
                return 0;
            }
        }
    }

    cout << -1 << endl;
    return 0;
}
