#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    if(!(cin>>n)) return 0;
    cin.ignore();
    vector<string> names;
    for(int i=0;i<n;i++){
        string line;
        getline(cin,line);
        // parse: name x y z, last three tokens are coords
        // find positions of last three space-separated integers
        // strip trailing
        int pos=line.size();
        int cnt=0;
        int cut=line.size();
        // walk from end, find 3 tokens
        int idx=line.size()-1;
        for(int t=0;t<3;t++){
            while(idx>=0 && line[idx]==' ') idx--;
            while(idx>=0 && line[idx]!=' ') idx--;
        }
        // idx points to space before name end
        string name = line.substr(0, idx);
        // trim trailing spaces
        while(!name.empty() && name.back()==' ') name.pop_back();
        names.push_back(name);
    }
    sort(names.begin(),names.end());
    cout<<names.size()<<"\n";
    for(auto&s:names) cout<<s<<"\n";
    return 0;
}
