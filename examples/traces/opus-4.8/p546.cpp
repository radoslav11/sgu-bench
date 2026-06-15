#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,a,b;
    cin>>n>>a>>b;
    string s; cin>>s;
    int c=n-a-b;
    if(c<0){ cout<<-1<<"\n"; return 0; }
    int need[3]={a,b,c};
    int keep[3]={0,0,0};
    vector<bool> kept(n,false);
    for(int i=0;i<n;i++){
        int d=s[i]-'0';
        if(keep[d]<need[d]){ keep[d]++; kept[i]=true; }
    }
    int rem[3];
    for(int d=0;d<3;d++) rem[d]=need[d]-keep[d];
    int t=0;
    int cur=0;
    for(int i=0;i<n;i++){
        if(!kept[i]){
            t++;
            while(cur<3 && rem[cur]==0) cur++;
            s[i]='0'+cur;
            rem[cur]--;
        }
    }
    cout<<t<<"\n"<<s<<"\n";
    return 0;
}
