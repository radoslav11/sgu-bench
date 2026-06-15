#include <iostream>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

int main() {
    long long x, y, d2, d1, b;
    cin >> x >> y >> d2 >> d1 >> b;

    vector<pair<long long, long long>> dir = {
        {1, 0},
        {0, 1},
        {-1, 1},
        {-1, 0},
        {0, -1},
        {1, -1}
    };

    for (int i = 0; i < d1; i++) {
        long long nx = -y;
        long long ny = x + y;
        x = nx;
        y = ny;
    }
    d2 = (d2 - d1 + 6) % 6;

    set<tuple<int, int, long long, long long>> current;
    current.insert(make_tuple(0, 0, 0, 0));

    const int BOUND = 200;

    for (int n = 1; n <= BOUND; n++) {
        set<tuple<int, int, long long, long long>> next_set;
        for (const auto& state : current) {
            int d = get<0>(state);
            int c = get<1>(state);
            long long dx = get<2>(state);
            long long dy = get<3>(state);

            if (c + 1 <= b) {
                int new_d = d;
                int new_c = c + 1;
                long long new_dx = dx + dir[d].first;
                long long new_dy = dy + dir[d].second;
                next_set.insert(make_tuple(new_d, new_c, new_dx, new_dy));
            }

            int new_d = (d + 1) % 6;
            int new_c = 1;
            long long new_dx = dx + dir[new_d].first;
            long long new_dy = dy + dir[new_d].second;
            next_set.insert(make_tuple(new_d, new_c, new_dx, new_dy));
        }

        for (const auto& state : next_set) {
            if (get<0>(state) == d2 && get<2>(state) == x && get<3>(state) == y) {
                cout << n << endl;
                return 0;
            }
        }

        current = next_set;
    }

    cout << -1 << endl;
    return 0;
}
