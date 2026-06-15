#include <bits/stdc++.h>
using namespace std;

// big numbers as strings
int cmp(const string&a,const string&b){
    if(a.size()!=b.size()) return a.size()<b.size()?-1:1;
    if(a<b) return -1;
    if(a>b) return 1;
    return 0;
}
string add(const string&a,const string&b){
    string res;
    int i=a.size()-1,j=b.size()-1,carry=0;
    while(i>=0||j>=0||carry){
        int s=carry;
        if(i>=0) s+=a[i--]-'0';
        if(j>=0) s+=b[j--]-'0';
        res.push_back('0'+s%10);
        carry=s/10;
    }
    reverse(res.begin(),res.end());
    return res;
}
string norm(string s){
    int i=0;
    while(i+1<(int)s.size()&&s[i]=='0')i++;
    return s.substr(i);
}

int main(){
    int n;
    scanf("%d",&n);
    vector<string> v(n);
    for(auto&s:v){cin>>s; s=norm(s);}
    sort(v.begin(),v.end(),[](const string&a,const string&b){
        return cmp(a,b)<0;
    });
    for(int i=0;i+2<n;i++){
        // v[i] <= v[i+1] <= v[i+2], check v[i]+v[i+1] > v[i+2]
        string s=add(v[i],v[i+1]);
        if(cmp(s,v[i+2])>0){
            cout<<v[i+2]<<" "<<v[i]<<" "<<v[i+1]<<"\n";
            return 0;
        }
    }
    cout<<"0 0 0\n";
    return 0;
}
