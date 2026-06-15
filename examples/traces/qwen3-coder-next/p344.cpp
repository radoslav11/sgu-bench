#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<char>> grid(N, vector<char>(M));
    vector<vector<int>> dist(N, vector<int>(M, -1)); // distance from nearest weed square
    queue<pair<int, int>> q;

    // Read grid and initialize queue with all weed squares
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 'X') {
                dist[i][j] = 0;
                q.push({i, j});
            }
        }
    }

    // Directions: up, down, left, right
    vector<int> dx = {0, 0, -1, 1};
    vector<int> dy = {-1, 1, 0, 0};

    // Multi-source BFS: expand from all weed squares simultaneously
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (nx >= 0 && nx < N && ny >= 0 && ny < M) {
                // If this square is empty and hasn't been visited yet
                if (dist[nx][ny] == -1) {
                    dist[nx][ny] = dist[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }

    // Count how many squares are occupied by weed
    // A square is occupied if it was originally weed (dist[i][j] == 0)
    // or if it became weed during expansion
    // According to the problem, expansion happens when a square has at least 2 adjacent weed squares
    // But the BFS approach above simulates "one step per day" expansion, which is different.

    // Let me reconsider: the problem says "if some square is free from the weed but at least two of its adjacent squares are occupied by the weed, that square will be also occupied by the weed soon"
    // This is NOT the typical cellular automaton where one cell spreads to neighbors.
    // It's about squares becoming weed if they have 2+ adjacent weed squares.

    // However, note the examples:
    // Example 1:
    // X..
    // .X.
    // .X.
    // Initially weed at (0,0), (1,1), (2,1)
    // Check (0,1): adjacent to (0,0) and (1,1) -> 2 weed neighbors -> becomes weed
    // Check (1,0): adjacent to (0,0) and (1,1) -> 2 weed neighbors -> becomes weed
    // Check (1,2): adjacent to (0,2) [empty], (1,1), (2,2) [empty] -> only 1 weed neighbor
    // Check (2,0): adjacent to (1,0) [now weed?], (2,1) -> but we need to consider simultaneous update
    // Actually, the process is iterative until no more changes.

    // However, the problem says: "Andrew is pretty sure that during last years weed occupied every square possible"
    // So we need to simulate until no more changes.

    // But note: the expansion condition requires 2 adjacent weed squares. This is not a typical BFS spread.

    // Alternate approach:
    // In the final state, a square is weed if:
    // 1. It was originally weed, OR
    // 2. It has at least 2 adjacent squares that are ultimately weed.

    // This is a fixed point problem. We can simulate until stable, but worst-case might be O(N*M*min(N,M)) which might be too slow for 1000x1000.

    // Insight: In the final configuration, the weed forms a "closure" under the rule that a cell becomes weed if it has ≥2 weed neighbors.
    // There's a known result: the final weed will be all cells in the "convex hull" of the original weed in terms of 4-connectivity? Not exactly.

    // Actually, let's think differently. Consider that if there's a rectangle containing all original weed, then all cells inside the rectangle will eventually become weed.
    // Why? Because for any interior cell, it will eventually have 2 adjacent weed cells as the weed spreads from the boundaries inward.

    // But the second example:
    // X..X
    // .X..
    // .X..
    // Original weed: (0,0), (0,3), (1,1), (2,1)
    // The output is 12, meaning almost the entire 3x4 grid (12 squares) is weed.

    // Let's simulate manually for example 2:
    // Initial:
    // X..X
    // .X..
    // .X..
    // 
    // Step 1: Check each empty cell
    // (0,1): neighbors (0,0)=X, (0,2)=., (1,1)=X -> 2 X's -> becomes X
    // (0,2): neighbors (0,1)=., (0,3)=X, (1,2)=. -> only 1 X -> stays .
    // (1,0): neighbors (0,0)=X, (1,1)=X, (2,0)=. -> 2 X's -> becomes X
    // (1,2): neighbors (0,2)=., (1,1)=X, (1,3)=., (2,2)=X -> 2 X's -> becomes X
    // (1,3): neighbors (0,3)=X, (1,2)=., (2,3)=. -> only 1 X -> stays .
    // (2,0): neighbors (1,0)=., (2,1)=X -> only 1 X -> stays .
    // (2,2): neighbors (1,2)=., (2,1)=X, (2,3)=. -> only 1 X -> stays .
    // (2,3): neighbors (1,3)=., (2,2)=. -> 0 X's -> stays .
    // After step 1:
    // XX.X
    // XX.X
    // .X..
    //
    // Step 2:
    // (0,2): neighbors (0,1)=X, (0,3)=X, (1,2)=X -> 3 X's -> becomes X
    // (1,3): neighbors (0,3)=X, (1,2)=X, (2,3)=. -> 2 X's -> becomes X
    // (2,0): neighbors (1,0)=X, (2,1)=X -> 2 X's -> becomes X
    // (2,2): neighbors (1,2)=X, (2,1)=X, (2,3)=. -> 2 X's -> becomes X
    // (2,3): neighbors (1,3)=., (2,2)=. -> still 0 X's (since (1,3) just became X, but step 2 changes are simultaneous) -> actually after step 2, (2,3) gets 2 neighbors? (2,2) and (1,3) both become X in step 2, but we check based on previous state.
    // Let's redo step 2 based on state after step 1:
    // (0,2): neighbors (0,1)=X, (0,3)=X, (1,2)=X -> 3 -> becomes X
    // (1,3): neighbors (0,3)=X, (1,2)=X, (2,3)=. -> 2 -> becomes X
    // (2,0): neighbors (1,0)=X, (2,1)=X -> 2 -> becomes X
    // (2,2): neighbors (1,2)=X, (2,1)=X, (2,3)=. -> 2 -> becomes X
    // (2,3): neighbors (1,3)=. (from step1 state), (2,2)=. -> 0 -> stays .
    // After step 2:
    // XXXX
    // XXXX
    // .XXX
    //
    // Step 3:
    // (2,0) is already X, (2,3): neighbors (1,3)=X, (2,2)=X -> 2 -> becomes X
    // After step 3:
    // XXXX
    // XXXX
    // XXXX -> 12 squares.

    // So the final answer is 12.

    // How to compute this efficiently?
    // We can simulate the process using a queue and a count of weed neighbors.
    // For each empty cell, maintain a count of how many of its neighbors are weed.
    // When a cell becomes weed, increment the neighbor count of all its adjacent cells.
    // When any cell's count reaches 2, it becomes weed in the next iteration.

    // Algorithm:
    // 1. Create a grid state and a neighbor_count array.
    // 2. Initialize: for each weed cell, mark it and for each of its neighbors, increment neighbor_count.
    // 3. Use a queue: initially, no empty cells are weed yet.
    // 4. For each empty cell, if neighbor_count >= 2, add it to a queue for next iteration.
    // 5. Process the queue: for each cell that becomes weed, update its neighbors' counts and check if any neighbor now has count >=2 and is still empty.

    // Steps:
    // - We'll use a visited/occupied array to mark which cells are weed.
    // - We'll use a count array for how many weed neighbors each cell has.
    // - Use a queue to process newly occupied cells.

    vector<vector<bool>> isWeed(N, vector<bool>(M, false));
    vector<vector<int>> neighborWeedCount(N, vector<int>(M, 0));
    queue<pair<int, int>> q2;

    // Initialize: mark original weed and count neighbors
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == 'X') {
                isWeed[i][j] = true;
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    if (ni >= 0 && ni < N && nj >= 0 && nj < M) {
                        neighborWeedCount[ni][nj]++;
                    }
                }
            }
        }
    }

    // Find all empty cells that have >=2 weed neighbors initially
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (!isWeed[i][j] && neighborWeedCount[i][j] >= 2) {
                q2.push({i, j});
                isWeed[i][j] = true; // mark immediately to avoid duplicate pushes
            }
        }
    }

    // Process the queue
    while (!q2.empty()) {
        auto [x, y] = q2.front();
        q2.pop();
        // This cell is now weed; update neighbors
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx >= 0 && nx < N && ny >= 0 && ny < M && !isWeed[nx][ny]) {
                neighborWeedCount[nx][ny]++;
                if (neighborWeedCount[nx][ny] >= 2) {
                    isWeed[nx][ny] = true;
                    q2.push({nx, ny});
                }
            }
        }
    }

    // Count total weed squares
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (isWeed[i][j]) {
                count++;
            }
        }
    }

    cout << count << endl;

    return 0;
}
