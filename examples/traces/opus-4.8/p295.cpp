#include <bits/stdc++.h>
using namespace std;

const uint32_t BASE = 1000000000u;

struct Big {
    vector<uint32_t> d; // little endian base 1e9
    Big(){ d.push_back(0); }
    Big(uint64_t v){ if(v==0) d.push_back(0); else { while(v){ d.push_back((uint32_t)(v%BASE)); v/=BASE; } } }
    void trim(){ while(d.size()>1 && d.back()==0) d.pop_back(); }
};

Big addBig(const Big&a, const Big&b){
    Big r; r.d.clear();
    uint64_t carry=0;
    size_t n=max(a.d.size(), b.d.size());
    for(size_t i=0;i<n||carry;i++){
        uint64_t cur=carry;
        if(i<a.d.size()) cur+=a.d[i];
        if(i<b.d.size()) cur+=b.d[i];
        r.d.push_back((uint32_t)(cur%BASE));
        carry=cur/BASE;
    }
    if(r.d.empty()) r.d.push_back(0);
    r.trim();
    return r;
}

void mulSmall(Big&a, uint32_t m){
    uint64_t carry=0;
    for(size_t i=0;i<a.d.size();i++){
        uint64_t cur=(uint64_t)a.d[i]*m + carry;
        a.d[i]=(uint32_t)(cur%BASE);
        carry=cur/BASE;
    }
    while(carry){
        a.d.push_back((uint32_t)(carry%BASE));
        carry/=BASE;
    }
    a.trim();
}

void subOne(Big&a){
    // a >= 1 assumed
    int i=0;
    while(a.d[i]==0){ a.d[i]=BASE-1; i++; }
    a.d[i]--;
    a.trim();
}

string toStr(const Big&a){
    string s = to_string(a.d.back());
    for(int i=(int)a.d.size()-2;i>=0;i--){
        string t = to_string(a.d[i]);
        s += string(9-t.size(),'0') + t;
    }
    return s;
}

int main(){
    int q,c;
    cin>>q>>c;
    cin.ignore();
    string line;
    getline(cin, line);
    
    int origLen = (int)line.size();
    
    string inter = "ABCEHKMOPTXaceopxy";
    set<char> iset(inter.begin(), inter.end());
    
    int L=0, K=0, W=0;
    bool inWord=false;
    for(char ch: line){
        if(ch==' '){
            inWord=false;
        } else {
            if(!inWord){ W++; inWord=true; }
            L++;
            if(iset.count(ch)) K++;
        }
    }
    
    // precompute binomial up to 65
    int N=65;
    vector<vector<Big>> C(N, vector<Big>(N, Big((uint64_t)0)));
    for(int n=0;n<N;n++){
        C[n][0]=Big((uint64_t)1);
        for(int k=1;k<=n;k++){
            C[n][k]=addBig(C[n-1][k-1], C[n-1][k]);
        }
    }
    
    Big sum((uint64_t)0);
    for(int T=q;T<=c;T++){
        int S=T-L;
        if(S<0) continue;
        if(S < W-1) continue;
        int n=S+1;
        int k=W;
        if(n>=N) continue; // shouldn't happen given constraints
        if(k>n) continue;
        sum = addBig(sum, C[n][k]);
    }
    
    for(int i=0;i<K;i++) mulSmall(sum,2);
    
    // subtract original if its length within range
    bool origIn = (origLen>=q && origLen<=c);
    if(origIn){
        // sum should be >= 1
        // check if sum is zero
        bool isZero = (sum.d.size()==1 && sum.d[0]==0);
        if(!isZero) subOne(sum);
    }
    
    cout<<toStr(sum)<<"\n";
    return 0;
}
