#include <bits/stdc++.h>
using namespace std;
int main(){
    int q,c;
    cin>>q>>c;
    cin.ignore();
    string line;
    getline(cin,line);
    string ambig="ABCEHKMOPTXaceopxy";
    set<char> amb(ambig.begin(),ambig.end());
    // extract words
    vector<string> words;
    stringstream ss(line);
    string w;
    while(ss>>w) words.push_back(w);
    int n=words.size();
    long long letter_variants=1;
    int S=0;
    for(auto&wd:words){
        int cnt=0;
        for(char ch:wd) if(amb.count(ch)) cnt++;
        S+=wd.size();
        letter_variants*=(1LL<<cnt);
    }
    // spacing variants: total length in [q,c], spaces_total >= max(0,n-1)
    // ways for given spaces_total = C(spaces_total+1, n) [n gaps including leading/trailing, n-1 internal each>=1]
    // actually: C(spaces_total - (n-1) + n, n) = C(spaces_total+1, n)
    auto C=[](long long a, long long b)->long long{
        if(b<0||b>a) return 0;
        long long r=1;
        for(long long i=0;i<b;i++) r=r*(a-i)/(i+1);
        return r;
    };
    long long space_variants=0;
    for(int L=q;L<=c;L++){
        int sp=L-S;
        if(sp<0) continue;
        if(n>1&&sp<n-1) continue;
        space_variants+=C(sp+1,n);
    }
    cout<<letter_variants*space_variants-1<<endl;
}
