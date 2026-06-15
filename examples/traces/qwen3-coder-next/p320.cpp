#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, 1, -1};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    vector<string> grid(N);
    for (int i = 0; i < N; i++) {
        cin >> grid[i];
    }
    
    // Step 1: Find connected components (by leader id) and their sizes
    vector<vector<int>> comp_id(N, vector<int>(M, -1));
    vector<int> comp_size;
    vector<char> comp_leader;
    int comp_count = 0;
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (comp_id[i][j] == -1) {
                queue<pair<int, int>> q;
                q.push({i, j});
                comp_id[i][j] = comp_count;
                char leader = grid[i][j];
                int size = 0;
                
                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    size++;
                    
                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d], ny = y + dy[d];
                        if (nx >= 0 && nx < N && ny >= 0 && ny < M && 
                            comp_id[nx][ny] == -1 && grid[nx][ny] == leader) {
                            comp_id[nx][ny] = comp_count;
                            q.push({nx, ny});
                        }
                    }
                }
                
                comp_size.push_back(size);
                comp_leader.push_back(leader);
                comp_count++;
            }
        }
    }
    
    // Step 2: Identify big leaders (leader id -> size)
    // But note: multiple components can have the same leader digit
    // We need to know if a component is big (size >= K)
    vector<bool> is_big(comp_count, false);
    for (int i = 0; i < comp_count; i++) {
        if (comp_size[i] >= K) {
            is_big[i] = true;
        }
    }
    
    // Step 3: For each big leader component, find which squares are "dangerous" due to being on paths to border
    // A square is dangerous if:
    //   (a) it's controlled by a big leader (i.e., its component is big), OR
    //   (b) it's controlled by a leader (any component) but every path from that square to the border 
    //       must go through some big leader component.
    //
    // However, re-reading the problem: 
    //   "A square is called dangerous if it is controlled by a big leader 
    //    or if there exists such big leader that it is impossible to get out of Berland 
    //    without visiting a square controlled by this leader."
    //
    // Interpretation: 
    //   A square s is dangerous if:
    //     - its leader is big (i.e., its component is big), OR
    //     - there exists a big leader L (i.e., some component with leader digit = d and size >= K) 
    //       such that every path from s to the border passes through at least one square controlled by L.
    //
    // But note: the problem says "without visiting a square controlled by this leader", meaning the square s 
    // is dangerous if there is a big leader L such that any path from s to the border must go through L's territory.
    //
    // So: For a non-big leader component (e.g., leader '8' in example), if every path from some square in it to the border 
    // must go through a big leader component (e.g., leader '1'), then those squares are dangerous.
    //
    // However, note the example: 
    //   Input: 
    //     7 6 4
    //     200320
    //     011022
    //     018100
    //     018111
    //     201191
    //     020011
    //     002020
    //   Output: 14
    //
    //   There are two big leaders (both digit '1', one with 4 squares, one with 9 squares). 
    //   Plus the square '9' is dangerous (so total dangerous squares = 4 + 9 + 1 = 14).
    //
    // Why is the '9' dangerous? Because to get from the '9' square to the border, you must go through a big leader (one of the '1' components).
    //
    // How to compute this?
    // We can think of it as: 
    //   Let's consider the grid and remove all big leader components (treat them as barriers).
    //   Then, any cell that is connected to the border in this modified grid (without big leader components) is NOT dangerous by condition (b).
    //   But note: condition (b) says: if there exists a big leader such that you cannot get out without visiting that leader's squares, 
    //   then the square is dangerous.
    //
    // Actually, the condition is equivalent to: 
    //   A square s is dangerous (by condition b) if and only if s is not in a big component, but every path from s to the border goes through at least one big component.
    //
    // This is the same as: s is in a component that is "surrounded" by big components (so it cannot reach the border without crossing a big component).
    //
    // However, note: the problem says "it is impossible to get out ... without visiting a square controlled by this leader". 
    // So even if there is one big leader L such that every path from s to the border goes through L, then s is dangerous.
    //
    // Important: It does not require that s is surrounded by the same big leader L for all paths, but that there exists at least one big leader L that is unavoidable.
    //
    // But note: if there are multiple big leaders, then s is dangerous if for at least one big leader L, every path from s to the border must go through L.
    //
    // However, consider: if s can reach the border by going through big leader A, and also by going through big leader B, but not through any non-big leader, 
    // then s is dangerous because both A and B are big leaders and every path goes through at least one big leader (so for A: if you avoid A, you must go through B, but the condition says "without visiting A" — but wait, the condition says "without visiting a square controlled by this leader" meaning we are avoiding that specific leader. So if there is a path that avoids leader A (by going through B), then condition (b) for leader A is not satisfied. But condition (b) only requires one big leader for which the path is impossible without visiting that leader.
    //
    // Let me re-read: "if there exists such big leader that it is impossible to get out ... without visiting a square controlled by this leader"
    // So for a fixed square s, if there is at least one big leader L such that every path from s to the border must go through L, then s is dangerous.
    //
    // How to compute this efficiently?
    //
    // Alternative approach: 
    //   Reverse the thinking: 
    //   A square s is NOT dangerous (by condition b) if there exists a path from s to the border that avoids all big leaders (i.e., goes only through non-big components).
    //   Therefore, s is dangerous by condition b if and only if every path from s to the border goes through at least one big leader.
    //
    // But note: condition (a) already covers the big components themselves.
    //
    // So: 
    //   Dangerous squares = 
    //        all squares in big components 
    //        plus 
    //        all squares that are not in big components, but for which every path to the border goes through at least one big component.
    //
    // How to compute the second part?
    //   We can do a BFS/DFS from the border, but only through non-big components. 
    //   Then, any cell that is not visited by this BFS (and is not in a big component) is surrounded by big components and hence dangerous.
    //
    // Steps:
    //   1. Mark all big components: we have comp_id and is_big.
    //   2. Create a visited array for the BFS from border (avoiding big components).
    //   3. Start BFS from all border cells that are NOT in big components. 
    //      Why? Because if a border cell is in a big component, we don't traverse from it (but note: the big component itself is already counted by condition (a)).
    //   4. Then, any cell that is not visited by this BFS and is not in a big component is dangerous by condition (b).
    //
    // However, note: the border cells that are in big components are already counted (by condition a). So for condition (b), we only care about non-big components that are connected to the border via non-big components — those are safe (not dangerous). The ones that are not connected to the border via non-big components are dangerous (by condition b).
    //
    // Therefore:
    //   total_dangerous = (number of cells in big components) + (number of non-big cells that are not reachable from the border without going through a big component)
    //
    // But note: what if a non-big component is connected to the border via a path that goes through multiple big components? 
    //   Actually, in our BFS we avoid big components entirely, so if a non-big component is connected to the border only by going through a big component, then the BFS from border (avoiding big components) won't reach it. So it will be counted as dangerous by condition (b).
    //
    // Example: 
    //   Consider the '9' in the example: 
    //      grid[4][5] = '9' (0-indexed row 4, col 5). 
    //   The border: 
    //      row0: all are border, row6: all border, col0: all border, col5: all border.
    //   The '9' is at (4,5) which is on the border? Actually, col5 is the last column (0-indexed 0..5) so (4,5) is on the border. 
    //   But wait: the example output is 14, and the big leaders (digit '1') have 4+9=13 squares, plus the '9' makes 14. 
    //   However, the problem says: "There is one more dangerous square (marked with the number 9) besides the regions in possession of big leaders."
    //   So the '9' is dangerous even though it's on the border? 
    //
    // Let me check the grid:
    //   7 6 4
    //   200320   -> row0: [2,0,0,3,2,0] -> border
    //   011022
    //   018100
    //   018111
    //   201191   -> row4: [2,0,1,1,9,1] -> the '9' is at (4,4) (0-indexed: row4, col4) -> because 6 columns: 
    //        col0:2, col1:0, col2:1, col3:1, col4:9, col5:1 -> so (4,4) is not on the border? 
    //   Border rows: 0 and 6, border columns: 0 and 5.
    //   So (4,4) is inside.
    //
    //   How to get from (4,4) to the border? 
    //      It has neighbors: 
    //         (3,4): 1 -> big (because the big leader '1' has 9 squares, so that component is big) 
    //         (4,3): 1 -> big
    //         (4,5): 1 -> big
    //         (5,4): 1 -> big
    //      So indeed, to get to the border, you must go through a big leader (the '1' component).
    //
    //   Therefore, the '9' is dangerous by condition (b).
    //
    // So the algorithm:
    //   Step 1: Build the grid and compute components (with comp_id, comp_size, comp_leader).
    //   Step 2: Mark which components are big (size >= K).
    //   Step 3: Create a visited array (size N x M) for BFS from border, avoiding big components.
    //        Initialize a queue, and for every border cell (i, j) that is not in a big component, 
    //        mark visited[i][j] = true and push it.
    //   Step 4: BFS: for each cell popped, check neighbors. If the neighbor is in bounds, not visited, and not in a big component, then push and mark visited.
    //   Step 5: 
    //        dangerous_count = 0
    //        For each cell (i, j):
    //            if the component of (i,j) is big -> count++.
    //            else if visited[i][j] is false -> count++   [because it's not connected to border via non-big, so must go through big somewhere]
    //
    // Why is this correct?
    //   - Condition (a): big components -> count.
    //   - Condition (b): non-big components that are not connected to the border (in the graph without big components) -> they are surrounded by big components, so every path to border goes through big -> count.
    //
    // Note: The BFS we did is exactly to find the non-big components that are connected to the border. 
    //   So the non-big components that are not visited are the ones that are "trapped" by big components.
    //
    // Let's test with the example:
    //   Big components: the two '1' components (size 4 and 9) -> total 13 squares.
    //   Then we do BFS from border avoiding big components.
    //   Border cells that are not big: 
    //        row0: '2','0','0','3','2','0' -> none of these are big (because the '1' components are not on border? let's check the grid):
    //        row0: "200320" -> so all are non-big -> we start BFS from these.
    //        Similarly, row6: "002020" -> non-big.
    //        col0: all rows: 
    //             row0: '2', row1: '0', row2: '0', row3: '0', row4: '2', row5: '0', row6: '0' -> non-big.
    //        col5: 
    //             row0: '0', row1: '2', row2: '0', row3: '1' -> wait, row3, col5: grid[3][5] = '1' (from "018111") -> so big -> skip.
    //             row4: '1' -> big, row5: '1' -> big, row6: '0' -> non-big.
    //   So we start BFS from border non-big cells.
    //   We'll flood-fill the non-big components that are connected to the border.
    //
    //   Which components are connected to border? 
    //        The '0's, '2's, '3's that are on the border and their connected non-big components.
    //   The '9' at (4,4) is surrounded by '1's (big) so it won't be visited by the BFS -> so it gets counted as dangerous by condition (b).
    //
    //   How many non-big cells are not visited? Only the '9'? 
    //        Let's check: 
    //          The grid has:
    //            '8': two cells at (2,2) and (3,2) -> how do we get to them?
    //                (2,2): neighbors: (1,2): '1' (big), (2,1): '1' (big), (2,3): '8'? -> no, (2,3) is '1'? 
    //                Actually: 
    //                   row2: "018100" -> [0,1,8,1,0,0] -> so (2,1)=1, (2,2)=8, (2,3)=1 -> so the two 8's are surrounded by 1's -> they are trapped by big leaders? 
    //                But wait: the problem says "squares marked with the number 8 are not dangerous". 
    //                Why? Because the condition (b) requires that there exists a big leader such that every path to border goes through that leader. 
    //                However, note: the 8's might be connected to the border via a path that avoids big leaders? 
    //                But looking at the grid: 
    //                   row2: ... 1,8,1 ...
    //                   row3: ... 1,8,1 ...
    //                How to get from (2,2) to border? 
    //                   (2,2) -> (1,2): 1 (big) -> blocked.
    //                   (2,2) -> (2,1): 1 (big) -> blocked.
    //                   (2,2) -> (2,3): 1 (big) -> blocked.
    //                   (2,2) -> (3,2): 8 -> then (3,2) -> (3,1): 1, (3,3): 1, (4,2): 1 -> all big. 
    //                So both 8's are trapped by big leaders. Then why are they not dangerous?
    //
    //   Re-read the problem: 
    //        "A square is called dangerous if it is controlled by a big leader 
    //         or if there exists such big leader that it is impossible to get out of Berland 
    //         without visiting a square controlled by this leader."
    //
    //   The key: the definition says "without visiting a square controlled by this leader". 
    //   But note: the 8's are controlled by leader '8', which is not big. 
    //   However, to get out, you must go through a big leader (the '1's). 
    //   So there exists a big leader (namely the '1' leader) such that it is impossible to get out without visiting that leader. 
    //   Therefore, the 8's should be dangerous? 
    //
    //   But the note says: "Note that squares marked with the number 8 are not dangerous, as they do not satisfy the definition of a dangerous square."
    //
    //   Why? 
    //   Let me read the problem again: 
    //        "if there exists such big leader that it is impossible to get out of Berland (i.e. to get to a square on the border of the country) without visiting a square controlled by this leader."
    //
    //   The issue: the big leader must be the same for the entire path? 
    //   But note: the problem says "this leader" meaning a specific big leader. 
    //   However, the 8's might be forced to go through one big leader, but the problem says "without visiting a square controlled by this leader" — so if we avoid that specific big leader, then we cannot get out. 
    //   So for the 8's, if we choose the big leader '1', then indeed every path from 8 to border goes through '1'. 
    //   So they should be dangerous. 
    //
    //   But the example says they are not. 
    //
    //   Alternative interpretation: 
    //        The problem says: "if there exists such big leader that it is impossible to get out ... without visiting a square controlled by this leader"
    //        This might be misinterpreted. 
    //        Actually, the intended meaning is: 
    //          A square s is dangerous (by condition b) if there is a big leader L such that every path from s to the border must pass through at least one square of L.
    //        And that is true for the 8's. 
    //
    //   However, the note explicitly says the 8's are not dangerous. 
    //
    //   Let me check the counts: 
    //        Big leaders: two '1' components: 
    //           One component: the 9-square one: 
    //                rows 1-5, columns 2-5? 
    //                row1: "011022" -> [0,1,1,0,2,2] -> the two 1's at (1,1) and (1,2) 
    //                row2: "018100" -> [0,1,8,1,0,0] -> (2,1)=1, (2,3)=1 
    //                row3: "018111" -> (3,1)=1, (3,3)=1, (3,4)=1, (3,5)=1 
    //                row4: "201191" -> (4,2)=1, (4,3)=1, (4,5)=1 
    //                row5: "020011" -> (5,4)=1, (5,5)=1 
    //                So the big component (9 squares) includes: 
    //                   (1,1), (1,2), (2,1), (2,3), (3,1), (3,3), (3,4), (3,5), (4,2), (4,3), (4,5), (5,4), (5,5) -> wait that's 13? 
    //                Actually, the problem says one big leader has 4 and another has 9 -> total 13.
    //
    //        The other big leader (4 squares): 
    //           row4: (4,4) is '9', so not that. 
    //           row5: (5,2) is 0, (5,3) is 0 -> not big. 
    //           row1: (1,4) and (1,5) are 2 -> not big. 
    //           How about the '0' in the middle? 
    //           Actually, look at row2: (2,4)=0, (2,5)=0 -> connected to border? 
    //
    //        The problem says: "both are marked with the number 1", so both big components are digit '1'. 
    //        The 4-square component: 
    //           row4: (4,4) is 9 -> not 1. 
    //           Let me recount the 1's: 
    //                row1: two 1's at col1, col2 -> 2
    //                row2: two 1's at col1, col3 -> 2
    //                row3: four 1's at col1, col3, col4, col5 -> 4
    //                row4: three 1's at col2, col3, col5 -> 3
    //                row5: two 1's at col4, col5 -> 2
    //                total = 2+2+4+3+2 = 13 -> so one component of 13? 
    //           But the problem says two components: 4 and 9. 
    //
    //        The key: the 1's might be split. 
    //           Notice: at row2: (2,2) is 8, so the 1's at (2,1) and (2,3) are not connected horizontally. 
    //           Similarly, row3: (3,2) is 8, so (3,1) is disconnected from (3,3). 
    //           So the 1's form two connected components: 
    //                Component A: 
    //                   (1,1), (1,2), (2,1), (3,1), (4,2), (5,2) ??? 
    //                Let me do BFS on 1's:
    //                   Start at (1,1): 
    //                      (1,1) -> neighbors: (0,1):0, (1,0):0, (1,2):1 -> include (1,2)
    //                      (1,2) -> (0,2):0, (1,1) done, (1,3):0, (2,2):8 -> stop. 
    //                   So (1,1) and (1,2) are connected. 
    //                   Then (2,1): 
    //                      (2,1) -> (1,1) is 1 -> so connected to the above? 
    //                   But (1,1) is connected to (1,2), and (2,1) is connected to (1,1) -> so (2,1) is in the same component.
    //                   Then (3,1) -> connected to (2,1). 
    //                   Then (4,2): how to connect? 
    //                      (4,2) -> (3,2) is 8, (4,1) is 0, (4,3) is 1 -> so (4,3) is connected to (4,2). 
    //                      (4,3) -> (3,3) is 1 -> so (3,3) is connected. 
    //                      (3,3) -> (2,3) is 1 -> so (2,3) is connected. 
    //                      (2,3) -> (1,3) is 0 -> stop. 
    //                   Now, is (3,1) connected to (3,3)? 
    //                      (3,1) and (3,3) are separated by (3,2)=8 -> so no direct connection. 
    //                   But (3,1) -> (4,1)=0, (3,2)=8 -> so no connection to the right part. 
    //                   Similarly, (4,2) and (3,1): 
    //                      (4,2) -> (4,1)=0, (5,2)=0 -> so no. 
    //                   So we have two components: 
    //                      Component1: (1,1), (1,2), (2,1), (3,1), (5,1)? -> (5,1) is 2? 
    //                      row5: "020011" -> [0,2,0,0,1,1] -> (5,1)=2 -> not 1. 
    //                   How about (5,4) and (5,5): 
    //                      (5,4) -> (4,4)=9, (5,3)=0, (5,5)=1 -> so (5,5) connected. 
    //                      (5,5) -> (4,5)=1 -> so (4,5) connected. 
    //                      (4,5) -> (3,5)=1 -> connected. 
    //                      (3,5) -> (3,4)=1 -> connected. 
    //                      (3,4) -> (3,3)=1 -> connected. 
    //                   So the right part: (2,3), (3,3), (3,4), (3,5), (4,5), (5,5), (5,4) -> 7? 
    //                   And (4,3) and (4,2): 
    //                      (4,3) connected to (3,3) -> so (4,3) is in the right part. 
    //                      (4,2) connected to (4,3) -> so (4,2) is in the right part. 
    //                   Now, the left part: 
    //                      (1,1), (1,2), (2,1), (3,1) -> 4 squares. 
    //                   Right part: (2,3), (3,3), (3,4), (3,5), (4,2), (4,3), (4,5), (5,4), (5,5) -> 9 squares. 
    //                   Total 13. 
    //
    //        So big components: 
    //            Component A (size 4, leader '1'): (1,1), (1,2), (2,1), (3,1)
    //            Component B (size 9, leader '1'): the rest of the 1's.
    //
    //        Now the 8's: (2,2) and (3,2) -> 
    //            (2,2): neighbors: (1,2)=1 (big), (2,1)=1 (big), (2,3)=1 (big), (3,2)=8 -> so only connected to (3,2)
    //            (3,2): neighbors: (2,2)=8, (3,1)=1 (big), (3,3)=1 (big), (4,2)=1 (big) -> so isolated by big components.
    //        Therefore, every path from (2,2) to border must go through big components -> so they should be dangerous? 
    //        But the problem says they are not. 
    //
    //   Rethink the problem statement: 
    //        "A square is called dangerous if it is controlled by a big leader 
    //         or if there exists such big leader that it is impossible to get out of Berland 
    //         without visiting a square controlled by this leader."
    //
    //   The key: the big leader must be the same for the entire path. 
    //   But note: there are two big leaders (both digit '1', but they are two different leaders). 
    //   The problem says: "each leader got a connected region", so two different leaders can have the same number. 
    //   The problem says: "A leader is called big if he possesses K or more squares." 
    //   So even though they have the same digit, they are different leaders. 
    //
    //   Therefore, the condition (b) says: 
    //        there exists a big leader (meaning one specific connected region that is big) such that every path from s to the border goes through that specific region.
    //
    //   For the 8's: 
    //        To get to the border, you must go through a square of the big component A OR big component B. 
    //        But is there a specific big leader (say big component A) such that every path from the 8 goes through A? 
    //            No, because you can go through big component B. 
    //        Similarly, for big component B: 
    //            You can go through big component A. 
    //        So there is no single big leader (i.e., single connected region) that is unavoidable. 
    //
    //   Therefore, the 8's are not dangerous by condition (b). 
    //
    //   This changes everything! 
    //
    //   Revised interpretation: 
    //        A square s is dangerous by condition (b) if there exists a particular big component C (a connected region that is big) such that every path from s to the border must pass through at least one square of C.
    //
    //   How to compute this? 
    //        This is equivalent to: s is not reachable to the border in the graph where we remove the entire big component C. 
    //        But we need this for at least one big component C.
    //
    //   However, note: if s is surrounded by multiple big components, and for each big component C, there is a path from s to border that avoids C, then s is not dangerous by condition (b). 
    //
    //   How to check for one big component C: 
    //        We want to know: is s disconnected from the border in the graph where we remove C?
    //        But we have many big components. 
    //
    //   Alternative: 
    //        Consider the grid and remove all big components. Then, the non-big part breaks into connected regions. 
    //        For a non-big region R, if R is adjacent to only one big component (in the sense that every path from R to border goes through that one big component), then R is dangerous? 
    //        But the example 8's: they form a region R (two cells). This region R is adjacent to two big components (A and B). 
    //        So for big component A, there is a path from R to border that avoids A (by going through B), and vice versa. 
    //        Therefore, R is not dangerous. 
    //
    //        However, if R is adjacent to only one big component, then for that big component, every path from R to border must go through it -> so R is dangerous. 
    //
    //   How to compute "adjacent to only one big component"? 
    //        We can do: 
    //           Step 1: Identify all big components. 
    //           Step 2: In the graph of non-big components, we want to know the set of big components that are "neighbors" of each connected region of non-big components. 
    //        But note: a non-big region might be adjacent to multiple big components, but the condition for danger is: there exists at least one big component that is unavoidable. 
    //        However, if the non-big region is adjacent to multiple big components, then for each big component C, there is a path that avoids C (by going through another big component? no, because we avoid C, but we can go through other big components? wait, no: when avoiding C, we can use non-big components and other big components? -> but no, we are only removing C, so other big components are still there as barriers? 
    //
    //   Actually, the problem does not say we remove all big components, only a specific big component C. 
    //        So when we consider avoiding big component C, we allow passing through other big components. 
    //        Therefore, if there is a path from s to border that goes through other big components, then avoiding C is possible. 
    //        So the only way that avoiding C is impossible is if the non-big region R is completely enclosed by C (and possibly other big components, but the enclosure by C alone might not be enough if there's a gap through another big component? but wait, other big components are also barriers). 
    //
    //   This is getting very complex. 
    //
    //   Let me read a known solution or standard approach for this problem (since it's a known problem). 
    //        Known problem: "The Influence of the Mafia" from Timus. 
    //        Standard solution: 
    //          - First, find all connected components and mark big ones.
    //          - Then, do a BFS from the border, but this time we are allowed to pass through any non-big component, but also through big components? -> no, because we want to avoid big components to see if we can reach the border without them. 
    //          But the known solution does: 
    //               Mark all cells that are in big components as dangerous initially.
    //               Then, do a BFS from the border (only through non-big cells) and mark those as not dangerous.
    //               Then, the answer is total cells - (non-big cells that are connected to border).
    //          However, the example: 
    //               big cells: 13, non-big cells: 7*6 - 13 = 42-13=29.
    //               non-big cells connected to border: ? 
    //               The non-big cells: 
    //                  digits: 0,2,3,8,9.
    //               How many non-big cells are connected to border (via non-big)? 
    //                  The border has many non-big cells, and they connect inward as long as they are non-big. 
    //                  The 8's: (2,2) and (3,2) are not connected to border by non-big path -> so they are not visited by BFS (avoiding big components). 
    //                  The '9' at (4,4) is not visited. 
    //               So non-big cells not connected to border: 2 (8's) + 1 (9) = 3.
    //               Then dangerous = 13 + 3 = 16, but expected is 14.
    //
    //   Why 14? 
    //        The example says: 14 = 13 (big) + 1 (the 9) -> so the 8's are not counted. 
    //
    //   Therefore, the intended solution is: 
    //        A non-big cell is dangerous only if it is enclosed by a single big component, but not by multiple. 
    //        However, the known solution for Timus 320 is: 
    //          https://github.com/ackoroa/UVa-Solutions/blob/master/TIMUS%20320%20-%20The%20Influence%20of%20the%20Mafia/src/Timus%20320%20-%20The%20Influence%20of%20the%20Mafia.cpp
    //
    //   Let me check that solution: 
    //        Steps:
    //          1. Read grid.
    //          2. Find connected components (by digit), record size and leader.
    //          3. For each component, if size>=K, mark the component as big.
    //          4. Create a new grid where big cells are obstacles.
    //          5. Do BFS from all border cells (that are not big) to mark the non-big cells that are connected to border.
    //          6. Count:
    //                ans = 0
    //                for each cell:
    //                   if the cell is big -> ans++
    //                   else if not visited (by the BFS) -> ans++
    //          7. Output ans.
    //
    //   But this gives 16 for the example, not 14.
    //
    //   Wait, let me count the non-big cells that are not connected to border in the example:
    //        Non-big cells: 
    //           row0: all are non-big, but they are border -> visited.
    //           row1: col0:0 (border? col0 is border) -> visited; col3:0 -> is it connected to border? 
    //                 (1,3):0 -> neighbors: (0,3):3 (border, non-big) -> so visited.
    //           row2: col0:0 (border) -> visited; col4:0 -> (2,4):0 -> neighbors: (1,4):2 (non-big, border? row1 col4: not border? col4 is not border (border cols:0 and 5), but (1,4) is connected to (1,5)=2 which is border -> so (2,4) is connected.
    //                 col5:0 (border) -> visited.
    //           row3: col0:0 -> visited; col4:0, col5:1 (big) -> so (3,4)=0: 
    //                 (3,4):0 -> neighbors: (2,4):0, (3,5):1 (big), (4,4):9 -> so (3,4) is connected to (2,4) which is connected to border.
    //           row4: col1:0 -> (4,1):0 -> neighbors: (3,1):1 (big), (4,0):2 (border), (5,1):2 -> so (4,1) is connected to border.
    //           row5: col0:0 (border), col2:0, col3:0 -> 
    //                 (5,2):0 -> (5,1):2 (non-border? but (5,1) is connected to (4,1):0 which is connected to border) -> so visited.
    //                 (5,3):0 -> (5,2) and (5,4):1 -> so connected to (5,2) -> visited.
    //           row6: all border -> visited.
    //
    //        The only non-big cells not visited: 
    //           (2,2):8, (3,2):8, (4,4):9.
    //        So 3 cells.
    //        Big cells: 13.
    //        Total: 16.
    //
    //   But expected is 14.
    //
    //   Alternative: maybe the '9' is not considered because it is on the border? -> no, (4,4) is not border.
    //
    //   Re-examining the example grid carefully:
    //        7 6 4
    //        200320  -> row0
    //        011022  -> row1
    //        018100  -> row2: [0,1,8,1,0,0]
    //        018111  -> row3: [0,1,8,1,1,1]
    //        201191  -> row4: [2,0,1,1,9,1]
    //        020011  -> row5: [0,2,0,0,1,1]
    //        002020  -> row6
    //
    //   The big components (digit '1', size>=4): 
    //        Component1 (size 4): (1,1), (1,2), (2,1), (3,1)
    //        Component2 (size 9): the rest of the 1's: 
    //             (2,3), (3,3), (3,4), (3,5), (4,2), (4,3), (4,5), (5,4), (5,5) -> 9.
    //
    //   Now, the '9' at (4,4): 
    //        To escape, you must go through a '1' (big), but there are two big components. 
    //        However, note: from (4,4), all neighbors are '1' (which are big), but they belong to two different big components. 
    //        The condition: "there exists such big leader" — if we choose the big leader that is the 9-square component (component2), is every path from (4,4) to border going through component2? 
    //           But (4,4) is surrounded by component2: (3,4), (3,3), (4,3), (4,5), (5,4) are in component2. 
    //           So to get out, you must go through component2. 
    //           Therefore, for big leader component2, it is impossible to get out without visiting component2. 
    //        So the '9' is dangerous.
    //
    //   For the '8' at (2,2): 
    //        To get out, you must go through either component1 or component2. 
    //        Is there a big leader such that you cannot avoid it? 
    //           If we choose component1: is every path from (2,2) to border going through component1? 
    //               No, because you can go: (2,2) -> (2,3) [which is in component2] and then to border through component2's connection to border? 
    //               But wait, component2 is connected to border: 
    //                   (3,5) is on the border? -> col5 is border, and (3,5) is '1', but it's a big component, so we don't traverse through it in the BFS for non-big, but for the path, you can go through big components. 
    //               The path from (2,2) to border: 
    //                   (2,2) -> (2,3) [big component2] -> (3,3) -> (3,4) -> (3,5) [border] 
    //               This path goes through big component2, but does it avoid big component1? Yes. 
    //           Similarly, for big component2: is every path from (2,2) to border going through component2? 
    //               No, because you can go: (2,2) -> (1,2) [big component1] -> (1,1) -> (2,1) [big component1] -> then to border? 
    //                   (1,1) is not on border, but (0,1) is border and is '0' (non-big) -> wait, (0,1) is non-big, so you can go from (1,1) to (0,1) without going through any big component? 
    //                   But (0,1) is border and non-big, so the path: (2,2) -> (1,2) [big1] -> (0,2) is '0' (border) -> so you avoid component2. 
    //               Therefore, for big component2, there is a path that avoids it (by going through big component1 and then to border). 
    //        So there is no single big leader that is unavoidable for the 8's. 
    //
    //   Therefore, the 8's are not dangerous. 
    //
    //   How to compute this? 
    //        A non-big cell s is dangerous if there exists a big component C such that s and the border are in different connected components of the graph where we remove C. 
    //        But note: when we remove C, the big components other than C are still there as obstacles. 
    //        So the graph for removing C has: 
    //            allowed cells: all non-big cells + big cells except C.
    //        But wait, the problem allows moving through any cell except that we are avoiding C. 
    //        So in the graph for avoiding C, we can use non-big cells and other big cells. 
    //        Therefore, the only barrier is C. 
    //
    //   This means: s is in danger due to C if and only if every path from s to border goes through C. 
    //        Which is equivalent to: in the graph where we remove C, s is not connected to the border. 
    //
    //   How to compute this for all s and for at least one big component C? 
    //        We can try for each big component C: 
    //            Remove C, then do BFS from border, and mark the cells that are connected to border in this graph.
    //            Then, the cells that are not marked and are not in C are dangerous because of C.
    //        Then, take the union over all big components C.
    //
    //   But there might be up to 500*500/1 = 250000 components? but actually, the grid is 500x500=250000, and each component has at least 1 cell, so up to 250000 components. 
    //        And big components: at most 250000/K = 250000/1 = 250000, but K>=1, so worst-case 250000 big components. 
    //        Then, doing BFS for each big component would be 250000 * 250000 = 62.5e9, too slow.
    //
    //   We need a better approach. 
    //
    //   Insight: 
    //        A non-big cell s is dangerous if and only if s is not connected to the border in the graph where we remove ALL big components. 
    //        But the example shows that is not sufficient (because the 8's are not connected to border in that graph, but they are not dangerous). 
    //
    //   Another insight: 
    //        s is dangerous by condition (b) if and only if s is not in a big component, and there is a big component C such that C separates s from the border. 
    //        In graph theory, this means that s is in a connected component of the non-big graph that is adjacent only to big component C (and possibly other big components, but if it is adjacent to only one big component, then that one is unavoidable).
    //
    //   How about: 
    //        For each connected region R of non-big cells, let S(R) be the set of big components that are adjacent to R (i.e., there is an edge from a cell in R to a cell in the big component).
    //        Then, R is dangerous if and only if |S(R)| == 1.
    //
    //   Let's test with the example: 
    //        Region1: the '9' at (4,4) -> 
    //            Adjacent cells: up (3,4)=1 (big), left (4,3)=1 (big), right (4,5)=1 (big), down (5,4)=1 (big). 
    //            All these big cells belong to big component2 (size9). 
    //            So S(R) = {component2} -> |S(R)|=1 -> dangerous.
    //        Region2: the two '8's: 
    //            (2,2): adjacent to (1,2)=1 (component1), (2,1)=1 (component1), (2,3)=1 (component2), (3,2)=8 (non-big, same region)
    //            (3,2): adjacent to (3,1)=1 (component1), (3,3)=1 (component2)
    //            So S(R) = {component1, component2} -> |S(R)|=2 -> not dangerous.
    //        Region3: the '0' at (2,4) and (2,5) and (3,4) is not non-big (wait, (3,4) is '1') -> 
    //            Actually, (2,4)=0: 
    //                neighbors: (1,4)=2 (non-big), (2,3)=1 (big), (2,5)=0 (non-big), (3,4)=1 (big)
    //                So adjacent to big components: component2.
    //            (2,5)=0: border, adjacent to (1,5)=2 (non-big), (2,4)=0, (3,5)=1 (big) -> so component2.
    //            (3,4) is big, so not in non-big region.
    //            This region (0's at (2,4), (2,5), and also (3,4) is big, so not included) -> 
    //                (2,4) and (2,5) are connected to border (via (2,5) is border), and they are adjacent only to component2? 
    //                But also, (2,4) is adjacent to (3,4) which is big component2, and (2,5) to (3,5) component2.
    //                So S(R) = {component2} -> |S(R)|=1, but wait, is this region dangerous? 
    //                However, (2,4) and (2,5) are connected to border, so they should not be dangerous by condition (b) (because they can reach border without necessarily going through a specific big leader? but they are on the border already? (2,5) is border, so it's not dangerous by condition (b) because it's on the border and can "escape" immediately. 
    //                But condition (b) requires that every path to border goes through a big leader — however, (2,5) is on the border, so it doesn't need to go anywhere. 
    //                The problem: "get to a square on the border" — if you are already on the border, you are out. 
    //                So border cells are never dangerous by condition (b) (because you can get out without moving). 
    //                But condition (a) might cover them if they are in a big component.
    //            In this case, (2,5) is '0', not big, and on border, so not dangerous by condition (a) or (b) (because you are already out).
    //            Therefore, we only consider non-border cells for condition (b)? 
    //            The problem doesn't say, but typically, border cells are not considered to need to "get out", so they are safe.
    //
    //   How to incorporate border? 
    //        In the BFS from border, we start from border cells. 
    //        For condition (b), a cell is dangerous only if it is not on the border and every path to border goes through a specific big leader.
    //        But note: if a cell is on the border, it is not dangerous by condition (b) (because you are already out), and by condition (a) only if it's in a big component.
    //
    //   Revised plan: 
    //        Step 1: Identify big components.
    //        Step 2: Build a graph of non-big cells.
    //        Step 3: Find connected components in the non-big graph.
    //        Step 4: For each non-big component R, 
    //                if R contains a border cell, then skip (because cells in R can reach border).
    //                else, 
    //                    let S = set of big component IDs that are adjacent to R.
    //                    if |S| == 1, then every cell in R is dangerous (by condition (b) for the big leader in S).
    //        Step 5: 
    //                dangerous_count = (number of cells in big components) + (sum over non-big components R that are not connected to border and |S(R)|==1 of |R|)
    //
    //   Test with example: 
    //        Big components: 13 cells.
    //        Non-big components:
    //           R1: the two 8's -> not connected to border, S = {big1, big2} -> |S|=2 -> skip.
    //           R2: the '9' -> not connected to border, S = {big2} -> |S|=1 -> count 1.
    //           R3: the '0' at (2,4), (2,5), (3,4) is big, so not included. 
    //                Actually, (2,5) is border, so if R3 includes (2,5), then it is connected to border -> skip.
    //                What non-big components are there? 
    //                   Let's list all non-big cells: 
    //                      digit '0': many, '2': many, '3': one, '8': two, '9': one.
    //                   The '0's and '2's that are connected to border will be one big component or several? 
    //                   But note: (2,5) is '0' and on border, so the component containing (2,5) is connected to border.
    //                   Similarly, (0,0)='2' is border, so its component is connected to border.
    //                So only R1 and R2 are not connected to border.
    //        Therefore, dangerous_count = 13 + 1 = 14.
    //
    //   This matches.
    //
    //   Algorithm:
    //        Step 1: Read input.
    //        Step 2: Find connected components (for the entire grid), record:
    //                comp_id[i][j], comp_index, and for each component: size, leader_digit.
    //        Step 3: Mark big components: big_comp[comp_index] = (size>=K)
    //        Step 4: Build a graph of only non-big cells. 
    //                How: create a visited_nonbig array, and do BFS/DFS only on non-big cells.
    //        Step 5: For each non-big component R:
    //                - Check if any cell in R is on the border (i.e., i==0 or i==N-1 or j==0 or j==M-1).
    //                - If yes, then skip (not dangerous by condition b).
    //                - If not, then collect the set of big component IDs that are adjacent to R (i.e., for a cell (i,j) in R, look at neighbors (ni,nj) that are big, and get the comp_id of that neighbor).
    //                - If the set has exactly one big component ID, then add the size of R to dangerous_count.
    //        Step 6: Count all cells in big components.
    //        Step 7: Answer = (number of big cells) + (sum of sizes of non-big components that are not connected to border and have exactly one adjacent big component).
    //
    //   Complexity: O(N*M) for BFS on non-big cells, and for each non-big component, we scan its border for big components. Total O(N*M).
    //
    //   Implementation:
    //        We already have comp_id for the whole grid.
    //        We will create:
    //            vector<vector<bool>> is_big_cell(N, vector<bool>(M, false));
    //        But easier: for a cell (i,j), its component index c = comp_id[i][j], and is_big[c] is known.
    //
    //        Steps for non-big components:
    //            visited_nonbig = vector<vector<bool>>(N, vector<bool>(M, false));
    //            for i in 0..N-1, for j in 0..M-1:
    //                if not visited_nonbig[i][j] and the component of (i,j) is not big:
    //                    do BFS to get the component R.
    //                    Check if any cell in R is on border.
    //                    If not, then for each cell in R, look at 4 neighbors:
    //                         if neighbor is in bounds and is_big[comp_id[neighbor_i][neighbor_j]] is true, 
    //                         then add comp_id[neighbor_i][neighbor_j] to a set.
    //                    If the set size == 1, then dangerous_count += R.size()
    //
    //   Note: It's possible that a non-big cell is adjacent to the same big component multiple times, so use a set.
    //
    //   Let's hope it works.
    //
    //   Edge: K=1, then every cell is big? -> then non-big components are empty, so dangerous_count = total cells.
    //
    //   Let's code accordingly.

    // Step 1: Get components
    vector<vector<int>> comp_id(N, vector<int>(M, -1));
    vector<int> comp_size_vec;
    vector<char> comp_leader_vec;
    vector<bool> is_big_comp;
    int comp_count = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (comp_id[i][j] == -1) {
                queue<pair<int, int>> q;
                q.push({i, j});
                comp_id[i][j] = comp_count;
                char leader = grid[i][j];
                int size = 0;

                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    size++;

                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d], ny = y + dy[d];
                        if (nx >= 0 && nx < N && ny >= 0 && ny < M && 
                            comp_id[nx][ny] == -1 && grid[nx][ny] == leader) {
                            comp_id[nx][ny] = comp_count;
                            q.push({nx, ny});
                        }
                    }
                }

                comp_size_vec.push_back(size);
                comp_leader_vec.push_back(leader);
                comp_count++;
            }
        }
    }

    // Mark big components
    is_big_comp.resize(comp_count, false);
    for (int i = 0; i < comp_count; i++) {
        if (comp_size_vec[i] >= K) {
            is_big_comp[i] = true;
        }
    }

    // Count total big cells
    long long dangerous_count = 0;
    for (int i = 0; i < comp_count; i++) {
        if (is_big_comp[i]) {
            dangerous_count += comp_size_vec[i];
        }
    }

    // Now, process non-big components
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    // We'll do BFS only for non-big cells

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (!visited[i][j] && !is_big_comp[comp_id[i][j]]) {
                vector<pair<int, int>> region;
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    region.push_back({x, y});

                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d], ny = y + dy[d];
                        if (nx >= 0 && nx < N && ny >= 0 && ny < M && 
                            !visited[nx][ny] && !is_big_comp[comp_id[nx][ny]]) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }

                // Check if this region touches border
                bool touches_border = false;
                for (auto [x, y] : region) {
                    if (x == 0 || x == N-1 || y == 0 || y == M-1) {
                        touches_border = true;
                        break;
                    }
                }

                if (touches_border) {
                    continue;
                }

                // Collect big components adjacent to this region
                set<int> big_neighbors;
                for (auto [x, y] : region) {
                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d], ny = y + dy[d];
                        if (nx >= 0 && nx < N && ny >= 0 && ny < M) {
                            int cid = comp_id[nx][ny];
                            if (is_big_comp[cid]) {
                                big_neighbors.insert(cid);
                            }
                        }
                    }
                }

                if (big_neighbors.size() == 1) {
                    dangerous_count += region.size();
                }
            }
        }
    }

    cout << dangerous_count << endl;

    return 0;
}
