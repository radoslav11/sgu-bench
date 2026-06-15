#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    long long M = *max_element(a.begin(), a.end());
    long long total_cloning = 0;
    for (int i = 0; i < n; i++) {
        long long d = M - a[i];
        total_cloning += __builtin_popcountll(d);
    }
    long long total_days = M + total_cloning;
    cout << total_days << '\n';

    if (total_days <= 1000) {
        vector<long long> current = a;
        for (int r = 0; r < M; r++) {
            if (r < M - 1) {
                for (int i = 0; i < n; i++) {
                    if (current[i] == 1) {
                        current[i] *= 2;
                        cout << "science mission to the planet " << (i+1) << '\n';
                    }
                }
            }
            for (int i = 0; i < n; i++) {
                current[i]--;
            }
            cout << "flying mission\n";
        }
    }

    return 0;
}
