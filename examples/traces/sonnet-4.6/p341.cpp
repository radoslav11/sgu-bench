#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    string line;
    vector<string> inputs,outputs;
    struct Gate{string out,type;vector<string>ins;};
    vector<Gate>gates;
    map<string,int>val;
    while(getline(cin,line)){
        if(line=="INPUT VALUES")break;
        if(line.empty()||line[0]=='#')continue;
        if(line.size()>=6&&line.substr(0,6)=="INPUT("){string j=line.substr(6,line.size()-7);inputs.push_back(j);}
        else if(line.size()>=7&&line.substr(0,7)=="OUTPUT("){string j=line.substr(7,line.size()-8);outputs.push_back(j);}
        else{
            auto eq=line.find('=');
            string out=line.substr(0,eq);
            while(!out.empty()&&(out.back()==' '||out.back()=='\t'))out.pop_back();
            while(!out.empty()&&(out.front()==' '||out.front()=='\t'))out=out.substr(1);
            string rest=line.substr(eq+1);
            while(!rest.empty()&&(rest.front()==' '||rest.front()=='\t'))rest=rest.substr(1);
            auto lp=rest.find('(');auto rp=rest.rfind(')');
            string op=rest.substr(0,lp);
            while(!op.empty()&&op.back()==' ')op.pop_back();
            string args=rest.substr(lp+1,rp-lp-1);
            vector<string>ins;
            stringstream ss(args);string tok;
            while(getline(ss,tok,',')){
                while(!tok.empty()&&(tok.front()==' '||tok.front()=='\t'))tok=tok.substr(1);
                while(!tok.empty()&&(tok.back()==' '||tok.back()=='\t'))tok.pop_back();
                ins.push_back(tok);
            }
            gates.push_back({out,op,ins});
        }
    }
    map<string,int>dff_out;
    for(auto&g:gates)if(g.type=="DFF")dff_out[g.out]=0;
    // precompute topo order for combinational gates
    map<string,Gate*>gmap;
    for(auto&g:gates)if(g.type!="DFF")gmap[g.out]=&g;
    // build adjacency
    map<string,vector<string>>rdep;
    map<string,int>base_deg;
    for(auto&[n,gp]:gmap){
        base_deg[n]=0;
        for(auto&i:gp->ins)if(gmap.count(i)){base_deg[n]++;rdep[i].push_back(n);}
    }
    while(getline(cin,line)){
        if(line.empty())continue;
        // strip \r
        if(!line.empty()&&line.back()=='\r')line.pop_back();
        if(line.empty())continue;
        for(int i=0;i<(int)inputs.size()&&i<(int)line.size();i++)val[inputs[i]]=line[i]-'0';
        for(auto&[k,v]:dff_out)val[k]=v;
        map<string,int>deg=base_deg;
        queue<string>q;
        for(auto&[n,d]:deg)if(d==0)q.push(n);
        while(!q.empty()){
            auto n=q.front();q.pop();
            auto*g=gmap[n];
            auto&ins=g->ins;
            if(g->type=="NOT")val[n]=1-val[ins[0]];
            else if(g->type=="AND"){int r=1;for(auto&i:ins)r&=val[i];val[n]=r;}
            else if(g->type=="NAND"){int r=1;for(auto&i:ins)r&=val[i];val[n]=1-r;}
            else if(g->type=="OR"){int r=0;for(auto&i:ins)r|=val[i];val[n]=r;}
            else if(g->type=="NOR"){int r=0;for(auto&i:ins)r|=val[i];val[n]=1-r;}
            if(rdep.count(n))for(auto&s:rdep[n])if(--deg[s]==0)q.push(s);
        }
        map<string,int>nd;
        for(auto&g:gates)if(g.type=="DFF")nd[g.out]=val[g.ins[0]];
        dff_out=nd;
        for(auto&o:outputs)cout<<val[o];
        cout<<"\n";
    }
}
