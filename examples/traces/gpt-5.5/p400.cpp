#include <bits/stdc++.h>
using namespace std;

struct Team {
    string name;
    int solved = 0;
    long long penalty = 0;
    vector<long long> add; // minimal penalty additions for currently unsolved problems
};

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static vector<string> split(const string& s) {
    vector<string> v;
    stringstream ss(s);
    string x;
    while (ss >> x) v.push_back(x);
    return v;
}

static bool isNonNegativeIntegerLine(const string& s) {
    string t = trim(s);
    if (t.empty()) return false;
    for (char c : t) if (!isdigit((unsigned char)c)) return false;
    return true;
}

static int prevRejectsOfUnsolved(const string& r) {
    if (r == "." || r == "..") return 0;
    if (!r.empty() && r[0] == '-') {
        int x = 0;
        for (int i = 1; i < (int)r.size(); ++i)
            if (isdigit((unsigned char)r[i])) x = x * 10 + (r[i] - '0');
        return x;
    }
    return -1; // already solved
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (true) {
        string header;
        while (getline(cin, header)) {
            header = trim(header);
            if (!header.empty()) break;
        }
        if (header.empty()) break;

        vector<string> ht = split(header);
        if ((int)ht.size() < 5) break;
        int Pn = (int)ht.size() - 4;

        vector<string> teamLines;
        long long balloons = 0;

        while (getline(cin, line)) {
            if (isNonNegativeIntegerLine(line)) {
                balloons = stoll(trim(line));
                break;
            }
            if (!trim(line).empty()) teamLines.push_back(line);
        }

        string myName;
        if (!getline(cin, myName)) break;
        myName = trim(myName);

        vector<Team> teams;
        int my = -1;

        for (string tl : teamLines) {
            vector<string> tok = split(tl);
            int n = tok.size();

            int resStart = n - Pn;
            int solvedIdx = resStart - 2;
            int penaltyIdx = resStart - 1;

            Team t;
            t.solved = stoi(tok[solvedIdx]);
            t.penalty = stoll(tok[penaltyIdx]);

            t.name = tok[1];
            for (int i = 2; i < solvedIdx; ++i) {
                t.name += " ";
                t.name += tok[i];
            }

            for (int i = resStart; i < n; ++i) {
                int rej = prevRejectsOfUnsolved(tok[i]);
                if (rej >= 0) {
                    t.add.push_back(241LL + 20LL * rej);
                }
            }
            sort(t.add.begin(), t.add.end());

            if (t.name == myName) my = (int)teams.size();
            teams.push_back(t);
        }

        int N = teams.size();
        long long otherCap = 0;
        for (int i = 0; i < N; ++i)
            if (i != my) otherCap += teams[i].add.size();

        Team &me = teams[my];
        int myRem = me.add.size();

        auto prefixAdd = [&](const Team& t, int k) {
            long long s = 0;
            for (int i = 0; i < k; ++i) s += t.add[i];
            return s;
        };

        int bestPlace = N + 1;

        int kLow = max<long long>(0, balloons - otherCap);
        int kHigh = min<long long>(myRem, balloons);

        for (int k = kLow; k <= kHigh; ++k) {
            int S = me.solved + k;
            long long Pen = me.penalty + prefixAdd(me, k);
            long long left = balloons - k;

            int forced = 0;
            long long safeTotal = 0;
            vector<long long> unsafeCaps;

            for (int i = 0; i < N; ++i) if (i != my) {
                Team &t = teams[i];
                int rem = t.add.size();

                bool alreadyBetter = false;
                if (t.solved > S) alreadyBetter = true;
                if (t.solved == S && t.penalty < Pen) alreadyBetter = true;

                if (alreadyBetter) {
                    forced++;
                    unsafeCaps.push_back(rem);
                    continue;
                }

                int safe = 0;
                if (t.solved < S) {
                    safe = min(rem, S - t.solved);
                } else {
                    safe = 0;
                }

                safeTotal += safe;
                if (rem > safe) unsafeCaps.push_back(rem - safe);
            }

            long long need = max(0LL, left - safeTotal);
            sort(unsafeCaps.rbegin(), unsafeCaps.rend());

            int extraBetter = 0;
            for (long long c : unsafeCaps) {
                if (need <= 0) break;
                need -= c;
                extraBetter++;
            }

            bestPlace = min(bestPlace, forced + extraBetter + 1);
        }

        int myForced = max<long long>(0, balloons - otherCap);
        int worstS = me.solved + myForced;
        bool myPenaltyInfinite = (myForced > 0);
        long long worstPen = me.penalty;
        if (!myPenaltyInfinite) {
            worstPen = me.penalty;
        }

        long long budgetOthers = balloons - myForced;

        vector<int> costs;
        for (int i = 0; i < N; ++i) if (i != my) {
            Team &t = teams[i];
            int rem = t.add.size();

            int cost = INT_MAX;

            if (t.solved > worstS) {
                cost = 0;
            } else if (t.solved == worstS) {
                if (myPenaltyInfinite || t.penalty < worstPen) cost = 0;
                else if (rem >= 1) cost = 1; // solving one more makes solved count higher
            } else {
                int needEqual = worstS - t.solved;
                int needMore = worstS + 1 - t.solved;

                if (myPenaltyInfinite) {
                    if (needEqual <= rem) cost = min(cost, needEqual);
                } else {
                    if (needEqual <= rem) {
                        long long p = t.penalty + prefixAdd(t, needEqual);
                        if (p < worstPen) cost = min(cost, needEqual);
                    }
                }

                if (needMore <= rem) cost = min(cost, needMore);
            }

            if (cost != INT_MAX) costs.push_back(cost);
        }

        sort(costs.begin(), costs.end());

        int better = 0;
        long long used = 0;
        for (int c : costs) {
            if (used + c <= budgetOthers) {
                used += c;
                better++;
            }
        }

        int worstPlace = better + 1;

        cout << bestPlace << ' ' << worstPlace << "\n";
    }

    return 0;
}
