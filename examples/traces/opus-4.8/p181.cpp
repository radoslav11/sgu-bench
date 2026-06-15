#include <bits/stdc++.h>
using namespace std;
int main(){
    long long A,alpha,beta,gamma,M,k;
    cin>>A>>alpha>>beta>>gamma>>M>>k;
    long long x=A%M;
    vector<long long> seen(M,-1);
    vector<long long> seq;
    long long i=0;
    while(i<k){
        if(seen[x]!=-1){
            long long start=seen[x];
            long long cycle=i-start;
            long long rem=(k-start)%cycle;
            cout<<seq[start+rem]<<endl;
            return 0;
        }
        seen[x]=i;
        seq.push_back(x);
        x=(alpha*x%M*x+beta*x+gamma)%M;
        i++;
    }
    cout<<x<<endl;
    return 0;
}
