#include <bits/stdc++.h>
using namespace std;
typedef pair<char,bool> Lit;
typedef vector<Lit> Term;
typedef vector<Term> DNF;
string expr; int pos;
DNF andDNF(DNF a,DNF b){DNF r;for(auto&x:a)for(auto&y:b){Term t=x;for(auto&l:y)t.push_back(l);r.push_back(t);}return r;}
DNF orDNF(DNF a,DNF b){for(auto&x:b)a.push_back(x);return a;}
DNF negDNF(DNF d){DNF r={{}};for(auto&t:d){DNF tt;for(auto&l:t)tt.push_back({{l.first,!l.second}});r=andDNF(r,tt);}return r;}
DNF parse_low(),parse_mid(),parse_high(),parse_atom();
DNF parse_atom(){if(expr[pos]=='('){pos++;DNF r=parse_low();pos++;return r;}DNF r={{{expr[pos],false}}};pos++;return r;}
DNF parse_high(){if(expr[pos]=='!'){pos++;return negDNF(parse_high());}return parse_atom();}
DNF parse_mid(){DNF r=parse_high();while(pos<(int)expr.size()&&expr[pos]=='&'){pos++;r=andDNF(r,parse_high());}return r;}
DNF parse_low(){DNF r=parse_mid();while(pos<(int)expr.size()&&(expr[pos]=='|'||expr[pos]=='='||expr[pos]=='<'||expr[pos]=='#')){if(expr[pos]=='|'){pos+=2;r=orDNF(r,parse_mid());}else if(expr[pos]=='#'){pos++;DNF b=parse_mid();DNF nr=negDNF(r),nb=negDNF(b);r=orDNF(andDNF(r,nb),andDNF(nr,b));}else if(expr[pos]=='<'){pos+=3;DNF b=parse_mid();r=orDNF(andDNF(r,b),andDNF(negDNF(r),negDNF(b)));}else{pos+=2;DNF nr=negDNF(r);r=orDNF(nr,parse_mid());}}return r;}
int main(){cin>>expr;pos=0;DNF d=parse_low();string out;for(auto&t:d){if(!out.empty())out+="||";for(int i=0;i<(int)t.size();i++){if(i)out+="&";if(t[i].second)out+="!";out+=t[i].first;}}if(out.empty())out="a&!a";cout<<out<<"\n";}
