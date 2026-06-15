#include <bits/stdc++.h>
using namespace std;
typedef long double ld;

struct Iv{ ld a,b; };

vector<Iv> normalize(vector<Iv> v){
    if(v.empty()) return v;
    sort(v.begin(),v.end(),[](const Iv&x,const Iv&y){return x.a<y.a;});
    vector<Iv> r;
    for(auto&iv:v){
        if(!r.empty() && iv.a <= r.back().b + 1e-12L){
            r.back().b = max(r.back().b, iv.b);
        } else r.push_back(iv);
    }
    return r;
}

int main(){
    ld v,G; int n;
    {
        long long vv,GG; cin>>vv>>GG>>n; v=vv; G=GG;
    }
    map<ld, vector<pair<ld,ld>>> mp;
    for(int i=0;i<n;i++){
        long long x1,x2,t; cin>>x1>>x2>>t;
        mp[(ld)t].push_back({(ld)x1,(ld)x2});
    }
    vector<pair<ld, vector<pair<ld,ld>>>> events;
    for(auto&pr:mp) events.push_back({pr.first, pr.second});
    events.push_back({(ld)1e18, {}}); // sentinel

    vector<pair<ld, vector<Iv>>> snaps;
    vector<Iv> R; R.push_back({0,0});
    ld cur=0;
    snaps.push_back({0, R});

    bool found=false;
    ld T=0, p_arr=0;

    for(auto&ev: events){
        ld te = ev.first;
        ld dtmax = te - cur;
        // check arrival
        ld best_s = 1e30L; ld witness=0; bool any=false;
        for(auto&iv:R){
            ld s,w;
            if(G>=iv.a && G<=iv.b){ s=0; w=G; }
            else if(G<iv.a){ s=(iv.a-G)/v; w=iv.a; }
            else { s=(G-iv.b)/v; w=iv.b; }
            if(s<best_s){ best_s=s; witness=w; any=true; }
        }
        if(any && cur+best_s <= te + 1e-6L){
            T = cur + best_s;
            p_arr = witness;
            found=true;
            break;
        }
        // expand R by dtmax
        ld s=dtmax;
        for(auto&iv:R){ iv.a -= v*s; iv.b += v*s; }
        R = normalize(R);
        cur = te;
        // subtract stones
        for(auto&st: ev.second){
            ld x1=st.first, x2=st.second;
            if(x1>=x2) continue;
            vector<Iv> nr;
            for(auto&iv:R){
                ld a=iv.a,b=iv.b;
                if(a < x2 && b > x1){
                    if(a <= x1) nr.push_back({a, min(b,x1)});
                    if(b >= x2) nr.push_back({max(a,x2), b});
                } else nr.push_back({a,b});
            }
            R = normalize(nr);
        }
        if(R.empty()){
            cout<<-1<<"\n";
            return 0;
        }
        snaps.push_back({cur, R});
    }

    if(!found){
        cout<<-1<<"\n"; return 0;
    }

    // reconstruct backward
    int m = snaps.size();
    vector<ld> pos(m);
    pos[m-1]=p_arr;
    for(int i=m-2;i>=0;i--){
        ld t_cur = snaps[i+1].first;
        ld t_prev = snaps[i].first;
        ld dt = t_cur - t_prev;
        ld pn = pos[i+1];
        ld bestd=1e30L, bestp=0; bool ok=false;
        for(auto&iv: snaps[i].second){
            ld cand = min(max(pn, iv.a), iv.b);
            ld d=fabsl(pn-cand);
            if(d <= v*dt + 1e-5L){
                if(d<bestd){bestd=d;bestp=cand;ok=true;}
            }
        }
        if(!ok){
            for(auto&iv: snaps[i].second){
                ld cand = min(max(pn, iv.a), iv.b);
                ld d=fabsl(pn-cand);
                if(d<bestd){bestd=d;bestp=cand;ok=true;}
            }
        }
        pos[i]=bestp;
    }

    // waypoints: snapshots then (T,G)
    vector<pair<ld,ld>> wp;
    for(int i=0;i<m;i++) wp.push_back({snaps[i].first, pos[i]});
    wp.push_back({T, G});

    vector<pair<ld,ld>> segs;
    for(size_t i=1;i<wp.size();i++){
        ld dt = wp[i].first - wp[i-1].first;
        if(dt <= 1e-12L) continue;
        ld w = (wp[i].second - wp[i-1].second)/dt;
        segs.push_back({w,dt});
    }

    cout.setf(std::ios::fixed); cout<<setprecision(12);
    cout<<segs.size()<<"\n";
    for(auto&s:segs){
        cout<<(double)s.first<<" "<<(double)s.second<<"\n";
    }
    return 0;
}
