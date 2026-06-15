#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    getline(cin,s);
    vector<int> st;
    string out;
    int i=0,n=s.size();
    while(i<n){
        if(s[i]=='<'){
            int j=s.find('>',i);
            string tag=s.substr(i+1,j-i-1);
            if(tag=="UP") st.push_back(1);
            else if(tag=="DOWN") st.push_back(0);
            else if(tag=="/UP"||tag=="/DOWN") st.pop_back();
            i=j+1;
        } else {
            char c=s[i];
            if(isalpha((unsigned char)c)&&!st.empty()){
                if(st.back()) c=toupper(c);
                else c=tolower(c);
            }
            out+=c;
            i++;
        }
    }
    cout<<out<<endl;
}
