#include <bits/stdc++.h>
using namespace std;
int M,N;
char grid[75][10];
int em[75];
const int INF=1e8;
int dp[128],ndp[128];

// For a row with empty mask e, incoming coverage inc, outgoing coverage out,
// find min horizontal dominoes to place such that no two adjacent uncovered empty cells
// uncovered = e & ~inc & ~out & ~horiz_covered
// horiz domino at j: covers j,j+1, both must be in (e&~inc&~out)
int solve_row(int e, int inc, int out){
    int rem = e & ~inc & ~out;
    // scan left to right, dp on position
    // state: last uncovered position (or -1)
    // Actually: dp[j] = min dominoes for first j columns
    // But we also need to know if col j-1 is uncovered
    // State: (col, prev_uncovered) 
    // dp2[prev_uncov] = min dominoes
    vector<int> d(2,INF);
    d[0]=0; // prev col not uncovered (either not empty, or covered)
    // Initially before col 0
    // Process col by col
    // prev_uncov: was previous column an uncovered empty cell?
    int cur[2]={0,INF}; // cur[0]=prev not uncov, cur[1]=prev uncov
    // but col -1 doesn't exist, so prev=0
    cur[0]=0; cur[1]=INF;
    for(int j=0;j<N;j++){
        int nxt[2]={INF,INF};
        bool in_rem = (rem>>j)&1;
        if(!in_rem){
            // col j is not an empty uncovered candidate (either candle or covered)
            // regardless of prev, col j is "not uncovered"
            nxt[0]=min(nxt[0],cur[0]);
            nxt[0]=min(nxt[0],cur[1]);
        } else {
            // col j is in rem, can be uncovered or covered by horizontal
            // option A: leave uncovered
            // then prev must not be uncovered (else two adjacent)
            nxt[1]=min(nxt[1],cur[0]); // prev not uncov, cur uncov
            // if cur[1] finite, prev was uncov -> two adjacent -> invalid
            
            // option B: cover with horizontal domino (j,j+1)
            if(j+1<N && ((rem>>( j+1))&1)){
                // place domino covering j and j+1
                // after this, j+1 is also covered, so next iteration j+1 not uncov
                // we handle j+1 as covered
                // advance j by 1 extra
                // Actually let's handle: after placing horiz at j,j+1:
                // j+1 becomes covered, so at position j+1, it's "not uncov"
                // We'll handle this by skipping j+1
                // Hmm, this is tricky with the loop structure
                // Let me use a different approach
            }
        }
        cur[0]=nxt[0]; cur[1]=nxt[1];
    }
    // After last col, cur[1] is fine (no col to the right)
    int res = min(cur[0],cur[1]);
    return res; // This doesn't account for horizontal dominoes yet
}
