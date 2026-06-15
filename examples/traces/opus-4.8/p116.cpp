#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    int LIM = N;
    vector<bool> isPrime(LIM+1, true);
    if(LIM>=0) isPrime[0]=false;
    if(LIM>=1) isPrime[1]=false;
    for(int i=2;i<=LIM;i++){
        if(isPrime[i]){
            for(int j=2*i;j<=LIM;j+=i) isPrime[j]=false;
        }
    }
    vector<int> primes;
    for(int i=2;i<=LIM;i++) if(isPrime[i]) primes.push_back(i);
    auto isPrimeNum = [](int x){
        if(x<2) return false;
        for(int d=2;(long long)d*d<=x;d++) if(x%d==0) return false;
        return true;
    };
    vector<int> superPrimes;
    for(int k=0;k<(int)primes.size();k++){
        int idx = k+1;
        if(isPrimeNum(idx)) superPrimes.push_back(primes[k]);
    }
    
    const int INF = INT_MAX/2;
    vector<int> dp(N+1, INF);
    vector<int> choice(N+1, -1);
    dp[0]=0;
    for(int v=1;v<=N;v++){
        for(int sp : superPrimes){
            if(sp<=v && dp[v-sp]+1 < dp[v]){
                dp[v]=dp[v-sp]+1;
                choice[v]=sp;
            }
        }
    }
    
    if(dp[N]>=INF){
        cout << 0 << "\n";
    } else {
        cout << dp[N] << "\n";
        vector<int> res;
        int cur=N;
        while(cur>0){
            res.push_back(choice[cur]);
            cur-=choice[cur];
        }
        sort(res.rbegin(), res.rend());
        for(int i=0;i<(int)res.size();i++){
            cout << res[i];
            if(i+1<(int)res.size()) cout << " ";
        }
        cout << "\n";
    }
    return 0;
}
