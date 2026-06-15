#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    scanf("%d",&n);
    vector<int>a(n+2,0);
    for(int i=1;i<=n;i++) scanf("%d",&a[i]);
    int ans=0;
    for(int start=0;start<=1;start++){
        vector<int>m(n+2,0);
        m[1]=start;
        bool ok=true;
        for(int i=1;i<=n;i++){
            int next=a[i]-m[i-1]-m[i];
            if(next<0||next>1){ok=false;break;}
            m[i+1]=next;
        }
        if(ok && m[n+1]==0) ans++;
    }
    printf("%d\n",ans);
}
