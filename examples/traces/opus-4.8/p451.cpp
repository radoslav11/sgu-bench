#include <bits/stdc++.h>
using namespace std;

struct Node{ int father=-1,mother=-1,spouse=-1; vector<int> children; };
vector<Node> nodes;
int newNode(){ nodes.push_back(Node()); return nodes.size()-1; }

int getFather(int x){ if(nodes[x].father<0){int f=newNode();int m=newNode();nodes[f].spouse=m;nodes[m].spouse=f;nodes[x].father=f;nodes[x].mother=m;nodes[f].children.push_back(x);nodes[m].children.push_back(x);} return nodes[x].father;}
int getMother(int x){ getFather(x); return nodes[x].mother;}
int getSpouse(int x){ if(nodes[x].spouse<0){int s=newNode();nodes[x].spouse=s;nodes[s].spouse=x;} return nodes[x].spouse;}
int newChild(int x){ int s=getSpouse(x); int c=newNode(); nodes[c].father=x;nodes[c].mother=s;nodes[x].children.push_back(c);nodes[s].children.push_back(c); return c;}

int main(){
    string line; getline(cin,line);
    // parse tokens after "C is A"
    // split by 's and spaces
    vector<string> rels;
    // find relations: words after A
    stringstream ss(line); vector<string> toks; string t;
    while(ss>>t) toks.push_back(t);
    // toks: C is A's father's ... ; relations separated
    for(int i=3;i<(int)toks.size();i++){
        string w=toks[i];
        // remove trailing 's
        size_t p=w.find("'s");
        if(p!=string::npos) w=w.substr(0,p);
        if(!w.empty()) rels.push_back(w);
    }
    int A=newNode();
    set<int> cur; cur.insert(A);
    auto sons=[&](int x,bool wantSon)->vector<int>{
        // children of x and spouse with gender; create new + existing
        vector<int> res;
        for(int c:nodes[x].children) res.push_back(c);
        res.push_back(newChild(x));
        return res;
    };
    for(string r:rels){
        set<int> nxt;
        for(int x:cur){
            if(r=="father") nxt.insert(getFather(x));
            else if(r=="mother") nxt.insert(getMother(x));
            else if(r=="son"||r=="daughter") for(int c:sons(x,true)) nxt.insert(c);
            else if(r=="husband"||r=="wife") nxt.insert(getSpouse(x));
            else if(r=="brother"||r=="sister"){
                int f=getFather(x);
                for(int c:nodes[f].children) if(c!=x) nxt.insert(c);
                nxt.insert(newChild(f));
            } else if(r=="grandfather") nxt.insert(getFather(getFather(x))),nxt.insert(getFather(getMother(x)));
            else if(r=="grandmother") nxt.insert(getMother(getFather(x))),nxt.insert(getMother(getMother(x)));
            else if(r=="grandson"||r=="granddaughter"){
                for(int c:sons(x,true)) for(int g:sons(c,true)) nxt.insert(g);
            } else if(r=="uncle"||r=="aunt"){
                for(int par:{getFather(x),getMother(x)}){int gf=getFather(par); for(int c:nodes[gf].children) if(c!=par) nxt.insert(c); nxt.insert(newChild(gf));}
            } else if(r=="nephew"||r=="niece"){
                int f=getFather(x); vector<int> sibs;
                for(int c:nodes[f].children) if(c!=x) sibs.push_back(c);
                sibs.push_back(newChild(f));
                for(int s:sibs) for(int c:sons(s,true)) nxt.insert(c);
            }
        }
        cur=nxt;
    }
    // BFS 0-1
    int n=nodes.size();
    vector<int> dist(n,INT_MAX); deque<int> dq; dist[A]=0; dq.push_back(A);
    while(!dq.empty()){int u=dq.front();dq.pop_front();
        auto rel=[&](int v,int w){ if(v>=0&&dist[u]+w<dist[v]){dist[v]=dist[u]+w; if(w==0)dq.push_front(v); else dq.push_back(v);} };
        rel(nodes[u].spouse,0); rel(nodes[u].father,1); rel(nodes[u].mother,1);
        for(int c:nodes[u].children) rel(c,1);
    }
    int mx=0,mn=INT_MAX;
    for(int x:cur){ mx=max(mx,dist[x]); mn=min(mn,dist[x]);}
    cout<<mx<<" "<<mn<<"\n";
}
