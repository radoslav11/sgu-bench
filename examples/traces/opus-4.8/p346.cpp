#include <bits/stdc++.h>
using namespace std;
int main(){
    int c[7];
    for(int i=0;i<7;i++) cin>>c[i];
    string s; cin>>s;
    int R=c[0];
    int colourSum=0, maxColour=0;
    for(int i=1;i<7;i++){
        if(c[i]){ colourSum += (i+1); maxColour=max(maxColour,i+1);}
    }
    long long ans=0;
    if(R>0){
        ans = (long long)R*(1+7) + 27;
        if(s=="RED") ans += 7;
    } else {
        ans = colourSum;
        if(s=="RED") ans += maxColour;
    }
    cout<<ans<<endl;
}
