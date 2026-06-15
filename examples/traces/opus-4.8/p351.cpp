#include <bits/stdc++.h>
using namespace std;
typedef double ld;
const ld PI=acos(-1.0L);
ld xa,ya,xb,yb,xc,yc,rc,D;

ld dist(ld x1,ld y1,ld x2,ld y2){return hypot(x1-x2,y1-y2);}

// distance from point to segment AB, also whether interior chord crosses circle
// compute chord length of segment intersect with circle interior
ld chordLen(ld ax,ld ay,ld bx,ld by){
    ld dx=bx-ax, dy=by-ay;
    ld len=hypot(dx,dy);
    if(len<1e-12) return 0;
    dx/=len; dy/=len;
    ld fx=ax-xc, fy=ay-yc;
    ld a=1.0;
    ld b=2*(fx*dx+fy*dy);
    ld c=fx*fx+fy*fy-rc*rc;
    ld disc=b*b-4*a*c;
    if(disc<=0) return 0;
    disc=sqrt(disc);
    ld t1=(-b-disc)/2, t2=(-b+disc)/2;
    t1=max(t1,0.0); t2=min(t2,len);
    if(t2<=t1) return 0;
    return t2-t1;
}

struct Part{char t;ld x1,y1,x2,y2;};

int main(){
    cin>>xa>>ya>>xb>>yb>>xc>>yc>>rc>>D;
    // check straight
    ld ch=chordLen(xa,ya,xb,yb);
    if(ch<=D+1e-9){
        printf("1\n");
        printf("S %.8f %.8f %.8f %.8f\n",xa,ya,xb,yb);
        return 0;
    }
    // need to go around. tangent points.
    ld dA=dist(xa,ya,xc,yc), dB=dist(xb,yb,xc,yc);
    ld lenTA=sqrt(dA*dA-rc*rc), lenTB=sqrt(dB*dB-rc*rc);
    // angle of A from center
    ld angA=atan2(ya-yc,xa-xc);
    ld angB=atan2(yb-yc,xb-xc);
    // tangent point angles: angA +/- alphaA where alphaA=acos(rc/dA)
    ld alA=acos(rc/dA), alB=acos(rc/dB);
    
    auto norm=[&](ld a){while(a<0)a+=2*PI;while(a>=2*PI)a-=2*PI;return a;};
    
    ld best=1e18; vector<Part> bestPath;
    // two sides: sign combos
    for(int sA=-1;sA<=1;sA+=2)for(int sB=-1;sB<=1;sB+=2){
        ld tA=angA+sA*alA; // tangent point angle on circle from A
        ld tB=angB+sB*alB;
        ld TAx=xc+rc*cos(tA), TAy=yc+rc*sin(tA);
        ld TBx=xc+rc*cos(tB), TBy=yc+rc*sin(tB);
        // arc direction: from tA to tB. tangent from A: travel direction along circle determined by sA
        // if sA=+1, going counterclockwise? We need arc that goes the consistent way.
        // arc sweep: we choose to go in direction such that we travel along boundary
        // determine arc from tA to tB in direction dirSign
        for(int dir=-1;dir<=1;dir+=2){
            // arc angle traveling 'dir' from tA to tB
            ld arc;
            if(dir>0) arc=norm(tB-tA); else arc=norm(tA-tB);
            // tangent must be valid: leaving A tangentially in dir of travel
            // we'll just compute path and check validity loosely, pick min
            // chord shortcut: replace middle portion of arc by chord of length up to D
            // chord subtends phi: chord=2r sin(phi/2)<=D
            ld maxPhi = (D>=2*rc)? PI : 2*asin(min(1.0,D/(2*rc)));
            ld usePhi=min(arc,maxPhi);
            // place symmetric: arc reduces by usePhi, add chord
            ld chord=2*rc*sin(usePhi/2);
            // midpoint placement
            ld a1 = (dir>0)? tA : tA; 
            // points where chord starts/ends
            ld remArc=arc-usePhi;
            ld p1ang = tA + dir*(remArc/2);
            ld p2ang = tA + dir*(remArc/2+usePhi);
            ld P1x=xc+rc*cos(p1ang),P1y=yc+rc*sin(p1ang);
            ld P2x=xc+rc*cos(p2ang),P2y=yc+rc*sin(p2ang);
            ld total=lenTA+ (remArc)*rc + chord + lenTB;
            // verify tangent segments don't cross circle (they shouldn't by construction)
            // build path
            vector<Part> path;
            path.push_back({'S',xa,ya,TAx,TAy});
            if(remArc/2>1e-9) path.push_back({'A',TAx,TAy,P1x,P1y});
            if(chord>1e-9) path.push_back({'S',P1x,P1y,P2x,P2y});
            if(remArc/2>1e-9) path.push_back({'A',P2x,P2y,TBx,TBy});
            path.push_back({'S',TBx,TBy,xb,yb});
            // check chord fly len <=D: chord through interior
            ld fly=chordLen(P1x,P1y,P2x,P2y);
            if(fly>D+1e-6) continue;
            if(total<best){best=total;bestPath=path;}
        }
    }
    printf("%d\n",(int)bestPath.size());
    for(auto&p:bestPath)
        printf("%c %.8f %.8f %.8f %.8f\n",p.t,p.x1,p.y1,p.x2,p.y2);
    return 0;
}
