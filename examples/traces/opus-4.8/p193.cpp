#include <bits/stdc++.h>
using namespace std;

// big integer as string (decimal), nonneg
string add(const string&a,const string&b){
    string r; int i=a.size()-1,j=b.size()-1,c=0;
    while(i>=0||j>=0||c){
        int s=c;
        if(i>=0)s+=a[i--]-'0';
        if(j>=0)s+=b[j--]-'0';
        r+=char('0'+s%10); c=s/10;
    }
    reverse(r.begin(),r.end());
    return r;
}
// a-b, a>=b
string sub(const string&a,const string&b){
    string r; int i=a.size()-1,j=b.size()-1,borrow=0;
    while(i>=0){
        int s=(a[i]-'0')-borrow-(j>=0?b[j]-'0':0);
        if(s<0){s+=10;borrow=1;}else borrow=0;
        r+=char('0'+s); i--; j--;
    }
    while(r.size()>1&&r.back()=='0')r.pop_back();
    reverse(r.begin(),r.end());
    return r;
}
// divide by 2
string div2(const string&a){
    string r; int rem=0;
    for(char ch:a){
        int cur=rem*10+(ch-'0');
        r+=char('0'+cur/2); rem=cur%2;
    }
    int p=0; while(p+1<(int)r.size()&&r[p]=='0')p++;
    return r.substr(p);
}

int main(){
    string n; cin>>n;
    int lastdigit=n.back()-'0';
    int last2 = (n.size()>=2)?((n[n.size()-2]-'0')*10+lastdigit):lastdigit;
    string ans;
    if(lastdigit%2==1){
        // odd: (N-1)/2
        ans=div2(sub(n,"1"));
    } else {
        string half=div2(n);
        if(last2%4==0){
            ans=sub(half,"1");
        } else {
            ans=sub(half,"2");
        }
    }
    cout<<ans<<endl;
}
