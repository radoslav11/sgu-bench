#include <bits/stdc++.h>
using namespace std;

int shots(int P){
    if(P==0) return 0;
    if(P<=78) return 1;   // pi/4 ~ 78.5398%
    if(P<=95) return 2;   // ~95.661%
    if(P<=99) return 3;   // ~99.96%
    return 4;             // P==100
}

int main(){
    int P;
    int caseNo=1;
    while(cin>>P){
        cout<<"Case #"<<caseNo++<<": "<<shots(P)<<"\n";
    }
    return 0;
}
