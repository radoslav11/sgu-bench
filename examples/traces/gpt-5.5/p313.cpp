#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long L;
    cin >> n >> L;

    vector<pair<long long,int>> houses(n), offices(n);

    for (int i = 0; i < n; ++i) {
        cin >> houses[i].first;
        houses[i].second = i;
    }
    for (int i = 0; i < n; ++i) {
        cin >> offices[i].first;
        offices[i].second = i;
    }

    sort(houses.begin(), houses.end());
    sort(offices.begin(), offices.end());

    vector<pair<long long,int>> events;
    events.reserve(2 * n);

    for (auto &p : houses) events.push_back({p.first, +1});
    for (auto &p : offices) events.push_back({p.first, -1});

    sort(events.begin(), events.end());

    vector<long long> weight(2 * n + 1, 0);
    int shiftBase = n;

    int pref = 0;
    int m = (int)events.size();

    for (int i = 0; i < m; ) {
        long long pos = events[i].first;
        int delta = 0;

        while (i < m && events[i].first == pos) {
            delta += events[i].second;
            ++i;
        }

        pref += delta;

        long long nextPos = (i < m ? events[i].first : events[0].first + L);
        long long len = nextPos - pos;

        if (len > 0) {
            weight[pref + shiftBase] += len;
        }
    }

    long long cur = 0;
    int c = -n;

    for (int v = -n; v <= n; ++v) {
        cur += weight[v + shiftBase];
        if (cur * 2 >= L) {
            c = v;
            break;
        }
    }

    int s = ((n - c) % n + n) % n;

    vector<int> answer(n);
    long long total = 0;

    for (int i = 0; i < n; ++i) {
        int j = (i + s) % n;

        long long a = houses[i].first;
        long long b = offices[j].first;

        long long diff = llabs(a - b);
        long long dist = min(diff, L - diff);

        total += dist;
        answer[houses[i].second] = offices[j].second + 1;
    }

    cout << total << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';

    return 0;
}
