#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> BigInt;
BigInt fromLL(long long v){if(v==0)return{0};BigInt r;while(v>0){r.push_back(v%1000000000LL);v/=1000000000LL;}return r;}
BigInt add(const BigInt&a,const BigInt&b){BigInt r;long long c=0;for(int i=0;i<(int)max(a.size(),b.size())||c;i++){long long s=c+(i<(int)a.size()?a[i]:0LL)+(i<(int)b.size()?b[i]:0LL);r.push_back(s%1000000000LL);c=s/1000000000LL;}return r;}
string toStr(const BigInt&a){string r=to_string(a.back());for(int i=(int)a.size()-2;i>=0;i--){string t=to_string(a[i]);r+=string(9-t.size(),'0')+t;}return r;}
int countOcc(const string&text,const string&pat){if(pat.empty())return 0;int cnt=0;size_t pos=0;while((pos=text.find(pat,pos))!=string::npos){cnt++;pos++;;}return cnt;}
int main(){
    int N;cin>>N;
    string S;cin>>S;
    int m=S.size();
    int keep=max(m-1,1);
    vector<string> pre(N+1),suf(N+1);
    vector<long long> len(N+1,0);
    vector<BigInt> cnt(N+1,fromLL(0));
    for(int k=1;k<=N;k++){
        int L;cin>>L;
        string cp="",cs="";
        long long cl=0;
        BigInt cc=fromLL(0);
        for(int i=0;i<L;i++){
            string tok;cin>>tok;
            string np,ns;long long nl;BigInt nc;
            if(tok=="a"||tok=="b"){np=tok;ns=tok;nl=1;nc=fromLL(0);}
            else{int idx=stoi(tok);np=pre[idx];ns=suf[idx];nl=len[idx];nc=cnt[idx];}
            string junc=cs+np;
            int jc=countOcc(junc,S);
            cc=add(add(cc,nc),fromLL(jc));
            string newpre=cp;
            if((int)newpre.size()<keep){
                newpre=newpre+np;
                if((int)newpre.size()>keep)newpre=newpre.substr(0,keep);
            }
            string newsuf=cs+ns;
            if((int)newsuf.size()>keep)newsuf=newsuf.substr(newsuf.size()-keep);
            cp=newpre;cs=newsuf;cl=min((long long)2e15,cl+nl);
        }
        pre[k]=cp;suf[k]=cs;len[k]=cl;cnt[k]=cc;
    }
    cout<<toStr(cnt[N])<<endl;
}
