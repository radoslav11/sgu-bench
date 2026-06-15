#include <bits/stdc++.h>
using namespace std;

static char buf[1<<25];
static int bufpos=0, buflen=0;
inline int gc(){ if(bufpos==buflen){ buflen=fread(buf,1,sizeof(buf),stdin); bufpos=0; if(buflen==0) return -1;} return buf[bufpos++]; }
inline long long readInt(){ int c=gc(); while(c!=-1 && (c<'0'||c>'9') && c!='-') c=gc(); int s=1; if(c=='-'){s=-1;c=gc();} long long x=0; while(c>='0'&&c<='9'){ x=x*10+(c-'0'); c=gc(); } return x*s; }

int main(){
    int t=(int)readInt();
    while(t--){
        long long n=readInt(); int m=(int)readInt();
        vector<pair<long long,int>> ev;
        ev.reserve((size_t)m*6+8);
        bool ok=true;
        auto addEdge=[&](long long x1,long long y1,long long x2,long long y2){
            long long dx=x2-x1, dy=y2-y1;
            long long type,lc,p1,p2;
            if(dy==0){ type=0; lc=y1; p1=x1; p2=x2; }
            else if(dx==0){ type=1; lc=x1; p1=y1; p2=y2; }
            else if(dx==dy){ type=2; lc=y1-x1; p1=x1; p2=x2; }
            else { type=3; lc=x1+y1; p1=x1; p2=x2; }
            long long base=(type*75001LL+(lc+25000))*50001LL;
            ev.push_back({base+p1,+1});
            ev.push_back({base+p2,-1});
        };
        for(int i=0;i<m;i++){
            long long x1=readInt(),y1=readInt(),x2=readInt(),y2=readInt(),x3=readInt(),y3=readInt();
            int h=0,v=0,d=0;
            long long L=-1,Lv=-1,Ld=-1;
            auto cls=[&](long long ax,long long ay,long long bx,long long by){
                long long dx=bx-ax, dy=by-ay;
                if(dy==0){h++; L=llabs(dx);}
                else if(dx==0){v++; Lv=llabs(dy);}
                else if(llabs(dx)==llabs(dy)){d++; Ld=llabs(dx);}
                else ok=false;
            };
            cls(x1,y1,x2,y2);
            cls(x2,y2,x3,y3);
            cls(x3,y3,x1,y1);
            if(!(h==1&&v==1&&d==1)) ok=false;
            else if(!(L==Lv && Ld==L)) ok=false;
            long long cross=(x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
            if(cross<0){ swap(x2,x3); swap(y2,y3); }
            addEdge(x1,y1,x2,y2);
            addEdge(x2,y2,x3,y3);
            addEdge(x3,y3,x1,y1);
        }
        // big triangle CW: (0,0)->(0,n)->(n,0)->(0,0)
        addEdge(0,0,0,n);
        addEdge(0,n,n,0);
        addEdge(n,0,0,0);
        if(ok){
            sort(ev.begin(),ev.end());
            size_t i=0;
            while(i<ev.size()){
                size_t j=i; long long sum=0; long long k=ev[i].first;
                while(j<ev.size() && ev[j].first==k){ sum+=ev[j].second; j++; }
                if(sum!=0){ ok=false; break; }
                i=j;
            }
        }
        printf(ok?"YES\n":"NO\n");
    }
    return 0;
}
