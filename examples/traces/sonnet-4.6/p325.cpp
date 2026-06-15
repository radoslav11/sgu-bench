#include<bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin>>s;
    int n=s.size();
    int freq[26]={};
    for(char c:s) freq[c-'A']++;
    int odd=0;
    for(int i=0;i<26;i++) if(freq[i]%2) odd++;
    if(n%2==0 && odd>0){cout<<-1;return 0;}
    if(n%2==1 && odd!=1){cout<<-1;return 0;}
    long long ans=0;
    int l=0,r=n-1;
    while(l<r){
        if(s[l]==s[r]){l++;r--;}
        else{
            int j=r-1;
            while(j>l && s[j]!=s[l]) j--;
            if(j==l){
                swap(s[l],s[l+1]);
                ans++;
            } else {
                while(j<r){swap(s[j],s[j+1]);j++;ans++;}
                l++;r--;
            }
        }
    }
    cout<<ans;
}
