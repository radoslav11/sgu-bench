#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    ll L;
    cin >> n >> L;
    vector<ll> a(n), b(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }

    vector<pair<ll, int>> houses_sorted, offices_sorted;
    for (int i = 0; i < n; i++) {
        houses_sorted.push_back({a[i], i});
    }
    for (int i = 0; i < n; i++) {
        offices_sorted.push_back({b[i], i});
    }
    sort(houses_sorted.begin(), houses_sorted.end());
    sort(offices_sorted.begin(), offices_sorted.end());

    vector<ll> a_sorted(n), b_sorted(n);
    for (int i = 0; i < n; i++) {
        a_sorted[i] = houses_sorted[i].first;
        b_sorted[i] = offices_sorted[i].first;
    }

    vector<pair<ll, int>> candidates;
    for (int j = 0; j < n; j++) {
        ll diff = abs(a_sorted[0] - b_sorted[j]);
        ll circular_diff = min(diff, L - diff);
        candidates.push_back({circular_diff, j});
    }
    sort(candidates.begin(), candidates.end());

    ll best_cost = 1e18;
    int best_shift = 0;
    int try_count = min(10, n);
    for (int t = 0; t < try_count; t++) {
        int j = candidates[t].second;
        int shift = (j - 0 + n) % n;

        ll cost = 0;
        for (int i = 0; i < n; i++) {
            int office_index = (i + shift) % n;
            ll diff = abs(a_sorted[i] - b_sorted[office_index]);
            cost += min(diff, L - diff);
        }
        if (cost < best_cost) {
            best_cost = cost;
            best_shift = shift;
        }
    }

    vector<int> ans(n);
    for (int i = 0; i < n; i++) {
        int office_sorted_index = (i + best_shift) % n;
        int house_original_index = houses_sorted[i].second;
        int office_original_index = offices_sorted[office_sorted_index].second;
        ans[house_original_index] = office_original_index + 1;
    }

    cout << best_cost << "\n";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}
