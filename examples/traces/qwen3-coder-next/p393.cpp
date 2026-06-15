#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
using namespace std;

bool dfs(int idx, int K, const vector<int>& order, 
         const vector<pair<int,int>>& words, 
         vector<int>& group, 
         set<pair<int,int>>& used_pairs, 
         const vector<vector<int>>& words_at_step) {

    if (idx == (int)order.size()) {
        return true;
    }

    int letter = order[idx];
    for (int b = 0; b < K; b++) {
        group[letter] = b;

        set<pair<int,int>> new_pairs;
        bool conflict = false;
        for (int j : words_at_step[idx]) {
            int u = words[j].first;
            int v = words[j].second;
            int p1 = group[u];
            int p2 = group[v];
            pair<int,int> p = make_pair(p1, p2);
            if (used_pairs.find(p) != used_pairs.end() || new_pairs.find(p) != new_pairs.end()) {
                conflict = true;
                break;
            }
            new_pairs.insert(p);
        }

        if (conflict) {
            continue;
        }

        for (auto p : new_pairs) {
            used_pairs.insert(p);
        }

        if (dfs(idx+1, K, order, words, group, used_pairs, words_at_step)) {
            return true;
        }

        for (auto p : new_pairs) {
            used_pairs.erase(p);
        }
    }
    return false;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<pair<int, int>> words;
    vector<int> deg(N, 0);

    for (int i = 0; i < M; i++) {
        string s;
        cin >> s;
        int u = s[0] - 'a';
        int v = s[1] - 'a';
        words.push_back(make_pair(u, v));
        deg[u]++;
        deg[v]++;
    }

    // Create order: sort by degree descending, then by index ascending
    vector<int> order(N);
    for (int i = 0; i < N; i++) {
        order[i] = i;
    }
    sort(order.begin(), order.end(), [&](int i, int j) {
        if (deg[i] != deg[j]) {
            return deg[i] > deg[j];
        }
        return i < j;
    });

    // Precompute pos: position of each letter in the order
    vector<int> pos(N);
    for (int i = 0; i < N; i++) {
        pos[order[i]] = i;
    }

    // Precompute words_at_step: for each step, list of word indices that complete at that step
    vector<vector<int>> words_at_step(N);
    for (int j = 0; j < M; j++) {
        int u = words[j].first;
        int v = words[j].second;
        int step = max(pos[u], pos[v]);
        words_at_step[step].push_back(j);
    }

    // Determine minimal K: K must satisfy K*K >= M
    int minK = 1;
    while (minK * minK < M) {
        minK++;
    }

    for (int K = minK; K <= N; K++) {
        vector<int> group(N);
        set<pair<int,int>> used_pairs;
        if (dfs(0, K, order, words, group, used_pairs, words_at_step)) {
            cout << K << endl;
            vector<vector<char>> buttons(K);
            for (int i = 0; i < N; i++) {
                buttons[group[i]].push_back(char('a' + i));
            }
            for (int i = 0; i < K; i++) {
                for (char c : buttons[i]) {
                    cout << c;
                }
                cout << '\n';
            }
            return 0;
        }
    }

    // Should not reach here, but fallback
    cout << N << endl;
    for (int i = 0; i < N; i++) {
        cout << char('a' + i) << '\n';
    }
    return 0;
}
