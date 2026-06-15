#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct State {
    string lastN;
    bool seenS2;

    bool operator==(const State& other) const {
        return seenS2 == other.seenS2 && lastN == other.lastN;
    }
};

struct StateHash {
    size_t operator()(const State& s) const {
        size_t h = hash<string>{}(s.lastN);
        h ^= hash<bool>{}(s.seenS2) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    string S1, S2;
    cin >> S1 >> S2;

    if (S1 == S2) {
        cout << S1 << endl;
        return 0;
    }

    queue<pair<State, string>> q;
    unordered_map<State, int, StateHash> visited;

    bool s1_contains_s2 = (S1.find(S2) != string::npos);
    State startState = {S1, s1_contains_s2};
    q.push({startState, S1});
    visited[startState] = N;

    while (!q.empty()) {
        State currState = q.front().first;
        string currStr = q.front().second;
        q.pop();

        if (currState.seenS2) {
            cout << currStr << endl;
            return 0;
        }

        for (int i = 0; i < N; ++i) {
            char c = 'A' + i;
            // New window of length N+1: last N characters + new character c
            // Check if this window contains all characters from 'A' to 'A'+N-1
            vector<bool> present(N, false);
            int distinct_count = 0;
            for (char ch : currState.lastN) {
                if (ch - 'A' < N && !present[ch - 'A']) {
                    present[ch - 'A'] = true;
                    distinct_count++;
                }
            }
            if (!present[c - 'A']) {
                distinct_count++;
            }

            if (distinct_count == N) {
                string nextStr = currStr + c;
                string nextLastN = nextStr.substr(nextStr.length() - N);
                bool newlySeenS2 = (nextStr.length() >= (size_t)N && nextStr.substr(nextStr.length() - N) == S2);
                bool totalSeenS2 = currState.seenS2 || newlySeenS2;

                State nextState = {nextLastN, totalSeenS2};
                if (visited.find(nextState) == visited.end()) {
                    visited[nextState] = nextStr.length();
                    q.push({nextState, nextStr});
                }
            }
        }
    }

    return 0;
}

