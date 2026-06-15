#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

struct Person {
    int s;
    int b;
    int idx;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Person> people(n);
    for (int i = 0; i < n; i++) {
        cin >> people[i].s >> people[i].b;
        people[i].idx = i + 1; // 1-based indexing
    }

    // Sort by strength ascending, and for same strength, beauty descending
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        if (a.s != b.s) {
            return a.s < b.s;
        }
        return a.b > b.b;
    });

    vector<int> beauties;
    for (int i = 0; i < n; i++) {
        beauties.push_back(people[i].b);
    }

    // LIS with reconstruction for strictly increasing subsequence
    vector<int> dp; // dp[i] = smallest ending beauty for LIS of length i+1
    vector<int> pos(n, -1); // pos[len] = index in beauties array for the ending of LIS of length len+1
    vector<int> parent(n, -1); // parent[i] = previous index in LIS ending at i

    for (int i = 0; i < n; i++) {
        int x = beauties[i];
        // Find the first position where dp[pos] >= x (for non-decreasing, but works for strict increase with our sorting)
        auto it = lower_bound(dp.begin(), dp.end(), x);
        int idx = it - dp.begin();

        if (idx == dp.size()) {
            dp.push_back(x);
        } else {
            dp[idx] = x;
        }
        pos[idx] = i;

        if (idx > 0) {
            parent[i] = pos[idx - 1];
        }
    }

    // Reconstruct the LIS
    vector<int> result_indices;
    int current = pos[dp.size() - 1];
    while (current != -1) {
        result_indices.push_back(people[current].idx);
        current = parent[current];
    }

    // Output
    cout << dp.size() << "\n";
    for (int i = 0; i < result_indices.size(); i++) {
        if (i > 0) cout << " ";
        cout << result_indices[i];
    }
    if (dp.size() > 0) {
        cout << "\n";
    }

    return 0;
}
