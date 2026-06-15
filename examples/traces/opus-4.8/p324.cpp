#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; if(!(cin>>n))return 0; cin.ignore();
    while(n--){
        string line; getline(cin,line);
        int a=line.find('"'),b=line.rfind('"');
        string s=line.substr(a+1,b-a-1);
        int L=s.size();
        struct T2{int type;string v;bool sep;};
        vector<T2> T; int i=0; bool pendingSep=false;
        while(i<L){
            if(s[i]==' '){pendingSep=true;i++;continue;}
            if(s[i]=='-'){int j=i;while(j<L&&s[j]=='-')j++;T.push_back({1,s.substr(i,j-i),pendingSep});pendingSep=false;i=j;}
            else{int j=i;while(j<L&&s[j]!=' '&&s[j]!='-')j++;T.push_back({0,s.substr(i,j-i),pendingSep});pendingSep=false;i=j;}
        }
        bool trailingSep=pendingSep; int m=T.size();
        bool err=false; vector<int> kind(m,0);
        for(int k=0;k<m&&!err;k++){
            if(T[k].type==1){
                int len=T[k].v.size();
                bool sepL=T[k].sep, sepR=(k+1<m)?T[k+1].sep:trailingSep;
                bool lw=(k>0&&T[k-1].type==0), rw=(k+1<m&&T[k+1].type==0);
                if(len==1){
                    if(!sepL&&!sepR&&lw&&rw)kind[k]=1;
                    else kind[k]=2;
                } else if(len==2)kind[k]=2;
                else if(len==3)kind[k]=3;
                else err=true;
            }
        }
        if(err){cout<<"error\n";continue;}
        string out;
        for(int k=0;k<m;k++){
            if(k>0){
                bool hyL=(T[k-1].type==1&&kind[k-1]==1);
                bool hyR=(T[k].type==1&&kind[k]==1);
                if(hyL||hyR) ; else out+=" ";
            }
            if(T[k].type==1){
                if(kind[k]==1)out+="-"; else if(kind[k]==2)out+="--"; else out+="---";
            } else out+=T[k].v;
        }
        if(m>0&&T[m-1].type==1&&kind[m-1]>=2)out+=" ";
        cout<<"\""<<out<<"\"\n";
    }
}
