#include <bits/stdc++.h>
using long long = long long;
using namespace std;
int main(){
    int n,k;
    cin>>n>>k;
    // For each color (0=even sum, 1=odd sum), compute dp[j] = ways to place j non-attacking bishops
    // diagonals of type (i+j)=d, grouped by color
    auto solve = [&](int color) -> vector<long long> {
        vector<int> diag_len;
        for(int d=color; d<=2*(n-1); d+=2){
            int len=0;
            for(int i=0;i<n;i++){int j=d-i;if(j>=0&&j<n)len++;}
            if(len>0)diag_len.push_back(len);
        }
        int nd=diag_len.size();
        // dp[i][j] = ways using first i "/" diags, placed j bishops, set of used "\" diags
        // "\" diag index = i - j + (n-1), but we need to track which are used
        // Use bitmask for used "\" diagonals (at most 2n-1 <= 19)
        int nback = 2*n-1;
        // dp[mask] = ways to have this mask of used "\" diags, for each count
        // But mask can be 2^19... too large
        // Alternative: since diag_len[i] <= n <= 10, and nd <= 2n-1 <= 19
        // Actually use dp[j][mask] but mask too large
        // Better: process "/" diags and for each, decide placement
        // dp[j] just count, no need mask if bishops on same "/" can't conflict on "\"?
        // Two cells on same "/" diagonal: i1+j1=d, i2+j2=d => i1-j1 != i2-j2 (unless same cell)
        // So within one "/" diagonal, all cells have distinct "\" diagonals!
        // Two cells on DIFFERENT "/" diagonals can share "\" diagonal if i1-j1=i2-j2
        vector<vector<long long>> dp(nd+1, vector<long long>(k+1,0));
        dp[0][0]=1;
        for(int i=0;i<nd;i++){
            int len=diag_len[i];
            for(int j=0;j<=k;j++){
                if(!dp[i][j])continue;
                dp[i+1][j]+=dp[i][j];// place none
                if(j+1<=k) dp[i+1][j+1]+=dp[i][j]*len;// place one (len choices)
            }
        }
        // But this overcounts: two bishops on different "/" diags sharing "\" diag
        // Hmm this approach is wrong for the "\" conflicts
        return vector<long long>(dp[nd].begin(), dp[nd].end());
    };
    // Actually the standard trick: bishops on white and black are independent,
    // and within each color, "/" diagonals and "\" diagonals form a bipartite structure
    // where each cell is intersection. No two cells in same "/" share "\", so
    // choosing one per "/" diagonal never conflicts on "/". But can conflict on "\".
    // We need inclusion-exclusion or a smarter DP.
    // Let me just brute force for n<=10.
    int N=n*n;
    // precompute for each cell its / and \ diagonal
    vector<int> slash(N), bslash(N);
    for(int i=0;i<n;i++)for(int j=0;j<n;j++){slash[i*n+j]=i+j;bslash[i*n+j]=i-j+n-1;}
    // dp over cells with color separation
    // separate white and black cells
    vector<int> white_cells, black_cells;
    for(int i=0;i<n;i++)for(int j=0;j<n;j++){if((i+j)%2==0)white_cells.push_back(i*n+j);else black_cells.push_back(i*n+j);}
    // For each color, compute f[j] = ways to place j non-attacking bishops
    auto computeF = [&](vector<int>& cells) -> vector<long long> {
        int m = cells.size();
        vector<long long> f(m+1,0);
        f[0]=1;
        // DP: process "/" diagonals
        // group cells by "/" diagonal
        map<int,vector<int>> bySlash;
        for(int c:cells) bySlash[slash[c]].push_back(c);
        // dp[j][used_bslash_mask] -- too large
        // Use dp over "/" diags with bitmask of used "\" diags
        // n<=10, bslash values 0..2n-2, so 2n-1 <= 19 bits
        // 2^19 = 524288, times k=100, might be ok? k<=n^2=100
        // Actually let's do dp[mask] = number of ways, mask = set of used "\" diags
        // and track count separately
        // dp[j] won't work. Use dp[mask] where mask encodes used bslash diags
        // For n=10, 2n-1=19, 2^19=512K entries, feasible
        int nb = 2*n-1;
        vector<vector<long long>> dp(k+1, vector<long long>(1<<nb,0));
        dp[0][0]=1;
        for(auto& [d, dcells]: bySlash){
            // process this "/" diagonal
            // cells in dcells all have distinct bslash values
            // We can place 0 or 1 bishop from this diagonal
            // iterate in reverse to avoid using same diag twice
            for(int j=min(k,(int)dp.size()-1);j>=0;j--){
                for(int mask=0;mask<(1<<nb);mask++){
                    if(!dp[j][mask])continue;
                    // try placing one bishop from dcells
                    for(int c:dcells){
                        int bd=bslash[c];
                        if(!(mask&(1<<bd))){
                            if(j+1<=k) dp[j+1][mask|(1<<bd)]+=dp[j][mask];
                        }
                    }
                }
            }
        }
        vector<long long> res(k+1,0);
        for(int j=0;j<=k;j++) for(int mask=0;mask<(1<<nb);mask++) res[j]+=dp[j][mask];
        return res;
    };
    auto fw = computeF(white_cells);
    auto fb = computeF(black_cells);
    long long ans=0;
    for(int i=0;i<=k;i++) if(i<(int)fw.size()&&k-i<(int)fb.size()) ans+=fw[i]*fb[k-i];
    cout<<ans<<endl;
}
