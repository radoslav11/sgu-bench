#include<bits/stdc++.h>
using namespace std;
// Find shortest palindrome containing all words
// Expand to include reverses, remove substrings, find shortest palindromic superstring

int overlap(const string&a,const string&b){
    int n=a.size(),m=b.size();
    int mx=0;
    for(int i=1;i<=min(n,m);i++)
        if(a.substr(n-i)==b.substr(0,i))mx=i;
    return mx;
}

int main(){
    int n;cin>>n;
    vector<string>words(n);
    for(auto&w:words)cin>>w;
    
    // Add reverses
    vector<string>all=words;
    for(auto&w:words){string r(w.rbegin(),w.rend());all.push_back(r);}
    
    // Remove duplicates and substrings
    vector<string>filtered;
    sort(all.begin(),all.end(),[](auto&a,auto&b){return a.size()>b.size();});
    for(auto&w:all){
        bool sub=false;
        for(auto&f:filtered)if(f.find(w)!=string::npos){sub=true;break;}
        if(!sub)filtered.push_back(w);
    }
    
    int m=filtered.size();
    // precompute overlaps
    vector<vector<int>>ov(m,vector<int>(m,0));
    for(int i=0;i<m;i++)for(int j=0;j<m;j++)if(i!=j)ov[i][j]=overlap(filtered[i],filtered[j]);
    
    // TSP DP for shortest superstring
    vector<vector<int>>dp(1<<m,vector<int>(m,INT_MAX/2));
    vector<vector<int>>par(1<<m,vector<int>(m,-1));
    for(int i=0;i<m;i++)dp[1<<i][i]=filtered[i].size();
    
    for(int mask=1;mask<(1<<m);mask++){
        for(int i=0;i<m;i++){
            if(!(mask&(1<<i)))continue;
            if(dp[mask][i]==INT_MAX/2)continue;
            for(int j=0;j<m;j++){
                if(mask&(1<<j))continue;
                int nm=mask|(1<<j);
                int nc=dp[mask][i]+(int)filtered[j].size()-ov[i][j];
                if(nc<dp[nm][j]){dp[nm][j]=nc;par[nm][j]={i*100000+mask};}
            }
        }
    }
    
    int full=(1<<m)-1;
    int best=INT_MAX,last=-1;
    for(int i=0;i<m;i++)if(dp[full][i]<best){best=dp[full][i];last=i;}
    
    // Reconstruct
    vector<int>order;
    int mask=full,cur=last;
    while(mask){
        order.push_back(cur);
        int p=par[mask][cur];
        if(p==-1)break;
        int pm=p%100000,pi=p/100000;
        mask=pm;cur=pi;
    }
    reverse(order.begin(),order.end());
    
    string super=filtered[order[0]];
    for(int i=1;i<(int)order.size();i++){
        int o=ov[order[i-1]][order[i]];
        super+=filtered[order[i]].substr(o);
    }
    
    // Make palindrome from super
    // Try: find shortest palindrome containing super as substring
    // palindrome = rev(super)[k:] + super + super_suffix_that_is_palindrome... complex
    // Simpler: super + reverse chars needed
    // Find longest suffix of super that is palindrome, prepend reverse of prefix
    string rev_super(super.rbegin(),super.rend());
    
    // Check if super already is palindrome
    if(super==rev_super){cout<<super;return 0;}
    
    // Find shortest palindrome containing super
    // = find longest prefix of super that is suffix of rev_super (overlap)
    // palindrome = rev_super[0..n-1-k] + super where k = overlap of rev_super suffix with super prefix
    // OR find overlap where rev_super ends with super's prefix
    int best_pal=super.size()*2;
    string best_str=rev_super+super;
    // try overlap
    string t=super+"#"+rev_super;
    int sz=t.size();
    vector<int>kmp(sz,0);
    for(int i=1;i<sz;i++){
        int j=kmp[i-1];
        while(j&&t[i]!=t[j])j=kmp[j-1];
        if(t[i]==t[j])j++;
        kmp[i]=j;
    }
    int ov2=kmp[sz-1];
    string cand1=rev_super+super.substr(ov2);
    
    // also try super + rev_super with overlap
    string t2=rev_super+"#"+super;
    vector<int>kmp2(t2.size(),0);
    for(int i=1;i<(int)t2.size();i++){
        int j=kmp2[i-1];
        while(j&&t2[i]!=t2[j])j=kmp2[j-1];
        if(t2[i]==t2[j])j++;
        kmp2[i]=j;
    }
    int ov3=kmp2[t2.size()-1];
    string cand2=super+rev_super.substr(ov3);
    
    string ans=(cand1.size()<=cand2.size())?cand1:cand2;
    cout<<ans;
}
