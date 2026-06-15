#include <bits/stdc++.h>
using namespace std;

int main(){
    long long N, H;
    cin >> N >> H;
    
    long long p = (long long)sqrtl((long double)N);
    while(p*p > N) p--;
    while((p+1)*(p+1) <= N) p++;
    
    // num(x,H)
    auto num = [&](long long x, long long y)->long long{
        if(x==0) return (y+1)*(y+1);
        if(x<0){
            long long k = y - x + 1;
            return (k-1)*(k-1) + 1 + y;
        } else {
            long long k = y + x + 1;
            return (k-1)*(k-1) + k + y;
        }
    };
    
    vector<pair<long long,long long>> res;
    long long lim = p + 2;
    for(long long x = -lim; x <= lim; x++){
        long long n = num(x, H);
        if(n >= 1 && n <= N){
            res.push_back({x, n});
        }
    }
    sort(res.begin(), res.end());
    
    bool first = true;
    for(auto &pr : res){
        if(!first) cout << ' ';
        cout << pr.second;
        first = false;
    }
    cout << "\n";
    return 0;
}
