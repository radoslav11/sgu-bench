#include<bits/stdc++.h>
using namespace std;
double X;

map<tuple<int,int,int>,double> Cm;
double Cprofit(int sc,int sb,int sa){
    if(sc>27){
        double p=0;
        if(sb<=21)p-=X;
        if(sa<=21)p-=1.0;
        return p;
    }
    auto k=make_tuple(sc,sb,sa);
    auto it=Cm.find(k);if(it!=Cm.end())return it->second;
    double sv=0;
    if(sc<=21){
        if(sb>21)sv+=X;else if(sb<sc)sv+=X;else if(sb>sc)sv-=X;
        if(sa>21)sv+=1.0;else if(sa<sc)sv+=1.0;else if(sa>sc)sv-=1.0;
    }else{if(sb<=21)sv-=X;if(sa<=21)sv-=1.0;}
    double rv=0;for(int d=1;d<=6;d++)rv+=Cprofit(sc+d,sb,sa);rv/=6;
    return Cm[k]=(rv>=sv?rv:sv);
}

map<tuple<int,int,int>,double> Cwm;
double Cwin(int sc,int sb,int sa){
    if(sc>27)return(sa<=21)?1.0:0.0;
    auto k=make_tuple(sc,sb,sa);
    auto it=Cwm.find(k);if(it!=Cwm.end())return it->second;
    double sp=0;
    if(sc<=21){
        if(sb>21)sp+=X;else if(sb<sc)sp+=X;else if(sb>sc)sp-=X;
        if(sa>21)sp+=1.0;else if(sa<sc)sp+=1.0;else if(sa>sc)sp-=1.0;
    }else{if(sb<=21)sp-=X;if(sa<=21)sp-=1.0;}
    double rp=0;for(int d=1;d<=6;d++)rp+=Cprofit(sc+d,sb,sa);rp/=6;
    double res;
    if(rp>=sp){
        double w=0;for(int d=1;d<=6;d++)w+=Cwin(sc+d,sb,sa);w/=6;res=w;
    }else{
        if(sc<=21)res=(sa<=21&&sa>sc)?1.0:0.0;
        else res=(sa<=21)?1.0:0.0;
    }
    return Cwm[k]=res;
}

map<pair<int,int>,double> Am;
double Awin(int sa,int sb){
    if(sa>21)return 0.0;
    auto k=make_pair(sa,sb);
    auto it=Am.find(k);if(it!=Am.end())return it->second;
    double sv=0;for(int d=1;d<=6;d++)sv+=Cwin(d,sb,sa);sv/=6;
    double rv=0;for(int d=1;d<=6;d++)rv+=Awin(sa+d,sb);rv/=6;
    return Am[k]=(rv>=sv?rv:sv);
}

map<pair<int,int>,double> Cspm;
double Csolo_profit(int sc,int sb){
    if(sc>27)return(sb<=21)?-1.0:0.0;
    auto k=make_pair(sc,sb);
    auto it=Cspm.find(k);if(it!=Cspm.end())return it->second;
    double sv=0;
    if(sc<=21){if(sb>21)sv=1;else if(sb<sc)sv=1;else if(sb>sc)sv=-1;}
    else{if(sb<=21)sv=-1;}
    double rv=0;for(int d=1;d<=6;d++)rv+=Csolo_profit(sc+d,sb);rv/=6;
    return Cspm[k]=(rv>=sv?rv:sv);
}

map<pair<int,int>,double> Cswm;
double Csolo_win(int sc,int sb){
    if(sc>27)return(sb<=21)?1.0:0.0;
    auto k=make_pair(sc,sb);
    auto it=Cswm.find(k);if(it!=Cswm.end())return it->second;
    double sp=0;
    if(sc<=21){if(sb>21)sp=1;else if(sb<sc)sp=1;else if(sb>sc)sp=-1;}
    else{if(sb<=21)sp=-1;}
    double rp=0;for(int d=1;d<=6;d++)rp+=Csolo_profit(sc+d,sb);rp/=6;
    double res;
    if(rp>=sp){double w=0;for(int d=1;d<=6;d++)w+=Csolo_win(sc+d,sb);w/=6;res=w;}
    else{if(sc<=21)res=(sb<=21&&sb>sc)?1.0:0.0;else res=(sb<=21)?1.0:0.0;}
    return Cswm[k]=res;
}

map<int,double> Bwm;
double Bwin(int sb){
    if(sb>21)return 0.0;
    auto it=Bwm.find(sb);if(it!=Bwm.end())return it->second;
    double sv=0;for(int d=1;d<=6;d++)sv+=Csolo_win(d,sb);sv/=6;
    double rv=0;for(int d=1;d<=6;d++)rv+=Bwin(sb+d);rv/=6;
    return Bwm[sb]=(rv>=sv?rv:sv);
}

bool Bstops(int sb){
    if(sb>21)return true;
    double sv=0;for(int d=1;d<=6;d++)sv+=Csolo_win(d,sb);sv/=6;
    double rv=0;for(int d=1;d<=6;d++)rv+=Bwin(sb+d);rv/=6;
    return sv>rv;
}

int main(){
    cin>>X;
    // precompute to ensure Bwin memoized
    for(int i=1;i<=27;i++)Bwin(i);
    
    map<int,double> current,done;
    for(int d=1;d<=6;d++)current[d]+=1.0/6;
    while(!current.empty()){
        map<int,double> next;
        for(auto&[s,p]:current){
            if(Bstops(s)){done[s]+=p;}
            else{for(int d=1;d<=6;d++)next[s+d]+=p/6;}
        }
        current=next;
    }
    
    double ans=0;
    for(auto&[sb,pb]:done){
        double aw=0;
        for(int d=1;d<=6;d++)aw+=Awin(d,sb);
        aw/=6;
        ans+=pb*aw;
    }
    printf("%.6f\n",ans);
}
