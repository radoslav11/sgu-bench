#include <bits/stdc++.h>
using namespace std;
int main(){
    int N;
    cin>>N;
    if(N==0){cout<<fixed<<setprecision(3)<<0.0;return 0;}
    double best=0;
    for(int k=1;k<=N;k++){
        int m=N-k;
        double M=k;
        double D=2.0+m;
        best=max(best,M*D);
    }
    cout<<fixed<<setprecision(3)<<best<<endl;
}
