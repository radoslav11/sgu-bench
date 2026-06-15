#include <bits/stdc++.h>
using namespace std;

static string trim(const string&s){
    int i=0,j=(int)s.size()-1;
    while(i<=j&&isspace((unsigned char)s[i]))i++;
    while(j>=i&&isspace((unsigned char)s[j]))j--;
    return s.substr(i,j-i+1);
}

enum Type{TINPUT,TNOT,TAND,TNAND,TOR,TNOR,TDFF};

int main(){
    unordered_map<string,int> id;
    id.reserve(20000);
    vector<int> type;
    vector<vector<int>> args;
    auto getId=[&](const string&name)->int{
        auto it=id.find(name);
        if(it!=id.end())return it->second;
        int nid=type.size();
        id[name]=nid;
        type.push_back(-1);
        args.push_back({});
        return nid;
    };
    vector<int> inputOrder;
    vector<int> outputOrder;
    
    string line;
    bool inValues=false;
    vector<string> valueLines;
    while(getline(cin,line)){
        string t=trim(line);
        if(t.empty())continue;
        if(t[0]=='#')continue;
        if(t=="INPUT VALUES"){inValues=true;break;}
        // check INPUT( or OUTPUT(
        if(t.rfind("INPUT(",0)==0){
            // INPUT(name)
            int p1=t.find('(');
            int p2=t.find(')');
            string name=trim(t.substr(p1+1,p2-p1-1));
            int nid=getId(name);
            type[nid]=TINPUT;
            inputOrder.push_back(nid);
        } else if(t.rfind("OUTPUT(",0)==0){
            int p1=t.find('(');
            int p2=t.find(')');
            string name=trim(t.substr(p1+1,p2-p1-1));
            int nid=getId(name);
            outputOrder.push_back(nid);
        } else {
            // definition j1 = op(args)
            int eq=t.find('=');
            string left=trim(t.substr(0,eq));
            string right=trim(t.substr(eq+1));
            int p1=right.find('(');
            int p2=right.rfind(')');
            string op=trim(right.substr(0,p1));
            string inside=right.substr(p1+1,p2-p1-1);
            // split by comma
            vector<int> as;
            {
                string cur;
                for(char c:inside){
                    if(c==','){
                        string nm=trim(cur);
                        if(!nm.empty())as.push_back(getId(nm));
                        cur.clear();
                    } else cur.push_back(c);
                }
                string nm=trim(cur);
                if(!nm.empty())as.push_back(getId(nm));
            }
            int nid=getId(left);
            args[nid]=as;
            int tp;
            if(op=="NOT")tp=TNOT;
            else if(op=="AND")tp=TAND;
            else if(op=="NAND")tp=TNAND;
            else if(op=="OR")tp=TOR;
            else if(op=="NOR")tp=TNOR;
            else if(op=="DFF")tp=TDFF;
            else tp=TINPUT;
            type[nid]=tp;
        }
    }
    if(inValues){
        while(getline(cin,line)){
            string t=trim(line);
            if(t.empty())continue;
            valueLines.push_back(t);
        }
    }
    
    int N=type.size();
    // build adjacency for combinational gates; DFF and INPUT are sources
    vector<vector<int>> adj(N);
    vector<int> indeg(N,0);
    for(int i=0;i<N;i++){
        int tp=type[i];
        if(tp==TINPUT||tp==TDFF)continue;
        for(int a:args[i]){
            adj[a].push_back(i);
            indeg[i]++;
        }
    }
    // Kahn
    vector<int> order;
    order.reserve(N);
    queue<int> q;
    for(int i=0;i<N;i++)if(indeg[i]==0)q.push(i);
    while(!q.empty()){
        int u=q.front();q.pop();
        order.push_back(u);
        for(int v:adj[u]){
            if(--indeg[v]==0)q.push(v);
        }
    }
    
    // dff state
    vector<char> dffState(N,0);
    vector<char> val(N,0);
    
    // list of dff ids
    vector<int> dffs;
    for(int i=0;i<N;i++)if(type[i]==TDFF)dffs.push_back(i);
    
    string out;
    for(const string& vl:valueLines){
        // set inputs
        for(size_t k=0;k<inputOrder.size()&&k<vl.size();k++){
            val[inputOrder[k]]=(vl[k]=='1')?1:0;
        }
        // set dff outputs from state
        for(int d:dffs)val[d]=dffState[d];
        // evaluate in topo order
        for(int u:order){
            int tp=type[u];
            if(tp==TINPUT||tp==TDFF)continue;
            switch(tp){
                case TNOT: val[u]=val[args[u][0]]?0:1; break;
                case TAND:{
                    char r=1;
                    for(int a:args[u])if(!val[a]){r=0;break;}
                    val[u]=r; break;
                }
                case TNAND:{
                    char r=1;
                    for(int a:args[u])if(!val[a]){r=0;break;}
                    val[u]=r?0:1; break;
                }
                case TOR:{
                    char r=0;
                    for(int a:args[u])if(val[a]){r=1;break;}
                    val[u]=r; break;
                }
                case TNOR:{
                    char r=0;
                    for(int a:args[u])if(val[a]){r=1;break;}
                    val[u]=r?0:1; break;
                }
            }
        }
        // outputs
        for(int o:outputOrder)out.push_back(val[o]?'1':'0');
        out.push_back('\n');
        // update dff states
        for(int d:dffs)dffState[d]=val[args[d][0]];
    }
    cout<<out;
    return 0;
}
