#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, K;
    scanf("%d %d", &N, &K);
    vector<long long> W(N), R(N), P(N);
    for(int i=0;i<N;i++){
        scanf("%lld %lld %lld", &W[i], &R[i], &P[i]);
    }
    vector<long long> A(K), B(N);
    for(int i=0;i<K;i++) scanf("%lld",&A[i]);
    for(int j=0;j<N;j++) scanf("%lld",&B[j]);
    
    const long long BASE = 20000000LL;
    
    vector<long long> tm(N);
    for(int i=0;i<N;i++) tm[i] = BASE - W[i];
    
    vector<int> order(N);
    vector<long long> horses(N);
    
    for(int st=0; st<K; st++){
        long long Ai = A[st];
        for(int j=0;j<N;j++){
            long long Bj = B[j];
            horses[j] = 3*Ai*Ai + 5*Ai*Bj + 2*Bj*Bj;
        }
        sort(horses.begin(), horses.end(), greater<long long>());
        
        for(int i=0;i<N;i++) order[i]=i;
        sort(order.begin(), order.end(), [&](int a, int b){
            if(tm[a]!=tm[b]) return tm[a]<tm[b];
            return P[a]>P[b];
        });
        
        for(int k=0;k<N;k++){
            int i = order[k];
            tm[i] += BASE - R[i] - horses[k];
        }
    }
    
    for(int i=0;i<N;i++) order[i]=i;
    sort(order.begin(), order.end(), [&](int a, int b){
        if(tm[a]!=tm[b]) return tm[a]<tm[b];
        return P[a]>P[b];
    });
    
    for(int i=0;i<N;i++){
        printf("%d", order[i]+1);
        if(i+1<N) printf(" ");
    }
    printf("\n");
    return 0;
}
