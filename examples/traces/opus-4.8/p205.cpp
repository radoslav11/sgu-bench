#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; scanf("%d",&n);
    vector<int> x(n);
    for(int i=0;i<n;i++) scanf("%d",&x[i]);
    int m,s; scanf("%d %d",&m,&s);
    vector<vector<int>> L(m, vector<int>(s));
    for(int i=0;i<m;i++) for(int j=0;j<s;j++) scanf("%d",&L[i][j]);
    int mask = m-1;
    vector<long long> next_dp(m,0), cur_dp(m);
    vector<vector<int>> choice(n, vector<int>(m,0));
    for(int j=n-1;j>=0;j--){
        for(int set=0;set<m;set++){
            long long best=LLONG_MAX; int bk=0;
            for(int k=0;k<s;k++){
                long long c = llabs((long long)x[j]-L[set][k]) + next_dp[k&mask];
                if(c<best){best=c;bk=k;}
            }
            cur_dp[set]=best;
            choice[j][set]=bk;
        }
        swap(cur_dp,next_dp);
    }
    printf("%lld\n", next_dp[0]);
    int set=0;
    for(int j=0;j<n;j++){
        int k=choice[j][set];
        if(j) printf(" ");
        printf("%d",k);
        set = k&mask;
    }
    printf("\n");
    return 0;
}
