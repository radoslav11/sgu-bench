#include <bits/stdc++.h>
using namespace std;

static bitset<10000001> notSelf;

int main(){
    int N, K;
    scanf("%d %d", &N, &K);
    vector<int> s(K);
    for(int i=0;i<K;i++) scanf("%d", &s[i]);
    
    for(int i=1;i<=N;i++){
        int sum=i, x=i;
        while(x){ sum+=x%10; x/=10; }
        if(sum<=N) notSelf[sum]=1;
    }
    
    vector<pair<int,int>> q(K);
    for(int i=0;i<K;i++) q[i]={s[i], i};
    sort(q.begin(), q.end());
    
    vector<int> ans(K);
    long long count=0;
    int qi=0;
    for(int i=1;i<=N;i++){
        if(!notSelf[i]){
            count++;
            while(qi<K && q[qi].first==count){
                ans[q[qi].second]=i;
                qi++;
            }
        }
    }
    
    printf("%lld\n", count);
    for(int i=0;i<K;i++){
        printf("%d", ans[i]);
        if(i+1<K) printf(" ");
    }
    printf("\n");
    return 0;
}
