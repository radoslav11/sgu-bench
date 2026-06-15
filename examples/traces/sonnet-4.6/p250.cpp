#include<bits/stdc++.h>
using namespace std;
int N,M,grid[185][185],rgt[185][185];
int main(){
    // read input carefully - "Test #X" lines?
    // looking at sample, input has "Test #1" header
    string line;
    // try reading
    // actually let me just parse
    scanf("%d %d",&N,&M);
    for(int i=0;i<N;i++) for(int j=0;j<M;j++) scanf("%d",&grid[i][j]);
    // compute right[i][j]
    for(int i=0;i<N;i++){
        rgt[i][M]=0;
        for(int j=M-1;j>=0;j--) rgt[i][j]=grid[i][j]?0:rgt[i][j+1]+1;
    }
    long long best=-1;
    int bc,br1,bh1,bw1,bh2,bw2,bh3,bw3;
    // for each left column c
    for(int c=0;c<M;c++){
        // w[i] = rgt[i][c]
        // For each pair (r2,r3): middle block rows [r2,r3), need r2>=1,r3<=N-1
        // Precompute for upper: best_top[r2] = max over r1<r2, width w1>w_mid, area w1*(r2-r1)
        // But w_mid depends on (r2,r3)...
        // Let's try: for each r2, r3 (O(N^2)), compute w_mid, then find best upper ending before r2 and best lower starting after r3
        // For upper ending before r2: best area with width > w_mid
        // Precompute for each r2: array of (width, area) for all possible top blocks ending at r2
        // top_best[r2][w] = max area of top block ending at row r2 with width >= w+1
        // That's O(N*M) storage and O(N^2) computation per column
        
        // Actually: for fixed c and r2, the possible top blocks are [r1,r2) with width=min(rgt[r1..r2-1][c])
        // Let me precompute for each r2: a sorted list or just max_area_with_width_gt[r2][w]
        // top_maxarea[r2] is array indexed by minimum width required (> w_mid means >= w_mid+1)
        // Let's store top_by_width[r2][w] = max area of rectangle ending at r2 with width exactly w
        // Then prefix max from right: top_ge[r2][w] = max area with width >= w
        
        // For each r2 in [1,N], compute widths and areas of all top blocks ending at r2
        // top block [r1, r2) has width = min_{r1<=r<r2} rgt[r][c]
        // As r1 decreases from r2-1 to 0, width is non-increasing
        
        vector<vector<pair<int,long long>>> top_blocks(N+1); // top_blocks[r2] = list of (width, area)
        // For each r2, iterate r1 from r2-1 down to 0
        // Actually let me just do brute force for small N
        
        // top_ge[r2][w] = max area of top rectangle (left col c, ending row r2, width >= w)
        vector<vector<long long>> top_ge(N+1, vector<long long>(M+2, 0));
        for(int r2=1;r2<=N;r2++){
            int minw=M+1;
            for(int r1=r2-1;r1>=0;r1--){
                minw=min(minw,rgt[r1][c]);
                if(minw==0) break;
                long long area=(long long)minw*(r2-r1);
                if(area>top_ge[r2][minw]) top_ge[r2][minw]=area;
            }
            // now make top_ge[r2] suffix max (max for width >= w)
            for(int w=M;w>=1;w--) top_ge[r2][w]=max(top_ge[r2][w],top_ge[r2][w+1]);
        }
        // similarly bot_ge[r3][w] = max area of bottom rectangle starting at row r3, width >= w
        vector<vector<long long>> bot_ge(N+1, vector<long long>(M+2, 0));
        for(int r3=0;r3<N;r3++){
            int minw=M+1;
            for(int r4=r3;r4<N;r4++){
                minw=min(minw,rgt[r4][c]);
                if(minw==0) break;
                long long area=(long long)minw*(r4-r3+1);
                if(area>bot_ge[r3][minw]) bot_ge[r3][minw]=area;
            }
            for(int w=M;w>=1;w--) bot_ge[r3][w]=max(bot_ge[r3][w],bot_ge[r3][w+1]);
        }
        
        // Now for each middle block [r2, r3) with r2>=1, r3<=N-1, r3>r2
        // Need top block ending at r2 with width > w_mid, and bot block starting at r3 with width > w_mid
        // Also store best config
        
        // But we also need to reconstruct... let me first find best area, then reconstruct
        // For reconstruction, store parameters
        
        // Also need top_best_config and bot_best_config
        // Let me also store: for each (r2, min_width): which r1 gives best area
        // This gets complex. Let me first find best area.
        
        for(int r2=1;r2<N;r2++){
            int minw_mid=M+1;
            for(int r3=r2+1;r3<N;r3++){
                minw_mid=min(minw_mid,rgt[r3-1][c]);
                if(minw_mid==0) break;
                long long area_mid=(long long)minw_mid*(r3-r2);
                // need top with width > minw_mid and bot with width > minw_mid
                long long ta=top_ge[r2][minw_mid+1];
                long long ba=bot_ge[r3][minw_mid+1];
                if(ta>0 && ba>0){
                    long long total=ta+area_mid+ba;
                    if(total>best){
                        best=total;
                        bc=c; 
                        // find actual params - need more info
                        bh2=r3-r2; bw2=minw_mid;
                        // store r2,r3 for reconstruction
                        br1=r2; // reuse as r2 for now
                        bh3=r3; // reuse as r3
                    }
                }
            }
        }
    }
    
    if(best==-1){printf("-1\n");return 0;}
    
    // Reconstruct - need to find actual blocks
    // Redo for best (bc, br1=r2_mid, bh3=r3_mid, bw2=w_mid)
    int c=bc, r2=br1, r3=bh3, w_mid=bw2;
    // find top block ending at r2 with width >= w_mid+1 and max area
    long long best_top_area=0;
    int best_r1=-1, best_w1=-1;
    {
        int minw=M+1;
        for(int r1=r2-1;r1>=0;r1--){
            minw=min(minw,rgt[r1][c]);
            if(minw==0) break;
            if(minw>w_mid){
                long long area=(long long)minw*(r2-r1);
                if(area>best_top_area){
                    best_top_area=area;
                    best_r1=r1; best_w1=minw;
                }
            }
        }
    }
    long long best_bot_area=0;
    int best_r4=-1, best_w3=-1;
    {
        int minw=M+1;
        for(int r4=r3;r4<N;r4++){
            minw=min(minw,rgt[r4][c]);
            if(minw==0) break;
            if(minw>w_mid){
                long long area=(long long)minw*(r4-r3+1);
                if(area>best_bot_area){
                    best_bot_area=area;
                    best_r4=r4; best_w3=minw;
                }
            }
        }
    }
    
    printf("%lld\n",best);
    // mark cells
    // top: rows [best_r1, r2), cols [c, c+best_w1)
    // mid: rows [r2, r3), cols [c, c+w_mid)
    // bot: rows [r3, best_r4+1), cols [c, c+best_w3)
    for(int i=0;i<N;i++){
        bool top_row=(i>=best_r1 && i<r2);
        bool mid_row=(i>=r2 && i<r3);
        bool bot_row=(i>=r3 && i<=best_r4);
        for(int j=0;j<M;j++){
            int val=grid[i][j];
            if(top_row && j>=c && j<c+best_w1) val=8;
            if(mid_row && j>=c && j<c+w_mid) val=8;
            if(bot_row && j>=c && j<c+best_w3) val=8;
            printf("%d",val);
            if(j<M-1) printf(" ");
        }
        printf("\n");
    }
    return 0;
}
