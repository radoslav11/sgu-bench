#include <bits/stdc++.h>
using namespace std;

struct Cand {
    int coin;
    int sign; // +1 = heavy, -1 = light
};

long long pow3v[20], Ucap[20];

string joinCoins(const vector<int>& v) {
    if (v.empty()) return "";
    string s;
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i) s += "+";
        s += to_string(v[i]);
    }
    return s;
}

void printIndent(int d) {
    for (int i = 0; i < d; ++i) cout << "  ";
}

void solveU(const vector<int>& coins, int k, vector<int> genuine, int dep);
void solveS(const vector<Cand>& cand, int k, vector<int> genuine, int dep);

void printWeigh(const vector<int>& L, const vector<int>& R, int dep) {
    printIndent(dep);
    cout << "weigh " << joinCoins(L) << " vs " << joinCoins(R) << "\n";
}

void solveU(const vector<int>& coins, int k, vector<int> genuine, int dep) {
    int n = (int)coins.size();

    if (n == 1 || k == 0) {
        printIndent(dep);
        cout << "fake " << coins[0] << "\n";
        return;
    }

    long long prevU = Ucap[k - 1];
    long long prevS = pow3v[k - 1];

    int z = min<long long>(n, prevU);
    while (z >= 0 && ((n - z) & 1)) --z;
    while (z >= 0 && (n - z) / 2 * 2 > prevS) z -= 2;

    int x = (n - z) / 2;

    vector<int> L, R, O;
    for (int i = 0; i < x; ++i) L.push_back(coins[i]);
    for (int i = 0; i < x; ++i) R.push_back(coins[x + i]);
    for (int i = 2 * x; i < n; ++i) O.push_back(coins[i]);

    printWeigh(L, R, dep);

    if (!L.empty() || !R.empty()) {
        printIndent(dep);
        cout << "case <:\n";
        vector<Cand> nc;
        for (int c : L) nc.push_back({c, -1});
        for (int c : R) nc.push_back({c, +1});
        vector<int> ng = genuine;
        for (int c : O) ng.push_back(c);
        solveS(nc, k - 1, ng, dep + 1);
    }

    if (!O.empty()) {
        printIndent(dep);
        cout << "case =:\n";
        vector<int> ng = genuine;
        for (int c : L) ng.push_back(c);
        for (int c : R) ng.push_back(c);
        solveU(O, k - 1, ng, dep + 1);
    }

    if (!L.empty() || !R.empty()) {
        printIndent(dep);
        cout << "case >:\n";
        vector<Cand> nc;
        for (int c : L) nc.push_back({c, +1});
        for (int c : R) nc.push_back({c, -1});
        vector<int> ng = genuine;
        for (int c : O) ng.push_back(c);
        solveS(nc, k - 1, ng, dep + 1);
    }

    printIndent(dep);
    cout << "end\n";
}

void solveS(const vector<Cand>& cand, int k, vector<int> genuine, int dep) {
    int m = (int)cand.size();

    if (m == 1 || k == 0) {
        printIndent(dep);
        cout << "fake " << cand[0].coin << "\n";
        return;
    }

    int cap = pow3v[k - 1];
    int a = min(cap, m);
    int b = min(cap, m - a);
    int c = m - a - b;

    vector<Cand> A, B, C;
    for (int i = 0; i < a; ++i) A.push_back(cand[i]);
    for (int i = a; i < a + b; ++i) B.push_back(cand[i]);
    for (int i = a + b; i < m; ++i) C.push_back(cand[i]);

    vector<int> L, R;

    auto putForOutcome = [&](const Cand& cd, char out) {
        if (out == '=') return;
        bool wantLess = (out == '<');
        if (cd.sign == +1) {
            if (wantLess) R.push_back(cd.coin);
            else L.push_back(cd.coin);
        } else {
            if (wantLess) L.push_back(cd.coin);
            else R.push_back(cd.coin);
        }
    };

    for (auto &cd : A) putForOutcome(cd, '<');
    for (auto &cd : C) putForOutcome(cd, '>');

    int diff = (int)L.size() - (int)R.size();
    int gi = 0;
    while (diff > 0 && gi < (int)genuine.size()) {
        R.push_back(genuine[gi++]);
        --diff;
    }
    while (diff < 0 && gi < (int)genuine.size()) {
        L.push_back(genuine[gi++]);
        ++diff;
    }

    printWeigh(L, R, dep);

    auto makeGenuine = [&](const vector<Cand>& keep) {
        set<int> ks;
        for (auto &x : keep) ks.insert(x.coin);
        vector<int> ng = genuine;
        for (auto &x : cand) {
            if (!ks.count(x.coin)) ng.push_back(x.coin);
        }
        return ng;
    };

    if (!A.empty()) {
        printIndent(dep);
        cout << "case <:\n";
        solveS(A, k - 1, makeGenuine(A), dep + 1);
    }

    if (!B.empty()) {
        printIndent(dep);
        cout << "case =:\n";
        solveS(B, k - 1, makeGenuine(B), dep + 1);
    }

    if (!C.empty()) {
        printIndent(dep);
        cout << "case >:\n";
        solveS(C, k - 1, makeGenuine(C), dep + 1);
    }

    printIndent(dep);
    cout << "end\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    pow3v[0] = 1;
    for (int i = 1; i < 20; ++i) pow3v[i] = pow3v[i - 1] * 3;

    Ucap[0] = 1;
    for (int i = 1; i < 20; ++i) Ucap[i] = Ucap[i - 1] + pow3v[i - 1];

    int k = 0;
    while (Ucap[k] < n) ++k;

    cout << "need " << k << " weighings\n";

    vector<int> coins;
    for (int i = 1; i <= n; ++i) coins.push_back(i);

    solveU(coins, k, {}, 0);

    return 0;
}
