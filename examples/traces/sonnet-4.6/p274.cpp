#include <bits/stdc++.h>
using namespace std;
bool isLetter(char c){return isalpha((unsigned char)c);}
bool isSymbol(char c){return isalpha((unsigned char)c)||isdigit((unsigned char)c)||c=='_'||c=='-';}
bool isWord(const string&s){if(s.empty())return false;for(char c:s)if(!isSymbol(c))return false;return true;}
bool isPrefix(const string&s){if(s.empty()||s.front()=='.'||s.back()=='.')return false;stringstream ss(s);string t;while(getline(ss,t,'.'))if(!isWord(t))return false;return true;}
bool isDomain(const string&s){if(s.size()!=2&&s.size()!=3)return false;for(char c:s)if(!isLetter(c))return false;return true;}
bool isSuffix(const string&s){size_t p=s.rfind('.');if(p==string::npos)return false;return isPrefix(s.substr(0,p))&&isDomain(s.substr(p+1));}
int main(){int n;cin>>n;cin.ignore();while(n--){string l;getline(cin,l);size_t a=l.find('@');if(a==string::npos||l.find('@',a+1)!=string::npos){cout<<"NO\n";continue;}string pre=l.substr(0,a),suf=l.substr(a+1);cout<<(isPrefix(pre)&&isSuffix(suf)?"YES":"NO")<<"\n";}}</p>
