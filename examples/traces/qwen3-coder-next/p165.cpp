#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<pair<long long, int>> a;
    for (int i = 0; i < N; i++) {
        double x;
        cin >> x;
        long long micrometers = llround(x * 1000000.0);
        long long dev = micrometers - 2000000;
        a.push_back({dev, i + 1});
    }

    sort(a.begin(), a.end());

    vector<int> order;
    long long cur = 0;
    int l = 0, r = N - 1;
    while (l <= r) {
        if (cur <= 0) {
            order.push_back(a[r].second);
            cur += a[r].first;
            r--;
        } else {
            order.push_back(a[l].second);
            cur += a[l].first;
            l++;
        }
    }

    cout << "yes" << endl;
    for (int i = 0; i < order.size(); i++) {
        if (i > 0) cout << " ";
        cout << order[i];
    }
    cout << endl;

    return 0;
}
