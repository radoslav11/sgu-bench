#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Hor {
    long long y, x1, x2;
};

struct Ver {
    long long x, y1, y2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<Hor> hor;
    vector<Ver> ver;

    for (int i = 0; i < n; i++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        if (y1 == y2) {
            if (x1 > x2) swap(x1, x2);
            hor.push_back({y1, x1, x2});
        } else if (x1 == x2) {
            if (y1 > y2) swap(y1, y2);
            ver.push_back({x1, y1, y2});
        }
    }

    sort(hor.begin(), hor.end(), [](const Hor& a, const Hor& b) {
        return a.y < b.y;
    });

    long long ans = 0;

    for (int i = 0; i < hor.size(); i++) {
        for (int j = i + 1; j < hor.size(); j++) {
            if (hor[i].y == hor[j].y) {
                continue;
            }

            long long L = max(hor[i].x1, hor[j].x1);
            long long R = min(hor[i].x2, hor[j].x2);
            if (L >= R) {
                continue;
            }

            long long count = 0;
            for (int k = 0; k < ver.size(); k++) {
                if (ver[k].x < L || ver[k].x > R) {
                    continue;
                }
                if (ver[k].y1 <= hor[i].y && ver[k].y2 >= hor[j].y) {
                    count++;
                }
            }
            ans += count * (count - 1) / 2;
        }
    }

    cout << ans << '\n';

    return 0;
}
