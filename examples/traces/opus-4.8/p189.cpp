#include <bits/stdc++.h>
using namespace std;
map<string,string> vars;
string S;int P;
string trim(const string&s){int i=0,j=s.size();while(i<j&&isspace((unsigned char)s[i]))i++;while(j>i&&isspace((unsigned char)s[j-1]))j--;return s.substr(i,j-i);}
void skip(){while(P<(int)S.size()&&isspace((unsigned char)S[P]))P++;}
long long readNum(){skip();int sign=1;if(P<(int)S.size()&&S[P]=='-'){sign=-1;P++;skip();}else if(P<(int)S.size()&&S[P]=='+'){P++;skip();}long long v=0;while(P<(int)S.size()&&isdigit((unsigned char)S[P])){v=v*10+(S[P]-'0');P++;}return sign*v;}
string readName(){skip();int s=P;while(P<(int)S.size()){char c=S[P];if(isalnum((unsigned char)c)||c=='$'||c=='_')P++;else break;}return S.substr(s,P-s);}
void expect(char ch){skip();if(P<(int)S.size()&&S[P]==ch)P++;}
string getStr(const string&str,long long o,bool hasC,long long c,int&outStart,int&outEnd){int len=str.size();int start=(o>=0)?(int)o:len+(int)o;int end;if(!hasC)end=len;else if(c>=0)end=start+(int)c;else end=len+(int)c;outStart=start;outEnd=end;return str.substr(start,end-start);}
int main(){
    int N,M;{string line;getline(cin,line);stringstream ss(line);ss>>N>>M;}
    for(int i=0;i<N;){string line;if(!getline(cin,line))break;if(line.find('=')==string::npos)continue;S=line;P=0;string name=readName();skip();expect('=');skip();int q1=S.find('"',P);int q2=S.find('"',q1+1);vars[name]=S.substr(q1+1,q2-q1-1);i++;}
    string out;
    for(int i=0;i<M;){
        string line;if(!getline(cin,line))break;if(trim(line).empty())continue;S=line;P=0;skip();
        string first=readName();
        if(first=="print"){
            expect('(');skip();string inner=readName();
            if(inner=="substr"){expect('(');string nm=readName();expect(',');long long o=readNum();bool hasC=false;long long c=0;skip();if(P<(int)S.size()&&S[P]==','){P++;c=readNum();hasC=true;}expect(')');int a,b;out+=getStr(vars[nm],o,hasC,c,a,b);}
            else out+=vars[inner];
            out+="\n";i++;
        }else if(first=="substr"){
            expect('(');string nm=readName();expect(',');long long o=readNum();bool hasC=false;long long c=0;skip();if(P<(int)S.size()&&S[P]==','){P++;c=readNum();hasC=true;}expect(')');skip();expect('=');skip();string r=readName();string rhs;
            if(r=="substr"){expect('(');string nm2=readName();expect(',');long long o2=readNum();bool hc2=false;long long c2=0;skip();if(P<(int)S.size()&&S[P]==','){P++;c2=readNum();hc2=true;}expect(')');int a,b;rhs=getStr(vars[nm2],o2,hc2,c2,a,b);}else rhs=vars[r];
            int a,b;getStr(vars[nm],o,hasC,c,a,b);string&t=vars[nm];t=t.substr(0,a)+rhs+t.substr(b);i++;
        }else{
            string nm=first;skip();expect('=');skip();string r=readName();
            if(r=="substr"){expect('(');string nm2=readName();expect(',');long long o2=readNum();bool hc2=false;long long c2=0;skip();if(P<(int)S.size()&&S[P]==','){P++;c2=readNum();hc2=true;}expect(')');int a,b;vars[nm]=getStr(vars[nm2],o2,hc2,c2,a,b);}else vars[nm]=vars[r];
            i++;
        }
    }
    cout<<out;
}
