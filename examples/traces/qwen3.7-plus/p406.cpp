#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Sequence {
    int k;
    vector<int> elements;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<Sequence> db(n);
    for (int i = 0; i < n; ++i) {
        cin >> db[i].k;
        db[i].elements.resize(db[i].k);
        for (int j = 0; j < db[i].k; ++j) {
            cin >> db[i].elements[j];
        }
    }

    for (int i = 0; i < m; ++i) {
        int l;
        cin >> l;
        vector<int> query(l);
        for (int j = 0; j < l; ++j) {
            cin >> query[j];
        }

        vector<int> match_indices;
        for (int i_seq = 0; i_seq < n; ++i_seq) {
            bool ok = true;
            for (int q : query) {
                int target = abs(q);
                bool found = false;
                for (int e : db[i_seq].elements) {
                    if (e == target) {
                        found = true;
                        break;
                    }
                }
                if (q > 0 && !found) {
                    ok = false;
                    break;
                }
                if (q < 0 && found) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                match_indices.push_back(i_seq);
            }
        }

        cout << match_indices.size() << "\n";
        for (int idx : match_indices) {
            cout << db[idx].k;
            for (int j = 0; j < db[idx].k; ++j) {
                cout << " " << db[idx].elements[j];
            }
            cout << "\n";
        }
    }

    return 0;
}
