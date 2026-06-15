#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> type(n + 1, 0);
    vector<int> val(n + 1, 0);

    for (int i = 2; i <= n; ++i) {
        char c;
        int v;
        cin >> c >> v;
        if (c == 'd') {
            type[i] = 0;
            val[i] = v;
        } else {
            type[i] = 1;
            val[i] = v;
        }
    }

    using Element = pair<int, int>;
    priority_queue<Element, vector<Element>, greater<Element>> pq;

    for (int i = 2; i <= n; ++i) {
        if (type[i] == 0) {
            pq.push({val[i], i});
        } else {
            if (i < n) {
                int limit = val[i] - 1;
                while (pq.size() > (size_t)limit) {
                    pq.pop();
                }
            }
        }
    }

    if (pq.size() >= (size_t)val[n]) {
        long long total_gold = 0;
        vector<int> dragons;
        while (!pq.empty()) {
            total_gold += pq.top().first;
            dragons.push_back(pq.top().second);
            pq.pop();
        }
        sort(dragons.begin(), dragons.end());
        cout << total_gold << "\n";
        cout << dragons.size() << "\n";
        for (size_t i = 0; i < dragons.size(); ++i) {
            cout << dragons[i] << (i + 1 == dragons.size() ? "" : " ");
        }
        cout << "\n";
    } else {
        cout << -1 << "\n";
    }

    return 0;
}
