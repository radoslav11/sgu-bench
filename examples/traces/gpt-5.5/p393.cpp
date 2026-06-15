#include <bits/stdc++.h>
using namespace std;

int N, M, K;
vector<pair<int,int>> words;
vector<int> orderLetters;
vector<int> colorOf;
vector<int> occ;
vector<int> answerColor;

bool canPlace(int v, int c, vector<int>& cells) {
    cells.clear();

    for (auto [a, b] : words) {
        int cell = -1;

        if (a == v && b == v) {
            cell = c * K + c;
        } else if (a == v && colorOf[b] != -1) {
            cell = c * K + colorOf[b];
        } else if (b == v && colorOf[a] != -1) {
            cell = colorOf[a] * K + c;
        }

        if (cell != -1) {
            if (occ[cell] > 0) return false;
            for (int x : cells) {
                if (x == cell) return false;
            }
            cells.push_back(cell);
        }
    }

    return true;
}

bool dfs(int pos, int usedColors) {
    if (pos == N) {
        answerColor = colorOf;
        return true;
    }

    int v = orderLetters[pos];

    for (int c = 0; c <= usedColors && c < K; c++) {
        vector<int> cells;
        if (!canPlace(v, c, cells)) continue;

        colorOf[v] = c;
        for (int cell : cells) occ[cell]++;

        if (dfs(pos + 1, max(usedColors, c + 1))) return true;

        for (int cell : cells) occ[cell]--;
        colorOf[v] = -1;
    }

    return false;
}

bool solveForK(int k) {
    K = k;
    colorOf.assign(N, -1);
    occ.assign(K * K, 0);
    answerColor.clear();
    return dfs(0, 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;

    words.resize(M);
    vector<int> deg(N, 0), self(N, 0);

    for (int i = 0; i < M; i++) {
        string s;
        cin >> s;
        int a = s[0] - 'a';
        int b = s[1] - 'a';
        words[i] = {a, b};
        deg[a]++;
        deg[b]++;
        if (a == b) self[a] = 1;
    }

    orderLetters.resize(N);
    iota(orderLetters.begin(), orderLetters.end(), 0);

    sort(orderLetters.begin(), orderLetters.end(), [&](int x, int y) {
        if (self[x] != self[y]) return self[x] > self[y];
        if (deg[x] != deg[y]) return deg[x] > deg[y];
        return x < y;
    });

    int bestK = 1;
    for (; bestK <= N; bestK++) {
        if (solveForK(bestK)) break;
    }

    int realK = 0;
    for (int c : answerColor) realK = max(realK, c + 1);

    vector<string> buttons(realK);
    for (int i = 0; i < N; i++) {
        buttons[answerColor[i]].push_back(char('a' + i));
    }

    cout << realK << '\n';
    for (string &s : buttons) {
        cout << s << '\n';
    }

    return 0;
}
