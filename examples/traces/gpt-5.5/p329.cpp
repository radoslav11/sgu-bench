#include <bits/stdc++.h>
using namespace std;

struct Key {
    int idx;
    unsigned char c0, c1, c2;
    unsigned long long mask;

    bool operator==(const Key& other) const {
        return idx == other.idx && c0 == other.c0 && c1 == other.c1 &&
               c2 == other.c2 && mask == other.mask;
    }
};

struct KeyHash {
    size_t operator()(const Key& k) const {
        unsigned long long x = k.mask;
        x ^= (unsigned long long)k.idx * 1000003ULL;
        x ^= (unsigned long long)k.c0 << 48;
        x ^= (unsigned long long)k.c1 << 40;
        x ^= (unsigned long long)k.c2 << 32;
        x ^= x >> 23;
        x *= 0x2127599bf4325c37ULL;
        x ^= x >> 47;
        return (size_t)x;
    }
};

int N;
int need[4];
vector<array<int, 3>> faces;
int F, E;
vector<int> firstFace, lastFace;
vector<unsigned long long> activeAfter;
unordered_map<Key, unsigned long long, KeyHash> memo;

int vid[6][6];

int getEdge(map<pair<int,int>, int>& mp, int a, int b) {
    if (a > b) swap(a, b);
    pair<int,int> p = {a, b};
    auto it = mp.find(p);
    if (it != mp.end()) return it->second;
    int id = (int)mp.size();
    mp[p] = id;
    return id;
}

unsigned long long dfs(int idx, int c0, int c1, int c2, int c3, unsigned long long mask) {
    if (c0 < 0 || c1 < 0 || c2 < 0 || c3 < 0) return 0;

    if (idx == F) {
        return (c0 == 0 && c1 == 0 && c2 == 0 && c3 == 0) ? 1ULL : 0ULL;
    }

    Key key{idx, (unsigned char)c0, (unsigned char)c1, (unsigned char)c2, mask};
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    array<int, 3> e = faces[idx];

    int fixed[3];
    vector<int> unknown;

    for (int i = 0; i < 3; ++i) {
        if (firstFace[e[i]] < idx) {
            fixed[i] = (mask >> e[i]) & 1ULL;
        } else {
            fixed[i] = -1;
            unknown.push_back(i);
        }
    }

    unsigned long long ans = 0;
    int variants = 1 << (int)unknown.size();

    for (int m = 0; m < variants; ++m) {
        int col[3];
        for (int i = 0; i < 3; ++i) col[i] = fixed[i];

        unsigned long long newMask = mask;

        for (int b = 0; b < (int)unknown.size(); ++b) {
            int pos = unknown[b];
            int color = (m >> b) & 1;
            col[pos] = color;
            if (color) newMask |= (1ULL << e[pos]);
            else newMask &= ~(1ULL << e[pos]);
        }

        int black = col[0] + col[1] + col[2];

        newMask &= activeAfter[idx + 1];

        if (black == 0)
            ans += dfs(idx + 1, c0 - 1, c1, c2, c3, newMask);
        else if (black == 1)
            ans += dfs(idx + 1, c0, c1 - 1, c2, c3, newMask);
        else if (black == 2)
            ans += dfs(idx + 1, c0, c1, c2 - 1, c3, newMask);
        else
            ans += dfs(idx + 1, c0, c1, c2, c3 - 1, newMask);
    }

    memo[key] = ans;
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    for (int i = 0; i < 4; ++i) cin >> need[i];

    int id = 0;
    for (int i = 0; i <= N; ++i) {
        for (int j = 0; j <= N - i; ++j) {
            vid[i][j] = id++;
        }
    }

    map<pair<int,int>, int> edgeMap;

    struct FaceTmp {
        int layer;
        int type;
        array<int, 3> edges;
    };

    vector<FaceTmp> tmp;

    for (int s = 0; s <= N - 1; ++s) {
        for (int i = 0; i <= s; ++i) {
            int j = s - i;
            if (i + j >= N) continue;

            int a = vid[i][j];
            int b = vid[i + 1][j];
            int c = vid[i][j + 1];

            array<int, 3> ed = {
                getEdge(edgeMap, a, b),
                getEdge(edgeMap, a, c),
                getEdge(edgeMap, b, c)
            };
            tmp.push_back({s, 0, ed});
        }

        for (int i = 0; i <= s; ++i) {
            int j = s - i;
            if (i + j >= N - 1) continue;

            int a = vid[i + 1][j];
            int b = vid[i][j + 1];
            int c = vid[i + 1][j + 1];

            array<int, 3> ed = {
                getEdge(edgeMap, a, b),
                getEdge(edgeMap, a, c),
                getEdge(edgeMap, b, c)
            };
            tmp.push_back({s, 1, ed});
        }
    }

    E = (int)edgeMap.size();

    for (auto &f : tmp) faces.push_back(f.edges);

    F = (int)faces.size();

    firstFace.assign(E, F);
    lastFace.assign(E, -1);

    for (int i = 0; i < F; ++i) {
        for (int e : faces[i]) {
            firstFace[e] = min(firstFace[e], i);
            lastFace[e] = max(lastFace[e], i);
        }
    }

    activeAfter.assign(F + 1, 0ULL);
    for (int idx = 0; idx <= F; ++idx) {
        unsigned long long m = 0;
        for (int e = 0; e < E; ++e) {
            if (firstFace[e] < idx && idx <= lastFace[e]) {
                m |= (1ULL << e);
            }
        }
        activeAfter[idx] = m;
    }

    cout << dfs(0, need[0], need[1], need[2], need[3], 0ULL) << '\n';

    return 0;
}
