#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    scanf("%d",&n);
    string s;
    s.resize(n);
    scanf("%s",&s[0]);
    n=s.size();
    
    for(int L=1;L<=20;L++){
        if((long long)(1<<L) > (long long)n+1) {
            // there are more strings than possible substrings, definitely some missing
        }
        // collect all substrings of length L
        unordered_set<int> seen;
        seen.reserve(1<<(L+1));
        int mask=(1<<L)-1;
        int cur=0;
        for(int i=0;i<n;i++){
            cur=((cur<<1)|(s[i]=='b'?1:0))&mask;
            if(i>=L-1) seen.insert(cur);
        }
        // find missing
        for(int v=0;v<(1<<L);v++){
            if(!seen.count(v)){
                string res;
                for(int b=L-1;b>=0;b--){
                    res+=((v>>b)&1)?'b':'a';
                }
                printf("%d\n%s\n",L,res.c_str());
                return 0;
            }
        }
    }
    return 0;
}
