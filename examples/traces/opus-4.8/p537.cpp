#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; scanf("%d",&n);
    for(int tc=1;tc<=n;tc++){
        char buf[20]; scanf("%s",buf);
        string s=buf;
        int L=s.size();
        map<char,int> idx;
        for(char c:s) if(!idx.count(c)){int t=idx.size(); idx[c]=t;}
        int k=idx.size();
        vector<long long> coef(k,0);
        long long p=1;
        for(int i=L-1;i>=0;i--){coef[idx[s[i]]]+=p; p*=10;}
        int firstLetter=idx[s[0]];
        mt19937 rng(12345);
        long long g=0;
        vector<int> digits={0,1,2,3,4,5,6,7,8,9};
        int iters=20000;
        for(int it=0;it<iters;it++){
            shuffle(digits.begin(),digits.end(),rng);
            if(digits[firstLetter]==0){
                for(int j=0;j<k;j++) if(digits[j]!=0){swap(digits[firstLetter],digits[j]);break;}
            }
            long long val=0;
            for(int i=0;i<k;i++) val+=coef[i]*digits[i];
            g=__gcd(g,val);
            if(g==1) break;
        }
        vector<long long> divs;
        if(g==0) g=1;
        for(long long d=1;d*d<=g;d++){
            if(g%d==0){divs.push_back(d); if(d!=g/d) divs.push_back(g/d);}
        }
        sort(divs.begin(),divs.end());
        printf("Case %d:",tc);
        for(long long d:divs) printf(" %lld",d);
        printf("\n");
    }
}
