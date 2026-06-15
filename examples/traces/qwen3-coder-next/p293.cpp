#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    string s;
    cin >> s;
    int m = 2 * n - 1;

    for (int k = 1; k <= m; k += 2) {
        int cntQ = 0, cntC = 0;
        for (int i = 0; i < k; i++) {
            if (s[i] == 'Q') 
                cntQ++;
            else 
                cntC++;
        }
        int odd = 0;
        if (cntQ % 2) odd++;
        if (cntC % 2) odd++;
        if (odd > 1) {
            cout << "He" << endl;
            return 0;
        }
    }

    string cur = s;
    vector<pair<int, int>> moves;

    for (int i = 0; i < m; i++) {
        if (i % 2 == 0) {
            int len = i + 1;
            bool is_palindrome = true;
            for (int j = 0; j < len / 2; j++) {
                if (cur[j] != cur[len - 1 - j]) {
                    is_palindrome = false;
                    break;
                }
            }
            if (is_palindrome) {
                moves.push_back(make_pair(0, 0));
            } else {
                int j = 0;
                while (j < len / 2 && cur[j] == cur[len - 1 - j]) {
                    j++;
                }
                int k_index = -1;
                int center = len / 2;

                if (center > j && center < len - 1 && cur[center] == cur[len - 1 - j]) {
                    swap(cur[j], cur[center]);
                    moves.push_back(make_pair(j + 1, center + 1));
                } else {
                    for (int p = j + 1; p < len - 1; p++) {
                        if (cur[p] == cur[len - 1 - j]) {
                            k_index = p;
                            break;
                        }
                    }
                    if (k_index != -1) {
                        swap(cur[j], cur[k_index]);
                        moves.push_back(make_pair(j + 1, k_index + 1));
                    } else {
                        for (int p = j + 1; p < len - 1; p++) {
                            if (cur[p] == cur[j]) {
                                k_index = p;
                                break;
                            }
                        }
                        if (k_index != -1) {
                            swap(cur[k_index], cur[len - 1 - j]);
                            moves.push_back(make_pair(k_index + 1, len - 1 - j + 1));
                        } else {
                            moves.push_back(make_pair(0, 0));
                        }
                    }
                }
            }
        } else {
            moves.push_back(make_pair(0, 0));
        }
    }

    cout << "Qc" << endl;
    for (auto &p : moves) {
        cout << p.first << " " << p.second << endl;
    }

    return 0;
}
