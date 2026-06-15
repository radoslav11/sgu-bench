#include <bits/stdc++.h>
using namespace std;

int main(){
    int N,M;
    cin>>N>>M;
    string A,B;
    cin>>A>>B;
    string best=B;
    for(int i=0;i<M;i++){
        string s=B.substr(i)+B.substr(0,i);
        if(s<best) best=s;
    }
    B=best;
    
    auto matches=[&](int p)->bool{
        int diff=0;
        for(int j=0;j<M;j++){
            if(A[(p+j)%N]!=B[j]){
                diff++;
                if(diff>1) return false;
            }
        }
        return true;
    };
    
    int found=-1;
    for(int k=0;k<N && found<0;k++){
        int pl=k%N;
        if(matches(pl)){ found=pl; break;}
        if(k>0){
            int pr=((N-k)%N+N)%N;
            if(pr!=pl && matches(pr)){ found=pr; break;}
        }
    }
    
    string res;
    for(int j=0;j<N;j++) res+=A[(found+j)%N];
    cout<<B<<"\n"<<res<<"\n";
    return 0;
}
