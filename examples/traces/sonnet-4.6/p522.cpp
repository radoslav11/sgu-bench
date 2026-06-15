#include<bits/stdc++.h>
using namespace std;
int main(){
    int n,x0,y0;
    cin>>n>>x0>>y0;
    vector<int>wx(n),wy(n);
    for(int i=0;i<n;i++)cin>>wx[i]>>wy[i];
    int xmn=*min_element(wx.begin(),wx.end());
    int xmx=*max_element(wx.begin(),wx.end());
    int ymn=*min_element(wy.begin(),wy.end());
    int ymx=*max_element(wy.begin(),wy.end());
    xmn=min(xmn,x0);xmx=max(xmx,x0);
    ymn=min(ymn,y0);ymx=max(ymx,y0);
    int W=xmx-xmn,H=ymx-ymn;
    bool onPerim=(x0==xmn||x0==xmx||y0==ymn||y0==ymx);
    if(!onPerim){cout<<-1;return 0;}
    if(W==0&&H==0){cout<<1<<"\nESWN";return 0;}
    if(W==0||H==0){cout<<-1;return 0;}
    long long area=(long long)W*H;
    // 4 ways to traverse rectangle from (x0,y0)
    // orientation: (d1,d2,d3,d4) where phase1={d1,d2}, phase2={d3,d4}
    struct Dir{int dx,dy;char c;};
    Dir N={0,1,'N'},S={0,-1,'S'},E={1,0,'E'},W2={-1,0,'W'};
    vector<array<Dir,4>> orients={{N,E,S,W2},{E,S,W2,N},{S,W2,N,E},{W2,N,E,S}};
    for(auto&o:orients){
        // from x0,y0: go in o[0] to corner, then o[1] to corner, then o[2] to corner, then o[3] back
        auto corner=[&](int x,int y,Dir d)->pair<int,int>{
            if(d.dx==1)return{xmx,y};
            if(d.dx==-1)return{xmn,y};
            if(d.dy==1)return{x,ymx};
            return{x,ymn};
        };
        string p="";
        int cx=x0,cy=y0;
        bool ok=true;
        for(int i=0;i<4;i++){
            auto[tx,ty]=(i<3)?corner(cx,cy,o[i]):make_pair(x0,y0);
            int ddx=tx-cx,ddy=ty-cy;
            if(ddx!=0&&ddy!=0){ok=false;break;}
            int steps=abs(ddx)+abs(ddy);
            if((ddx>0&&o[i].dx!=1)||(ddx<0&&o[i].dx!=-1)||(ddy>0&&o[i].dy!=1)||(ddy<0&&o[i].dy!=-1)){
                if(steps>0){ok=false;break;}
            }
            p+=string(steps,o[i].c);
            cx=tx;cy=ty;
        }
        if(ok&&cx==x0&&cy==y0&&p.size()>0){
            cout<<area<<"\n"<<p;
            return 0;
        }
    }
    cout<<-1;
}
