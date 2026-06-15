#include <bits/stdc++.h>
using namespace std;

typedef vector<uint32_t> Big; // base 1e9, little endian
const uint32_t BASE=1000000000u;

void mulSmall(Big&a, uint64_t m){
    uint64_t carry=0;
    for(size_t i=0;i<a.size();i++){
        uint64_t cur=(uint64_t)a[i]*m+carry;
        a[i]=cur%BASE; carry=cur/BASE;
    }
    while(carry){a.push_back(carry%BASE); carry/=BASE;}
    if(a.empty())a.push_back(0);
}
void addBig(Big&a,const Big&b){
    uint64_t carry=0;
    for(size_t i=0;i<b.size()||carry;i++){
        if(i==a.size())a.push_back(0);
        uint64_t cur=(uint64_t)a[i]+(i<b.size()?b[i]:0)+carry;
        a[i]=cur%BASE; carry=cur/BASE;
    }
}
// divide by small, return remainder
uint64_t divSmall(Big&a, uint64_t d){
    uint64_t rem=0;
    for(int i=a.size()-1;i>=0;i--){
        uint64_t cur=rem*BASE+a[i];
        a[i]=cur/d; rem=cur%d;
    }
    while(a.size()>1&&a.back()==0)a.pop_back();
    return rem;
}

int main(){
    int n; 
    if(!(cin>>n))return 0;
    // Sum over j=0..n-3 of (n!/j!) * n^j, then divide by n, then by 2.
    // Compute term iteratively. term_j = n!/j! * n^j.
    // term_0 = n!
    // term_{j} = term_{j-1} * n / j  (since n!/j! = n!/(j-1)! / j, and n^j = n^{j-1}*n)
    // term_j/term_{j-1} = (1/j)*n... wait: (n!/j!)/(n!/(j-1)!) = (j-1)!/j! = 1/j. times n (for n^j/n^{j-1}).
    // so term_j = term_{j-1} * n / j. But division must be exact? n!/j! is integer, times n^j integer.
    // term_j = n!/j! * n^j. ratio = n/j. Not necessarily exact individually.
    // Instead compute term as separate: keep numerator factorial part and power separately is messy.
    // Better: compute n!/j! directly = product, times n^j.
    // Let's just compute each term from scratch incrementally maintaining big integer for n!/j! and n^j.
    
    // fact_ratio for j: F_j = n!/j!. F_0=n!, F_j = F_{j-1}/j  ... not exact.
    // Reverse: F_{n-3} = n!/(n-3)! = n*(n-1)*(n-2). build up: start small.
    // Let's iterate j from high to low? Simpler: maintain F as n!/j! by starting F=1 at j=n, multiply.
    // We need j from 0..n-3. F_j = product_{t=j+1}^{n} t.
    // Start at j=n-3: F = n*(n-1)*...*(j+1). 
    // Let me just iterate j decreasing from n-3 down to 0, maintaining F and P=n^j.
    
    Big sum; sum.push_back(0);
    // compute for each j: need F_j and n^j.
    // maintain F starting j=n-3 then decreasing j multiply by (j+1).
    // Initialize F_{n-3}= product_{t=n-2}^{n} t = n*(n-1)*(n-2)
    Big F; F.push_back(1);
    for(int t=n-2;t<=n;t++) mulSmall(F,(uint64_t)t);
    Big P; P.push_back(1); // n^j, j=n-3
    for(int e=0;e<n-3;e++) mulSmall(P,(uint64_t)n);
    
    for(int j=n-3;j>=0;j--){
        Big term=F;
        // multiply by P (n^j) -- P big times big needed; do big multiply
        // implement big*big
        {
            Big res(term.size()+P.size(),0);
            for(size_t i=0;i<term.size();i++){
                uint64_t carry=0;
                for(size_t k=0;k<P.size()||carry;k++){
                    uint64_t cur=res[i+k]+carry+(k<P.size()?(uint64_t)term[i]*P[k]:0);
                    res[i+k]=cur%BASE; carry=cur/BASE;
                }
            }
            while(res.size()>1&&res.back()==0)res.pop_back();
            term=res;
        }
        addBig(sum,term);
        if(j>0){
            mulSmall(F,(uint64_t)j); // F_{j-1}=F_j*(j)? F_j=n!/j!, F_{j-1}=n!/(j-1)!=F_j*j
            divSmall(P,(uint64_t)n); // n^{j-1}
        }
    }
    // sum currently = sum_{j} n!/j! * n^j. divide by n then by 2.
    divSmall(sum,(uint64_t)n);
    divSmall(sum,2ULL);
    
    // print
    string s=to_string(sum.back());
    for(int i=(int)sum.size()-2;i>=0;i--){
        string p=to_string(sum[i]);
        s+=string(9-p.size(),'0')+p;
    }
    cout<<s<<"\n";
    return 0;
}
