#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdint>

using namespace std;

struct Placement {
    uint32_t mask;
};

struct Piece {
    char ch;
    vector<Placement> placements;
};

int N;
vector<Piece> pieces;
bool usedPiece[26];
char ans[5][5];
uint32_t fullMask;

vector<pair<int,int>> normalize(vector<pair<int,int>> v) {
    int minr = v[0].first, minc = v[0].second;
    for (auto &p : v) {
        if (p.first < minr) minr = p.first;
        if (p.second < minc) minc = p.second;
    }
    for (auto &p : v) {
        p.first -= minr;
        p.second -= minc;
    }
    sort(v.begin(), v.end());
    return v;
}

void generatePlacements(Piece &piece, const vector<pair<int,int>> &cells) {
    vector<uint32_t> seenMasks;

    for (int rot = 0; rot < 4; rot++) {
        vector<pair<int,int>> shape;

        for (auto &p : cells) {
            int r = p.first;
            int c = p.second;

            if (rot == 0) shape.push_back({ r, c });
            if (rot == 1) shape.push_back({ c, -r });
            if (rot == 2) shape.push_back({ -r, -c });
            if (rot == 3) shape.push_back({ -c, r });
        }

        shape = normalize(shape);

        int maxr = 0, maxc = 0;
        for (auto &p : shape) {
            if (p.first > maxr) maxr = p.first;
            if (p.second > maxc) maxc = p.second;
        }

        for (int sr = 0; sr + maxr < N; sr++) {
            for (int sc = 0; sc + maxc < N; sc++) {
                uint32_t mask = 0;

                for (auto &p : shape) {
                    int nr = sr + p.first;
                    int nc = sc + p.second;
                    mask |= 1u << (nr * N + nc);
                }

                bool exists = false;
                for (uint32_t m : seenMasks) {
                    if (m == mask) {
                        exists = true;
                        break;
                    }
                }

                if (!exists) {
                    seenMasks.push_back(mask);
                    piece.placements.push_back({mask});
                }
            }
        }
    }
}

bool dfs(uint32_t occupied) {
    if (occupied == fullMask) return true;

    int bestCell = -1;
    vector<pair<int,int>> bestCandidates;

    for (int cell = 0; cell < N * N; cell++) {
        if (occupied & (1u << cell)) continue;

        vector<pair<int,int>> candidates;

        for (int i = 0; i < (int)pieces.size(); i++) {
            if (usedPiece[i]) continue;

            for (int j = 0; j < (int)pieces[i].placements.size(); j++) {
                uint32_t mask = pieces[i].placements[j].mask;

                if ((mask & (1u << cell)) && (mask & occupied) == 0) {
                    candidates.push_back({i, j});
                }
            }
        }

        if (bestCell == -1 || candidates.size() < bestCandidates.size()) {
            bestCell = cell;
            bestCandidates = candidates;
            if (bestCandidates.empty()) return false;
        }
    }

    for (auto &cand : bestCandidates) {
        int pi = cand.first;
        int pj = cand.second;

        uint32_t mask = pieces[pi].placements[pj].mask;

        usedPiece[pi] = true;

        for (int cell = 0; cell < N * N; cell++) {
            if (mask & (1u << cell)) {
                ans[cell / N][cell % N] = pieces[pi].ch;
            }
        }

        if (dfs(occupied | mask)) return true;

        for (int cell = 0; cell < N * N; cell++) {
            if (mask & (1u << cell)) {
                ans[cell / N][cell % N] = '.';
            }
        }

        usedPiece[pi] = false;
    }

    return false;
}

int main() {
    scanf("%d", &N);

    vector<pair<int,int>> cells[26];

    char s[25];
    for (int r = 0; r < 20; r++) {
        scanf("%s", s);
        for (int c = 0; c < 20; c++) {
            if (s[c] >= 'A' && s[c] <= 'Z') {
                cells[s[c] - 'A'].push_back({r, c});
            }
        }
    }

    for (int i = 0; i < 26; i++) {
        if (!cells[i].empty()) {
            Piece p;
            p.ch = char('A' + i);
            generatePlacements(p, cells[i]);
            pieces.push_back(p);
        }
    }

    for (int i = 0; i < 26; i++) usedPiece[i] = false;

    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++) {
            ans[r][c] = '.';
        }
    }

    fullMask = (1u << (N * N)) - 1u;

    dfs(0);

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            putchar(ans[r][c]);
        }
        putchar('\n');
    }

    return 0;
}
