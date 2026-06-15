#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, f;
    cin >> n >> f;
    vector<int> e(n);
    for (int i = 0; i < n; i++) {
        cin >> e[i];
    }

    vector<bool> active(101, false);
    for (int i = 0; i < n; i++) {
        active[e[i]] = true;
    }

    vector<int> stops;
    int current = f;

    while (true) {
        int target = -1;
        for (int i = 0; i < n; i++) {
            if (active[e[i]]) {
                target = e[i];
                break;
            }
        }
        if (target == -1) {
            break;
        }

        vector<int> candidates;
        if (target > current) {
            for (int x = current + 1; x <= target; x++) {
                if (active[x]) {
                    candidates.push_back(x);
                }
            }
        } else {
            for (int x = current - 1; x >= target; x--) {
                if (active[x]) {
                    candidates.push_back(x);
                }
            }
        }

        for (int stop : candidates) {
            stops.push_back(stop);
            active[stop] = false;
        }
        current = candidates.back();
    }

    for (int i = 0; i < stops.size(); i++) {
        if (i > 0) {
            cout << " ";
        }
        cout << stops[i];
    }
    cout << endl;

    return 0;
}
