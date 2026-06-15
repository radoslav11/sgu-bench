#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const char dchars[]={'_','.',',' ,'-','~','=','\'','^','"'};
const int dmaxes[]={2,3,5,7,11,13,17,19,23};

ll prods[9]; // prods[i] = product of (dmaxes[0..i-1]+1), prods[0]=1

void init_prods(){
    prods[0]=1;
    for(int i=1;i<9;i++) prods[i]=prods[i-1]*(dmaxes[i-1]+1);
}

ll diputs_to_rank(const string& s){
    int cnt[9]={};
    for(char c:s){
        for(int i=0;i<9;i++) if(dchars[i]==c){cnt[i]++;break;}
    }
    ll rank=0;
    for(int i=0;i<9;i++) rank+=(ll)cnt[i]*prods[i];
    return rank;
}

string rank_to_diputs(ll rank){
    if(rank==0) return "O";
    int cnt[9]={};
    for(int i=8;i>=0;i--){
        cnt[i]=rank/prods[i];
        rank%=prods[i];
    }
    string res="";
    for(int i=8;i>=0;i--) for(int j=0;j<cnt[i];j++) res+=dchars[i];
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    init_prods();
    
    ll MAX_RANK=1;
    for(int i=0;i<9;i++) MAX_RANK*=(dmaxes[i]+1);
    MAX_RANK--;
    
    string input((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    
    set<char> dset(dchars, dchars+9);
    
    struct Token {
        bool is_number;
        bool is_diputs;
        ll value;
        string text;
    };
    
    vector<Token> tokens;
    
    int n=input.size();
    int i=0;
    while(i<n){
        char c=input[i];
        if(c=='O'){
            tokens.push_back({true,true,0,"O"});
            i++;
        } else if(dset.count(c)){
            int cnt[9]={};
            string s="";
            int j=i;
            while(j<n && dset.count(input[j])){
                char ch=input[j];
                int di=-1;
                for(int k=0;k<9;k++) if(dchars[k]==ch){di=k;break;}
                if(di>=0 && cnt[di]<dmaxes[di]){
                    cnt[di]++;
                    s+=ch;
                    j++;
                } else break;
            }
            if(s.empty()){
                tokens.push_back({false,false,0,string(1,c)});
                i++;
            } else {
                ll val=diputs_to_rank(s);
                tokens.push_back({true,true,val,s});
                i=j;
            }
        } else if(isdigit(c)){
            if(c=='0'){
                tokens.push_back({true,false,0,"0"});
                i++;
            } else {
                int j=i;
                string s="";
                while(j<n && isdigit(input[j])){s+=input[j];j++;}
                // split greedily into parts <= MAX_RANK
                int start=0;
                int slen=s.size();
                string maxs=to_string(MAX_RANK);
                int maxlen=maxs.size();
                while(start<slen){
                    int rem=slen-start;
                    int take=min(rem,maxlen);
                    string sub=s.substr(start,take);
                    ll val=stoll(sub);
                    if(val>MAX_RANK){
                        take--;
                        sub=s.substr(start,take);
                        val=stoll(sub);
                    }
                    tokens.push_back({true,false,val,sub});
                    start+=take;
                }
                i=j;
            }
        } else {
            tokens.push_back({false,false,0,string(1,c)});
            i++;
        }
    }
    
    // collect numbers
    vector<int> num_idx;
    vector<ll> vals;
    for(int k=0;k<(int)tokens.size();k++){
        if(tokens[k].is_number){
            num_idx.push_back(k);
            vals.push_back(tokens[k].value);
        }
    }
    
    if(vals.size()%2==1){
        sort(vals.begin(),vals.end());
        for(int k=0;k<(int)num_idx.size();k++)
            tokens[num_idx[k]].value=vals[k];
    }
    
    for(auto& t:tokens){
        if(!t.is_number) cout<<t.text;
        else if(t.is_diputs) cout<<t.value;
        else cout<<rank_to_diputs(t.value);
    }
    
    return 0;
}
