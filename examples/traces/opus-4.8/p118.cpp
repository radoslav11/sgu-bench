#include <bits/stdc++.h>
using namespace std;
int main(){
    int K;
    cin>>K;
    while(K--){
        int n;
        cin>>n;
        vector<long long> a(n);
        bool firstZero=false;
        for(int i=0;i<n;i++){
            cin>>a[i];
        }
        long long prod=1;
        long long total=0;
        bool anyNonzeroTerm=false;
        for(int i=0;i<n;i++){
            prod=(prod*(a[i]%9))%9;
            total=(total+prod)%9;
        }
        // determine if real sum is zero: sum is zero iff all terms zero iff A1==0
        bool sumZero = (a[0]==0);
        int dr;
        if(sumZero) dr=0;
        else { dr = (total%9==0)?9:(int)(total%9); }
        cout<<dr<<"\n";
    }
}
