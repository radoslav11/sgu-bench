#include <bits/stdc++.h>
using namespace std;
map<tuple<int,int,int>,int> memo;
int g(int len,int lc,int rc){
    if(len==0)return 0;
    auto key=make_tuple(len,lc,rc);
    auto it=memo.find(key);
    if(it!=memo.end())return it->second;
    set<int> s;
    for(int p=0;p<len;p++){
        for(int c=1;c<=2;c++){
            if(p==0&&lc==c)continue;
            if(p==len-1&&rc==c)continue;
            int left=g(p,lc,c);
            int right=g(len-1-p,c,rc);
            s.insert(left^right);
        }
    }
    int m=0;while(s.count(m))m++;
    memo[key]=m;return m;
}
int main(){
    int n;string str;
    cin>>n>>str;
    int total=0;
    int i=0;
    while(i<n){
        if(str[i]!='0'){i++;continue;}
        int j=i;while(j<n&&str[j]=='0')j++;
        int len=j-i;
        int lc=0,rc=0;
        if(i>0)lc=str[i-1]-'0';
        if(j<n)rc=str[j]-'0';
        total^=g(len,lc,rc);
        i=j;
    }
    cout<<(total?"FIRST":"SECOND")<<endl;
}
