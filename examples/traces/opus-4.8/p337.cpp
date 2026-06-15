#include <bits/stdc++.h>
using namespace std;

int main(){
    int K;
    string s;
    // read K
    if(!(cin>>K)) return 0;
    cin>>s;
    int n = s.size();
    string s2 = s + s;
    int N = 2*n;
    
    int Lmax = (n-1)/2; // max half length, total length 2L <= n-1
    
    for(int L = Lmax; L >= 1; --L){
        // mismatch array m[p] = s2[p]!=s2[p+L]
        // window sum over [i, i+L-1] for i in 0..n-1
        vector<int> pre(N+1, 0);
        for(int p=0;p<N;p++){
            int val = 0;
            if(p+L < N){
                val = (s2[p]!=s2[p+L])?1:0;
            }
            pre[p+1] = pre[p] + val;
        }
        int best = -1;
        for(int i=0;i<n;i++){
            int sum = pre[i+L] - pre[i];
            if(sum <= K){
                if(best == -1){
                    best = i;
                } else {
                    // compare substring of length 2L starting at i vs best
                    int len = 2*L;
                    int cmp = 0;
                    for(int j=0;j<len;j++){
                        char a = s2[i+j];
                        char b = s2[best+j];
                        if(a<b){cmp=-1;break;}
                        if(a>b){cmp=1;break;}
                    }
                    if(cmp<0) best = i;
                }
            }
        }
        if(best != -1){
            // output substring
            int len = 2*L;
            cout << s2.substr(best, len) << "\n";
            return 0;
        }
    }
    // no valid substring
    cout << "\n";
    return 0;
}
