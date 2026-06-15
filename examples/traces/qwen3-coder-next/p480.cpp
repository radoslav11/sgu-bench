#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

int main() {
    int c, k;
    cin >> c >> k;

    vector<vector<long long>> a(c, vector<long long>(k));
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < k; j++) {
            cin >> a[i][j];
        }
    }

    vector<long long> T(k, 0);
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < c; i++) {
            T[j] += a[i][j];
        }
    }

    const long long BOUND = 1000000000000000LL;
    vector<long long> use(k, 0);
    long long need = 1;
    bool feasible = true;

    for (int j = k - 1; j >= 0; j--) {
        if (need > BOUND / 8) {
            need = BOUND + 1;
            break;
        }
        need *= 8;
        if (need <= T[j]) {
            use[j] = need;
            need = 0;
            break;
        } else {
            use[j] = T[j];
            need -= T[j];
        }
    }

    if (need > 0) {
        cout << "NO SOLUTION" << endl;
        return 0;
    }

    vector<tuple<int, int, long long>> result;
    for (int j = 0; j < k; j++) {
        long long rem = use[j];
        for (int i = 0; i < c; i++) {
            if (rem <= 0) break;
            long long take = min(rem, a[i][j]);
            if (take > 0) {
                result.push_back(make_tuple(i + 1, j, take));
                rem -= take;
            }
        }
    }

    cout << result.size() << endl;
    for (auto &t : result) {
        cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << endl;
    }

    return 0;
}
