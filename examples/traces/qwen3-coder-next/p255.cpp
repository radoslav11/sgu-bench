#include <bits/stdc++.h>
using namespace std;

const int MAX_M = 1000;

int main() {
    vector<unsigned long long> S;
    for (int a = 2; a < 60; a++) {
        for (int b = 1; b < a; b++) {
            for (int c = 0; c < b; c++) {
                unsigned long long x = (1ULL << a) | (1ULL << b) | (1ULL << c);
                S.push_back(x);
            }
        }
    }
    sort(S.begin(), S.end());

    map<unsigned long long, int> diff_map;
    for (auto x : S) {
        unsigned long long L = (x + 1) / 2;
        unsigned long long R = x;
        diff_map[L]++;
        diff_map[R]--;
    }

    vector<unsigned long long> keys;
    for (auto& p : diff_map) {
        keys.push_back(p.first);
    }
    sort(keys.begin(), keys.end());

    vector<long long> freq(MAX_M + 1, 0);
    unsigned long long current = 0;
    unsigned long long prev_k = 0;

    for (int i = 0; i < keys.size(); i++) {
        unsigned long long k = keys[i];
        if (prev_k < k) {
            if (current <= MAX_M) {
                freq[current] += (k - prev_k);
            }
        }
        current += diff_map[k];
        prev_k = k;
    }

    int N;
    cin >> N;
    while (N--) {
        long long M;
        cin >> M;
        if (M > MAX_M) {
            cout << "NO" << endl;
        } else {
            if (freq[M] == 1) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }
    return 0;
}
