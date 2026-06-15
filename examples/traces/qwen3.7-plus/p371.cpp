#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long N, M;
    if (!(cin >> N >> M)) return 0;

    long long K = M - N + 1;
    if (K < 1) {
        cout << "No solution\n";
        return 0;
    }

    long long min_R, max_R;
    if (K == 1) {
        min_R = max(0LL, M - 10);
        max_R = min(M - 3, M / 2);
    } else {
        min_R = max(0LL, M - 10 * K);
        max_R = min(M - 3 * K, (M - 2 * K + 2) / 2);
    }

    if (min_R > max_R) {
        cout << "No solution\n";
        return 0;
    }

    long long R = max_R;
    long long sum_v = M - R;

    vector<long long> v(K, 3);
    long long rem = sum_v - 3 * K;
    for (int i = 0; i < K && rem > 0; ++i) {
        long long add = min(7LL, rem);
        v[i] += add;
        rem -= add;
    }

    vector<vector<long long>> lines(K);
    long long c = 1;
    for (int i = 0; i < K; ++i) {
        if (i == 0) {
            for (int j = 0; j < v[i]; ++j) {
                lines[i].push_back(c + j);
            }
            c += v[i];
        } else {
            for (int j = 0; j < v[i]; ++j) {
                lines[i].push_back(c - 1 + j);
            }
            c += v[i] - 1;
        }
    }

    long long V_circ = c - 1;

    set<long long> transfers;
    if (K >= 2) {
        for (int i = 0; i < K - 1; ++i) {
            transfers.insert(lines[i].back());
        }
    }

    vector<long long> non_transfers;
    for (int i = 0; i < K; ++i) {
        for (long long s : lines[i]) {
            if (transfers.find(s) == transfers.end()) {
                non_transfers.push_back(s);
            }
        }
    }

    if ((long long)non_transfers.size() < R) {
        cout << "No solution\n";
        return 0;
    }

    vector<pair<long long, long long>> radials;
    for (int j = 0; j < R; ++j) {
        long long u = non_transfers[j];
        long long v_station = V_circ + 1 + j;
        radials.push_back({u, v_station});
    }

    cout << K << "\n";
    for (int i = 0; i < K; ++i) {
        cout << v[i];
        for (long long s : lines[i]) {
            cout << " " << s;
        }
        cout << "\n";
    }
    cout << R << "\n";
    for (int j = 0; j < R; ++j) {
        cout << radials[j].first << " " << radials[j].second << "\n";
    }

    return 0;
}
