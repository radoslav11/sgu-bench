#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n; cin.ignore();
    vector<string> records(n);
    for(int i=0;i<n;i++) getline(cin,records[i]);
    set<string> inside;
    vector<string> output;
    int guestCounter=1;
    auto freshName=[&]()->string{
        string name;
        do{ name="guest"+to_string(guestCounter++); }while(inside.count(name));
        return name;
    };
    for(auto& rec:records){
        if(rec[0]=='+'){
            string name=rec.substr(2);
            if(inside.count(name)){ output.push_back("- "+name); inside.erase(name); }
            output.push_back(rec); inside.insert(name);
        } else if(rec[0]=='-'){
            string name=rec.substr(2);
            if(!inside.count(name)){ output.push_back("+ "+name); inside.insert(name); }
            output.push_back(rec); inside.erase(name);
        } else {
            int k=stoi(rec.substr(2));
            int cur=inside.size();
            if(k>cur){ int need=k-cur; for(int i=0;i<need;i++){ string fn=freshName(); output.push_back("+ "+fn); inside.insert(fn); } }
            else if(k<cur){ int rem=cur-k; vector<string> tr; for(auto it=inside.begin();it!=inside.end()&&(int)tr.size()<rem;++it) tr.push_back(*it); for(auto& nm:tr){ output.push_back("- "+nm); inside.erase(nm); } }
            output.push_back(rec);
        }
    }
    cout<<output.size()<<"\n";
    for(auto& line:output) cout<<line<<"\n";
}
