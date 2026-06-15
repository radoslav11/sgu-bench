#include <bits/stdc++.h>
using namespace std;

struct Student {
    int id;
    int p;
    int w;
    int deadline;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K;
    cin >> K;

    vector<int> cap(K), Q(K);
    int N = 0;

    for (int i = 0; i < K; ++i) {
        cin >> cap[i];
        N += cap[i];
    }

    for (int i = 0; i < K; ++i) {
        cin >> Q[i];
    }

    vector<int> P(N), W(N);
    for (int i = 0; i < N; ++i) cin >> P[i];
    for (int i = 0; i < N; ++i) cin >> W[i];

    vector<pair<int, int>> zones;
    for (int i = 0; i < K; ++i) {
        zones.push_back({Q[i], i});
    }
    sort(zones.begin(), zones.end());

    vector<int> sortedQ(K);
    vector<int> pref(K + 1, 0);
    vector<int> slots;

    for (int i = 0; i < K; ++i) {
        int zoneId = zones[i].second;
        sortedQ[i] = zones[i].first;
        pref[i + 1] = pref[i] + cap[zoneId];

        for (int j = 0; j < cap[zoneId]; ++j) {
            slots.push_back(zoneId);
        }
    }

    vector<Student> students;
    students.reserve(N);

    for (int i = 0; i < N; ++i) {
        int cntZones = lower_bound(sortedQ.begin(), sortedQ.end(), P[i]) - sortedQ.begin();
        int deadline = pref[cntZones];
        students.push_back({i, P[i], W[i], deadline});
    }

    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        if (a.deadline != b.deadline) return a.deadline < b.deadline;
        return a.id < b.id;
    });

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;

    for (const Student& s : students) {
        heap.push({s.w, s.id});
        if ((int)heap.size() > s.deadline) {
            heap.pop();
        }
    }

    vector<char> selected(N, 0);
    while (!heap.empty()) {
        selected[heap.top().second] = 1;
        heap.pop();
    }

    vector<Student> chosen;
    chosen.reserve(N);

    for (const Student& s : students) {
        if (selected[s.id]) {
            chosen.push_back(s);
        }
    }

    vector<int> ans(N, -1);
    int ptr = 0;

    for (const Student& s : chosen) {
        ans[s.id] = slots[ptr++] + 1;
    }

    for (int i = 0; i < N; ++i) {
        if (ans[i] == -1) {
            ans[i] = slots[ptr++] + 1;
        }
    }

    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';

    return 0;
}
