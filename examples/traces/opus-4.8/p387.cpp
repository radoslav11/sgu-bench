#include <bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld EPS=1e-7;

ld x1a[60],y1a[60],x2a[60],y2a[60];

pair<ld,ld> rot(int k, ld x, ld y){
    if(k==1) return {y,-x};
    if(k==2) return {-x,-y};
    return {-y,x};
}

vector<pair<ld,ld>> segInt(ld Ax,ld Ay,ld Bx,ld By,ld Qx0,ld Qy0,ld Qx1,ld Qy1){
    ld dx=Bx-Ax, dy=By-Ay;
    ld ex=Qx1-Qx0, ey=Qy1-Qy0;
    ld cr=dx*ey-dy*ex;
    vector<pair<ld,ld>> res;
    if(fabsl(cr)>1e-6){
        ld wx=Qx0-Ax, wy=Qy0-Ay;
        ld t=(wx*ey-wy*ex)/cr;
        ld s=(wx*dy-wy*dx)/cr;
        if(t>=-EPS && t<=1+EPS && s>=-EPS && s<=1+EPS){
            if(t<0)t=0; if(t>1)t=1;
            res.push_back({t,t});
        }
    } else {
        ld wx=Qx0-Ax, wy=Qy0-Ay;
        ld c2=wx*dy-wy*dx;
        if(fabsl(c2)<=1e-6){
            ld dd=dx*dx+dy*dy;
            ld t0=(wx*dx+wy*dy)/dd;
            ld wx2=Qx1-Ax, wy2=Qy1-Ay;
            ld t1=(wx2*dx+wy2*dy)/dd;
            ld lo=min(t0,t1), hi=max(t0,t1);
            lo=max(lo,(ld)0.0); hi=min(hi,(ld)1.0);
            if(lo<=hi+EPS) res.push_back({lo, max(lo,hi)});
        }
    }
    return res;
}

vector<pair<ld,ld>> intersectLists(const vector<pair<ld,ld>>&L1, const vector<pair<ld,ld>>&L2){
    vector<pair<ld,ld>> res;
    for(auto&a:L1) for(auto&b:L2){
        ld lo=max(a.first,b.first);
        ld hi=min(a.second,b.second);
        if(lo<=hi+EPS){
            res.push_back({lo, max(lo,hi)});
        }
    }
    return res;
}

void mergeList(vector<pair<ld,ld>>&v){
    if(v.empty())return;
    sort(v.begin(),v.end());
    vector<pair<ld,ld>> out;
    for(auto&p:v){
        if(out.empty()||p.first>out.back().second+1e-9){
            out.push_back(p);
        } else {
            out.back().second=max(out.back().second,p.second);
        }
    }
    v=out;
}

int main(){
    int n;
    if(scanf("%d",&n)!=1)return 0;
    for(int i=0;i<n;i++){
        double a,b,c,d;
        scanf("%lf %lf %lf %lf",&a,&b,&c,&d);
        x1a[i]=a;y1a[i]=b;x2a[i]=c;y2a[i]=d;
    }
    ld totalLen=0, integralSum=0;
    vector<pair<ld,ld>> points;
    for(int i=0;i<n;i++){
        ld Ax=x1a[i],Ay=y1a[i],Bx=x2a[i],By=y2a[i];
        vector<pair<ld,ld>> lst[3];
        for(int k=1;k<=3;k++){
            for(int j=0;j<n;j++){
                auto Q0=rot(k,x1a[j],y1a[j]);
                auto Q1=rot(k,x2a[j],y2a[j]);
                auto r=segInt(Ax,Ay,Bx,By,Q0.first,Q0.second,Q1.first,Q1.second);
                for(auto&p:r) lst[k-1].push_back(p);
            }
        }
        auto AB=intersectLists(lst[0],lst[1]);
        auto ABC=intersectLists(AB,lst[2]);
        mergeList(ABC);
        ld dx=Bx-Ax, dy=By-Ay;
        ld L=hypotl(dx,dy);
        for(auto&p:ABC){
            ld lo=p.first, hi=p.second;
            if(hi-lo>1e-9){
                ld c0=Ax*Ax+Ay*Ay;
                ld c1=2*(Ax*dx+Ay*dy);
                ld c2=dx*dx+dy*dy;
                ld intf=c0*(hi-lo)+c1*(hi*hi-lo*lo)/2+c2*(hi*hi*hi-lo*lo*lo)/3;
                integralSum+=2*L*intf;
                totalLen+=L*(hi-lo);
            } else {
                ld t=(lo+hi)/2;
                ld px=Ax+t*dx, py=Ay+t*dy;
                points.push_back({px,py});
            }
        }
    }
    ld ans;
    if(totalLen>1e-9){
        ans=integralSum/totalLen;
    } else {
        // dedupe points
        vector<pair<ld,ld>> uniq;
        for(auto&p:points){
            bool found=false;
            for(auto&q:uniq){
                if(fabsl(p.first-q.first)<1e-6 && fabsl(p.second-q.second)<1e-6){found=true;break;}
            }
            if(!found) uniq.push_back(p);
        }
        if(uniq.empty()){
            ans=0;
        } else {
            ld sum=0;
            for(auto&p:uniq){
                sum+=2*(p.first*p.first+p.second*p.second);
            }
            ans=sum/(ld)uniq.size();
        }
    }
    printf("%.10f\n",(double)ans);
    return 0;
}
