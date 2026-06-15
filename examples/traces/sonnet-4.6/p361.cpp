#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M;
    cin>>N>>M;
    int col[3]={0,2,1};
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            cout<<(j%3==col[i%3]?'#':'0');
        }
        cout<<'\n';
    }
}
