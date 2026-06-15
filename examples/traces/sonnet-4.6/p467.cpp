#include<bits/stdc++.h>
using namespace std;
int main(){
    int ans[101]={0};
    for(int i=1;i<=78;i++) ans[i]=1;
    for(int i=79;i<=100;i++) ans[i]=2;
    int p,cas=1;
    while(cin>>p){
        cout<<"Case #"<<cas++<<": "<<ans[p]<<"\n";
    }
}
