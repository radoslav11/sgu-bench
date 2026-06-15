#include <bits/stdc++.h>
using namespace std;
int main(){
    string f,s; getline(cin,f); getline(cin,s);
    char cur=f[0]; bool ok=!s.empty();
    for(size_t i=0;i<s.size()&&ok;i++){
        char comp=(cur=='A')?'B':'A';
        if(s[i]==cur){ if(i==s.size()-1)ok=false; }
        else if(s[i]==comp){ cur=comp; }
        else ok=false;
    }
    cout<<(ok?"YES":"NO")<<endl;
}
