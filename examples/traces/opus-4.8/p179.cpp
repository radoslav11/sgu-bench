#include <bits/stdc++.h>
using namespace std;
int main(){
    string s; getline(cin,s);
    while(!s.empty() && (s.back()=='\n'||s.back()=='\r'||s.back()==' ')) s.pop_back();
    int n=s.size();
    vector<int> pre(n+1,0);
    for(int i=0;i<n;i++) pre[i+1]=pre[i]+(s[i]=='('?1:-1);
    for(int i=n-1;i>=0;i--){
        if(s[i]=='('){
            int newBal=pre[i]-1;
            if(newBal<0) continue;
            int rem=n-i-1;
            if((rem-newBal)<0 || ((rem-newBal)&1)) continue;
            int a=(rem-newBal)/2, b=rem-a;
            string res=s.substr(0,i);
            res+=')'; res+=string(a,'('); res+=string(b,')');
            cout<<res<<"\n";
            return 0;
        }
    }
    cout<<"No solution\n";
}
