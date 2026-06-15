#include <bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld EPS=1e-9;
struct Pt{ld x,y;};
ld cr(Pt a,Pt b){return a.x*b.y-a.y*b.x;}
Pt sb(Pt a,Pt b){return {a.x-b.x,a.y-b.y};}
ld sd(Pt l1,Pt l2,Pt p){return cr(sb(l2,l1),sb(p,l1));}
ld ar(vector<Pt>&v){ld s=0;int n=v.size();for(int i=0;i<n;i++)s+=cr(v[i],v[(i+1)%n]);return fabsl(s)/2;}
int main(){
int n;scanf("%d",&n);
vector<Pt> poly(n);
for(int i=0;i<n;i++)scanf("%Lf%Lf",&poly[i].x,&poly[i].y);
int q;scanf("%d",&q);
while(q--){
Pt l1,l2;scanf("%Lf%Lf%Lf%Lf",&l1.x,&l1.y,&l2.x,&l2.y);
vector<Pt> pos,neg;
for(int i=0;i<n;i++){
Pt a=poly[i],b=poly[(i+1)%n];
ld sa=sd(l1,l2,a),sb2=sd(l1,l2,b);
if(sa>=0)pos.push_back(a);
if(sa<=0)neg.push_back(a);
if((sa>EPS&&sb2<-EPS)||(sa<-EPS&&sb2>EPS)){
ld t=sa/(sa-sb2);
Pt ip={a.x+t*(b.x-a.x),a.y+t*(b.y-a.y)};
pos.push_back(ip);neg.push_back(ip);
}
}
ld a1=ar(pos),a2=ar(neg);
printf("%.6Lf\n",min(a1,a2));
}
}
