#include <bits/stdc++.h>
using namespace std;

const long long CAP = 1000;

struct Big {
    vector<long long> d; // base 1e9, little endian
    Big(){}
    Big(long long x){ if(x){ while(x){ d.push_back(x%1000000000LL); x/=1000000000LL; } } }
    void add(const Big&o){
        long long carry=0;
        size_t n=max(d.size(),o.d.size());
        d.resize(n,0);
        for(size_t i=0;i<n;i++){
            long long s=d[i]+carry+(i<o.d.size()?o.d[i]:0);
            d[i]=s%1000000000LL;
            carry=s/1000000000LL;
        }
        if(carry) d.push_back(carry);
    }
    string toString()const{
        if(d.empty()) return "0";
        string s=to_string(d.back());
        for(int i=(int)d.size()-2;i>=0;i--){
            string t=to_string(d[i]);
            s+=string(9-t.size(),'0')+t;
        }
        return s;
    }
};

int m;
string S;

struct Node {
    Big count;
    string pre, suf;
    long long len;
};

Node combine(const Node&A,const Node&B){
    Node R;
    // cross occurrences
    string comb = A.suf + B.pre;
    int boundary = (int)A.suf.size();
    long long cross=0;
    int L=(int)comb.size();
    for(int i=0;i+m<=L;i++){
        if(i<boundary && boundary<=i+m-1){
            bool ok=true;
            for(int j=0;j<m;j++){ if(comb[i+j]!=S[j]){ok=false;break;} }
            if(ok) cross++;
        }
    }
    R.count=A.count;
    R.count.add(B.count);
    R.count.add(Big(cross));
    R.len = min(CAP, A.len+B.len);
    long long want = (long long)(m-1);
    // prefix
    {
        string tmp;
        if(A.len >= want) tmp = A.pre;
        else { tmp = A.pre + B.pre; }
        long long take = min(want, R.len);
        if((long long)tmp.size()>take) tmp=tmp.substr(0,take);
        R.pre=tmp;
    }
    // suffix
    {
        string tmp;
        if(B.len >= want) tmp = B.suf;
        else { tmp = A.suf + B.suf; }
        long long take = min(want, R.len);
        if((long long)tmp.size()>take) tmp=tmp.substr(tmp.size()-take);
        R.suf=tmp;
    }
    return R;
}

Node baseTerminal(char c){
    Node R;
    R.len=1;
    long long want=m-1;
    string s(1,c);
    R.pre = (want>=1)? s : string("");
    R.suf = (want>=1)? s : string("");
    long long cnt=0;
    if(m==1 && S[0]==c) cnt=1;
    R.count=Big(cnt);
    return R;
}

int main(){
    int N;
    if(!(cin>>N)) return 0;
    cin>>S;
    m=(int)S.size();
    vector<Node> nodes(N+1);
    for(int k=1;k<=N;k++){
        int Lk; cin>>Lk;
        Node cur;
        cur.len=0; cur.pre=""; cur.suf=""; cur.count=Big(0);
        bool first=true;
        for(int i=0;i<Lk;i++){
            string tok; cin>>tok;
            Node part;
            if(tok=="a"||tok=="b"){
                part=baseTerminal(tok[0]);
            } else {
                int idx=stoi(tok);
                part=nodes[idx];
            }
            if(first){ cur=part; first=false; }
            else cur=combine(cur,part);
        }
        nodes[k]=cur;
    }
    cout<<nodes[N].count.toString()<<"\n";
    return 0;
}
