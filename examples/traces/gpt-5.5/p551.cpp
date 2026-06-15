#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t1, t2;
    cin >> n >> t1 >> t2;

    bool active1 = true, active2 = true;
    long long next1 = t1, next2 = t2;

    int written = 0;
    long long lastTime = 0;

    while (active1 || active2) {
        long long cur = LLONG_MAX;
        if (active1) cur = min(cur, next1);
        if (active2) cur = min(cur, next2);

        bool finish1 = active1 && next1 == cur;
        bool finish2 = active2 && next2 == cur;

        if (finish1) written++;
        if (finish2) written++;

        lastTime = cur;

        if (finish1) {
            if (written < n) {
                next1 = cur + t1;
            } else {
                active1 = false;
            }
        }

        if (finish2) {
            if (written < n) {
                next2 = cur + t2;
            } else {
                active2 = false;
            }
        }
    }

    cout << written << ' ' << lastTime << '\n';

    return 0;
}
