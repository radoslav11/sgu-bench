#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> grid(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> grid[i][j];
        }
    }

    // Precompute prefix sums for quick area checks
    vector<vector<int>> prefix(N + 1, vector<int>(M + 1, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            prefix[i + 1][j + 1] = grid[i][j] + prefix[i][j + 1] + prefix[i + 1][j] - prefix[i][j];
        }
    }

    auto getSum = [&](int r1, int c1, int r2, int c2) {
        if (r1 > r2 || c1 > c2) return 0;
        return prefix[r2 + 1][c2 + 1] - prefix[r1][c2 + 1] - prefix[r2 + 1][c1] + prefix[r1][c1];
    };

    // Check if a rectangle [r1..r2] x [c1..c2] is free of trees (all zeros in grid)
    auto isRectFree = [&](int r1, int c1, int r2, int c2) {
        return getSum(r1, c1, r2, c2) == 0;
    };

    int bestArea = -1;
    vector<pair<int, int>> bestPoints; // Will store coordinates of occupied cells

    // The house shape: three rectangles stacked vertically
    // 1. Top block: rows [r1, r2], columns [c1, c2]
    // 2. Middle block: rows [r2+1, r3], columns [c1', c2'] where width(c1',c2') < min(width of top, bottom)
    // 3. Bottom block: rows [r3+1, r4], columns [c3, c4]
    // All three share the same left column coordinate (left-aligned vertically), per condition 4: "Left-top cells of all blocks must be on one vertical"
    // So: c1 == c1' == c3 = L (common left column)
    // Also: top and bottom widths >= middle width (strictly greater for both, per condition 5)

    // So let's fix:
    // - common left column L (0 <= L < M)
    // - top block: rows [r1, r2], width w1 = R1 - L + 1
    // - middle block: rows [r2+1, r3], width w2 = R2 - L + 1, with w2 < w1 and w2 < w3
    // - bottom block: rows [r3+1, r4], width w3 = R3 - L + 1

    for (int L = 0; L < M; ++L) {
        // For fixed left column L, try all possible top, middle, bottom block ranges

        // Precompute for each row range starting at L: which rightmost columns are free
        // Instead, let's try all possible top, middle, bottom row ranges
        for (int r1 = 0; r1 < N; ++r1) {
            // Top block must start at row r1
            // Try all possible r2 >= r1 such that top block [r1..r2] x [L..R1] is free
            for (int r2 = r1; r2 < N; ++r2) {
                // Determine max possible width for top block
                int maxTopWidth = 0;
                for (int R1 = L; R1 < M; ++R1) {
                    if (isRectFree(r1, L, r2, R1)) {
                        maxTopWidth = R1 - L + 1;
                    } else {
                        break;
                    }
                }
                if (maxTopWidth == 0) break;

                // Now middle block starts at r2+1
                for (int r3 = r2 + 1; r3 < N; ++r3) {
                    // Middle block: [r2+1..r3] x [L..R2]
                    int maxMidWidth = 0;
                    for (int R2 = L; R2 < M; ++R2) {
                        if (isRectFree(r2 + 1, L, r3, R2)) {
                            maxMidWidth = R2 - L + 1;
                        } else {
                            break;
                        }
                    }
                    if (maxMidWidth == 0) break;

                    // Bottom block starts at r3+1
                    for (int r4 = r3 + 1; r4 < N; ++r4) {
                        // Bottom block: [r3+1..r4] x [L..R3]
                        int maxBotWidth = 0;
                        for (int R3 = L; R3 < M; ++R3) {
                            if (isRectFree(r3 + 1, L, r4, R3)) {
                                maxBotWidth = R3 - L + 1;
                            } else {
                                break;
                            }
                        }
                        if (maxBotWidth == 0) break;

                        // Now iterate over possible widths
                        for (int w1 = 1; w1 <= maxTopWidth; ++w1) {
                            if (!isRectFree(r1, L, r2, L + w1 - 1)) continue;

                            for (int w3 = 1; w3 <= maxBotWidth; ++w3) {
                                if (!isRectFree(r3 + 1, L, r4, L + w3 - 1)) continue;

                                // Middle width must be < min(w1, w3)
                                int maxW2 = min(w1, w3) - 1;
                                if (maxW2 <= 0) continue;

                                for (int w2 = 1; w2 <= maxMidWidth && w2 <= maxW2; ++w2) {
                                    if (!isRectFree(r2 + 1, L, r3, L + w2 - 1)) continue;

                                    int area = (r2 - r1 + 1) * w1 + (r3 - (r2 + 1) + 1) * w2 + (r4 - (r3 + 1) + 1) * w3;
                                    if (area > bestArea) {
                                        bestArea = area;
                                        bestPoints.clear();

                                        // Add top block
                                        for (int i = r1; i <= r2; ++i) {
                                            for (int j = L; j < L + w1; ++j) {
                                                bestPoints.push_back({i, j});
                                            }
                                        }

                                        // Add middle block
                                        for (int i = r2 + 1; i <= r3; ++i) {
                                            for (int j = L; j < L + w2; ++j) {
                                                bestPoints.push_back({i, j});
                                            }
                                        }

                                        // Add bottom block
                                        for (int i = r3 + 1; i <= r4; ++i) {
                                            for (int j = L; j < L + w3; ++j) {
                                                bestPoints.push_back({i, j});
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (bestArea <= 0) {
        cout << "-1" << endl;
    } else {
        cout << bestArea << endl;

        // Create output grid
        vector<vector<int>> outGrid = grid;
        for (auto [i, j] : bestPoints) {
            outGrid[i][j] = 8;
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                if (j > 0) cout << " ";
                cout << outGrid[i][j];
            }
            cout << "\n";
        }
    }

    return 0;
}
