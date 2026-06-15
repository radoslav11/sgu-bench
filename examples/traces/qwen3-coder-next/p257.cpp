#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int P, O, S;
    cin >> P >> O >> S;
    int N;
    cin >> N;
    vector<string> crystals(N);
    for (int i = 0; i < N; i++) {
        cin >> crystals[i];
    }

    vector<int> r(3);
    r[0] = P;
    r[1] = O;
    r[2] = S;

    vector<int> indices(N);
    for (int i = 0; i < N; i++) {
        indices[i] = i;
    }

    sort(indices.begin(), indices.end(), [&](int i, int j) {
        int val_i = max({
            (crystals[i][0] == 'B' ? 2 : 1),
            (crystals[i][1] == 'B' ? 2 : 1),
            (crystals[i][2] == 'B' ? 2 : 1)
        });
        int val_j = max({
            (crystals[j][0] == 'B' ? 2 : 1),
            (crystals[j][1] == 'B' ? 2 : 1),
            (crystals[j][2] == 'B' ? 2 : 1)
        });
        if (val_i != val_j) {
            return val_i > val_j;
        }
        return i < j;
    });

    string assignment(N, ' ');

    for (int i = 0; i < N; i++) {
        int idx = indices[i];
        string& s = crystals[idx];
        int best_val = 0;
        int best_cred = -1;

        if (r[0] > 0) {
            int val = (s[0] == 'B' ? 2 : 1);
            if (val > best_val) {
                best_val = val;
                best_cred = 0;
            } else if (val == best_val) {
                if (best_cred == -1 || r[0] > r[best_cred]) {
                    best_cred = 0;
                }
            }
        }
        if (r[1] > 0) {
            int val = (s[1] == 'B' ? 2 : 1);
            if (val > best_val) {
                best_val = val;
                best_cred = 1;
            } else if (val == best_val) {
                if (best_cred == -1 || r[1] > r[best_cred]) {
                    best_cred = 1;
                }
            }
        }
        if (r[2] > 0) {
            int val = (s[2] == 'B' ? 2 : 1);
            if (val > best_val) {
                best_val = val;
                best_cred = 2;
            } else if (val == best_val) {
                if (best_cred == -1 || r[2] > r[best_cred]) {
                    best_cred = 2;
                }
            }
        }

        if (best_cred == -1) {
            best_cred = 0;
        }

        int val;
        if (best_cred == 0) {
            val = (s[0] == 'B' ? 2 : 1);
        } else if (best_cred == 1) {
            val = (s[1] == 'B' ? 2 : 1);
        } else {
            val = (s[2] == 'B' ? 2 : 1);
        }

        r[best_cred] = max(0, r[best_cred] - val);
        if (best_cred == 0) {
            assignment[idx] = 'P';
        } else if (best_cred == 1) {
            assignment[idx] = 'O';
        } else {
            assignment[idx] = 'S';
        }
    }

    if (r[0] <= 0 && r[1] <= 0 && r[2] <= 0) {
        cout << assignment << endl;
    } else {
        cout << "no solution" << endl;
    }

    return 0;
}
