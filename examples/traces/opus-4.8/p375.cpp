#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N;
    cin>>N;
    if(N==1){cout<<0<<"\n"<<"\n";return 0;}
    if(N%2==0){cout<<"No solution\n";return 0;}
    vector<int> seq;
    long long x=N;
    while(x>1){
        if(x%2==0){cout<<"No solution\n";return 0;}
        long long a=(x+1)/2;
        long long b=(x-1)/2;
        if(a%2==1){seq.push_back(1);x=a;}
        else {seq.push_back(2);x=b;}
        if(x==0){cout<<"No solution\n";return 0;}
    }
    reverse(seq.begin(),seq.end());
    cout<<seq.size()<<"\n";
    for(int i=0;i<(int)seq.size();i++){
        cout<<seq[i];
        if(i+1<(int)seq.size())cout<<" ";
    }
    cout<<"\n";
    return 0;
}
