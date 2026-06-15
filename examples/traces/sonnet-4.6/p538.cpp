#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    getline(cin,s);
    int n=s.size(),ans=0;
    auto onlySpacesLetters=[&](int l,int r)->bool{
        for(int i=l;i<=r;i++) if(!isalpha(s[i])&&s[i]!=' ') return false;
        return true;
    };
    for(int i=0;i<n;i++){
        if(s[i]=='('){
            int j=i+1;
            while(j<n&&s[j]!='('&&s[j]!=')') j++;
            if(j<n&&s[j]==')'&&onlySpacesLetters(i+1,j-1)) {} else ans++;
        } else if(s[i]==')'){
            int j=i-1;
            while(j>=0&&s[j]!='('&&s[j]!=')') j--;
            if(j>=0&&s[j]=='('&&onlySpacesLetters(j+1,i-1)) {} else ans++;
        }
    }
    cout<<ans<<endl;
}
