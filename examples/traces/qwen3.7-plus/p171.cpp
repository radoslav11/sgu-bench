#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Zone {
    int orig_id;
    int Q;
    int cap;
};

struct Student {
    int orig_id;
    int P;
    int w;
    int assign_to;
};

int parent_node[105];

int find_set(int v) {
    if (v == 0) return 0;
    if (v == parent_node[v]) return v;
    return parent_node[v] = find_set(parent_node[v]);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int K;
    if (!(cin >> K)) return 0;

    vector<int> N_arr(K);
    long long total_N = 0;
    for (int i = 0; i < K; ++i) {
        cin >> N_arr[i];
        total_N += N_arr[i];
    }

    int N = total_N;
    if (N == 0) return 0;

    vector<int> Q_arr(K);
    for (int i = 0; i < K; ++i) {
        cin >> Q_arr[i];
    }

    vector<int> P(N);
    for (int i = 0; i < N; ++i) {
        cin >> P[i];
    }

    vector<int> w(N);
    for (int i = 0; i < N; ++i) {
        cin >> w[i];
    }

    vector<Zone> zones(K);
    for (int i = 0; i < K; ++i) {
        zones[i] = {i + 1, Q_arr[i], N_arr[i]};
    }

    sort(zones.begin(), zones.end(), [](const Zone& a, const Zone& b) {
        return a.Q < b.Q;
    });

    vector<Student> students(N);
    for (int i = 0; i < N; ++i) {
        students[i] = {i + 1, P[i], w[i], -1};
    }

    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.w > b.w;
    });

    vector<int> Q_1based(K + 1);
    for (int i = 1; i <= K; ++i) {
        Q_1based[i] = zones[i - 1].Q;
    }

    vector<int> cap(K + 1);
    for (int i = 0; i <= K; ++i) {
        parent_node[i] = i;
    }
    for (int i = 1; i <= K; ++i) {
        cap[i] = zones[i - 1].cap;
    }

    for (int i = 0; i < N; ++i) {
        int c_j = lower_bound(Q_1based.begin() + 1, Q_1based.end(), students[i].P) - Q_1based.begin() - 1;
        if (c_j > 0) {
            int avail = find_set(c_j);
            if (avail > 0) {
                students[i].assign_to = zones[avail - 1].orig_id;
                cap[avail]--;
                if (cap[avail] == 0) {
                    parent_node[avail] = find_set(avail - 1);
                }
            }
        }
    }

    vector<int> unqualified_indices;
    for (int i = 0; i < N; ++i) {
        if (students[i].assign_to == -1) {
            unqualified_indices.push_back(i);
        }
    }

    int u_idx = 0;
    for (int i = 1; i <= K; ++i) {
        while (cap[i] > 0 && u_idx < (int)unqualified_indices.size()) {
            students[unqualified_indices[u_idx]].assign_to = zones[i - 1].orig_id;
            cap[i]--;
            u_idx++;
        }
    }

    vector<int> ans(N + 1);
    for (int i = 0; i < N; ++i) {
        ans[students[i].orig_id] = students[i].assign_to;
    }

    for (int i = 1; i <= N; ++i) {
        cout << ans[i] << (i == N ? "" : " ");
    }
    cout << "\n";

    return 0;
}
