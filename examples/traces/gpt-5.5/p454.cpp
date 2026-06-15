#include <bits/stdc++.h>
using namespace std;

struct Cell {
    int r, c;
    int h = -1, v = -1;
    int val = 0;
};

struct Run {
    int target = 0;
    vector<int> cells;
    int sum = 0;
    int mask = 0;
    int filled = 0;
};

int n, m;
vector<string> rawCells;
bool isWhite[6][6];
int cellId[6][6];
int vClue[6][6], hClue[6][6];

vector<Cell> cells;
vector<Run> runs;

bool possible[512][10][46];
const int ALL = (1 << 9) - 1;

bool checkState(int runId, int addDigit = 0) {
    Run &run = runs[runId];

    int newSum = run.sum;
    int newMask = run.mask;
    int newFilled = run.filled;

    if (addDigit) {
        int bit = 1 << (addDigit - 1);
        if (newMask & bit) return false;
        newSum += addDigit;
        newMask |= bit;
        newFilled++;
    }

    int remCells = (int)run.cells.size() - newFilled;
    int remSum = run.target - newSum;

    if (remSum < 0 || remSum > 45) return false;

    int availMask = ALL ^ newMask;
    return possible[availMask][remCells][remSum];
}

bool canPlace(int cid, int digit) {
    Cell &cell = cells[cid];
    return checkState(cell.h, digit) && checkState(cell.v, digit);
}

void placeDigit(int cid, int digit) {
    cells[cid].val = digit;
    int bit = 1 << (digit - 1);

    for (int runId : {cells[cid].h, cells[cid].v}) {
        runs[runId].sum += digit;
        runs[runId].mask |= bit;
        runs[runId].filled++;
    }
}

void removeDigit(int cid, int digit) {
    cells[cid].val = 0;
    int bit = 1 << (digit - 1);

    for (int runId : {cells[cid].h, cells[cid].v}) {
        runs[runId].sum -= digit;
        runs[runId].mask ^= bit;
        runs[runId].filled--;
    }
}

bool solve() {
    int bestCell = -1;
    vector<int> bestCandidates;

    for (int i = 0; i < (int)cells.size(); i++) {
        if (cells[i].val != 0) continue;

        vector<int> candidates;
        for (int d = 1; d <= 9; d++) {
            if (canPlace(i, d)) {
                candidates.push_back(d);
            }
        }

        if (candidates.empty()) return false;

        if (bestCell == -1 || candidates.size() < bestCandidates.size()) {
            bestCell = i;
            bestCandidates = candidates;
        }
    }

    if (bestCell == -1) return true;

    for (int d : bestCandidates) {
        placeDigit(bestCell, d);
        if (solve()) return true;
        removeDigit(bestCell, d);
    }

    return false;
}

int parseTwoDigits(const string &s, int pos) {
    if (s[pos] == 'X') return -1;
    return (s[pos] - '0') * 10 + (s[pos + 1] - '0');
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    memset(possible, false, sizeof(possible));

    for (int mask = 0; mask < 512; mask++) {
        possible[mask][0][0] = true;

        for (int d = 1; d <= 9; d++) {
            int bit = 1 << (d - 1);
            if (!(mask & bit)) continue;

            for (int k = 8; k >= 0; k--) {
                for (int sum = 45 - d; sum >= 0; sum--) {
                    if (possible[mask ^ bit][k][sum]) {
                        possible[mask][k + 1][sum + d] = true;
                    }
                }
            }
        }
    }

    cin >> n >> m;

    memset(isWhite, false, sizeof(isWhite));
    memset(cellId, -1, sizeof(cellId));
    memset(vClue, -1, sizeof(vClue));
    memset(hClue, -1, sizeof(hClue));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            string s;
            cin >> s;

            if (s == ".....") {
                isWhite[i][j] = true;
                cellId[i][j] = (int)cells.size();
                cells.push_back({i, j});
            } else {
                vClue[i][j] = parseTwoDigits(s, 0);
                hClue[i][j] = parseTwoDigits(s, 3);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (isWhite[i][j]) continue;

            if (hClue[i][j] != -1) {
                Run run;
                run.target = hClue[i][j];

                int id = (int)runs.size();

                for (int col = j + 1; col < m && isWhite[i][col]; col++) {
                    int cid = cellId[i][col];
                    cells[cid].h = id;
                    run.cells.push_back(cid);
                }

                runs.push_back(run);
            }

            if (vClue[i][j] != -1) {
                Run run;
                run.target = vClue[i][j];

                int id = (int)runs.size();

                for (int row = i + 1; row < n && isWhite[row][j]; row++) {
                    int cid = cellId[row][j];
                    cells[cid].v = id;
                    run.cells.push_back(cid);
                }

                runs.push_back(run);
            }
        }
    }

    solve();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (j) cout << ' ';

            if (!isWhite[i][j]) {
                cout << '_';
            } else {
                cout << cells[cellId[i][j]].val;
            }
        }
        cout << '\n';
    }

    return 0;
}
