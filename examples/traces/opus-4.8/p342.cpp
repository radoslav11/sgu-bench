#include <bits/stdc++.h>
using namespace std;

int main(){
    string A;
    long long B;
    cin >> A >> B;
    
    if(B == 1){
        // answer is A itself
        cout << A << "\n";
        return 0;
    }
    
    // get digits of A in base B (least significant first)
    vector<long long> digits;
    string cur = A;
    // remove possible issues
    while(!(cur.size()==1 && cur[0]=='0')){
        // divide cur by B, get remainder
        string next;
        long long rem = 0;
        for(char ch : cur){
            long long d = rem * 10 + (ch - '0');
            long long q = d / B;
            rem = d % B;
            if(!next.empty() || q != 0){
                next.push_back((char)('0' + q));
            }
        }
        if(next.empty()) next = "0";
        digits.push_back(rem);
        cur = next;
    }
    if(digits.empty()) digits.push_back(0);
    
    const long long INF = LLONG_MAX/4;
    long long dp0 = 0, dp1 = INF;
    
    for(long long d : digits){
        long long n0 = INF, n1 = INF;
        // from carry 0
        if(dp0 < INF){
            long long v = d + 0;
            // option A: cost v, carry 0
            n0 = min(n0, dp0 + v);
            // option B: cost B-v, carry 1
            n1 = min(n1, dp0 + (B - v));
        }
        // from carry 1
        if(dp1 < INF){
            long long v = d + 1;
            n0 = min(n0, dp1 + v);
            n1 = min(n1, dp1 + (B - v));
        }
        dp0 = n0;
        dp1 = n1;
    }
    
    long long ans = dp0;
    if(dp1 < INF) ans = min(ans, dp1 + 1);
    
    cout << ans << "\n";
    return 0;
}
