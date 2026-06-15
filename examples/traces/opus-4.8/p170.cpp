#include <bits/stdc++.h>
using namespace std;
int main(){
    string a,b;
    getline(cin,a);
    getline(cin,b);
    // trim possible trailing whitespace/CR
    while(!a.empty()&&(a.back()=='\r'||a.back()=='\n'||a.back()==' '))a.pop_back();
    while(!b.empty()&&(b.back()=='\r'||b.back()=='\n'||b.back()==' '))b.pop_back();
    if(a.size()!=b.size()){cout<<-1<<endl;return 0;}
    int n=a.size();
    int cntA=count(a.begin(),a.end(),'+');
    int cntB=count(b.begin(),b.end(),'+');
    if(cntA!=cntB){cout<<-1<<endl;return 0;}
    // For each '+', count number of '-' before it.
    vector<int> da, db;
    int c=0;
    for(int i=0;i<n;i++){ if(a[i]=='+') da.push_back(c); else c++; }
    c=0;
    for(int i=0;i<n;i++){ if(b[i]=='+') db.push_back(c); else c++; }
    long long ans=0;
    for(size_t i=0;i<da.size();i++) ans+=abs(da[i]-db[i]);
    cout<<ans<<endl;
    return 0;
}
