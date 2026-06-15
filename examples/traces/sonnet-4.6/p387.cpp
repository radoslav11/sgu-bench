#include<bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld EPS=1e-9;
typedef pair<ld,ld> pdd;

struct Pt{ld x,y;
Pt(ld x=0,ld y=0):x(x),y(y){}
Pt operator-(Pt o){return{x-o.x,y-o.y};}
Pt operator+(Pt o){return{x+o.x,y+o.y};}
Pt operator*(ld t){return{x*t,y*t};}
ld dot(Pt o){return x*o.x+y*o.y;}
ld cross(Pt o){return x*o.y-y*o.x;}
ld norm(){return sqrt(x*x+y*y);}
};

Pt rot90(Pt p){return Pt(-p.y,p.x);}
Pt rotk(Pt p,int k){for(int i=0;i<(k%4+4)%4;i++)p=rot90(p);return p;}

struct Seg{Pt a,b;};

// Given segment S (set of points on it), and constraint that rotk(P,k) in SegC
// => P in rotk(SegC, -k) = rotk(SegC, 4-k)
// So candidate segments for P from input segment i, rotation k:
// seg = rotateby(4-k)(input_seg_i)

// Intersect segment [A,B] with half-plane or another segment along same line
// We'll represent valid portions of a segment as parameter intervals [0,1]

// Check if point is on segment (parameterized)
// Project point onto line of segment, get t in [0,1]

bool onSeg(Seg s, Pt p){
    Pt d=s.b-s.a;
    if(d.norm()<EPS)return (p-s.a).norm()<EPS;
    ld t=d.dot(p-s.a)/d.dot(d);
    if(t<-EPS||t>1+EPS)return false;
    Pt q=s.a+d*t;
    return (q-p).norm()<EPS;
}

// For a candidate segment (parameterized as a+t*(b-a), t in interval),
// intersect with constraint: rotk(P,k) must be on some segment from list
// Returns new list of intervals

// We'll work with segments as (Seg, vector of [t0,t1] intervals)

struct CSeg{
    Pt a,b; // base segment
    vector<pdd> ivs; // valid parameter intervals
};

// Intersect interval list with constraint: for point P=a+t*(b-a),
// rotk(P,k) must lie on at least one segment in list segs

vector<pdd> intersectConstraint(CSeg& cs, int k, vector<Seg>& segs){
    // For each interval in cs.ivs, we keep only parts where rotk(P,k) in some seg
    // rotk(P,k) = rotk(a+t*(b-a), k) = rotk(a,k) + t*rotk(b-a,k)
    // This is a linear function of t, so for each seg in segs,
    // the set of t where rotk(P,k) is on seg is an interval (or empty)
    
    Pt ra=rotk(cs.a,k), rd=rotk(cs.b-cs.a,k);
    // point on rotated line: ra + t*rd
    // must lie on seg s: s.a + u*(s.b-s.a), u in [0,1], and same point
    // ra + t*rd = s.a + u*(s.b-s.a)
    // 2 equations, 2 unknowns (t,u)
    
    // collect valid t-intervals from all segs
    vector<pdd> valid;
    for(auto& s:segs){
        Pt sd=s.b-s.a;
        // ra + t*rd = s.a + u*sd
        // t*rd - u*sd = s.a - ra
        Pt rhs=s.a-ra;
        ld det=rd.cross(sd); // rd.x*sd.y - rd.y*sd.x... wait cross of rd and (-sd)?
        // rd.x*(-sd.y) - rd.y*(-sd.x)? No.
        // t*rdx - u*sdx = rhs.x
        // t*rdy - u*sdy = rhs.y
        // det = rdx*(-sdy) - rdy*(-sdx) = -rdx*sdy+rdy*sdx = -(rdx*sdy-rdy*sdx) = -rd.cross(sd)
        // Hmm let me redo:
        // [rdx, -sdx][t]   [rhs.x]
        // [rdy, -sdy][u] = [rhs.y]
        // det = rdx*(-sdy) - (-sdx)*rdy = -rdx*sdy + sdx*rdy = -(rdx*sdy - rdy*sdx) = -rd.cross(sd)
        // Wait rd.cross(sd) = rdx*sdy - rdy*sdx
        ld D = rd.x*(-sd.y) - (-sd.x)*rd.y; // = -rdx*sdy + sdx*rdy = -rd.cross(sd)... 
        // Actually D = rd.x*sd.y... let me just compute directly
        // Matrix [[rdx,-sdx],[rdy,-sdy]], det = rdx*(-sdy)-(-sdx)*rdy = -rdx*sdy+sdx*rdy
        D = -rd.x*sd.y + sd.x*rd.y;
        if(abs(D)<EPS){
            // parallel: check if collinear
            // if ra is on line of s (or offset is 0)
            // check rhs is parallel to sd (or rd)
            // collinear if rhs cross sd == 0
            if(abs(rhs.cross(sd))<EPS && abs(rhs.cross(rd))<EPS){
                // collinear, find t range
                // ra + t*rd on segment s: s.a + u*sd
                // if rd is zero vector?
                if(rd.norm()<EPS){
                    // P is fixed point, check if on s
                    // u = ?
                    ld u;
                    if(sd.norm()>EPS){
                        u=sd.dot(ra-s.a)/sd.dot(sd);
                    } else {
                        u=0;
                        if((ra-s.a).norm()>EPS) continue;
                    }
                    if(u>=-EPS&&u<=1+EPS) valid.push_back({-1e18,1e18});
                } else {
                    // parameterize: ra+t*rd on s means
                    // s.a + u*sd = ra+t*rd
                    // if sd~rd direction: u = t*(rd.norm/sd.norm) + offset
                    // find t where u in [0,1]
                    // u = dot(ra+t*rd - s.a, sd)/dot(sd,sd)
                    // = (dot(ra-s.a,sd) + t*dot(rd,sd))/dot(sd,sd)
                    ld sdd=sd.dot(sd);
                    if(sdd<EPS){ // s is a point
                        // check if ra is that point (for some t)
                        // ra+t*rd = s.a => t = dot(s.a-ra,rd)/dot(rd,rd)
                        ld rdd=rd.dot(rd);
                        ld t0=rd.dot(s.a-ra)/rdd;
                        // check
                        Pt q=cs.a+rotk(Pt(cs.a.x+(cs.b.x-cs.a.x)*t0,cs.a.y+(cs.b.y-cs.a.y)*t0),-k); // hmm this is getting complicated
                        valid.push_back({t0,t0});
                    } else {
                        ld u0=sd.dot(ra-s.a)/sdd;
                        ld du=sd.dot(rd)/sdd;
                        // u = u0 + t*du in [0,1]
                        ld t0,t1;
                        if(abs(du)<EPS){
                            if(u0>=-EPS&&u0<=1+EPS) valid.push_back({-1e18,1e18});
                        } else {
                            t0=(0-u0)/du; t1=(1-u0)/du;
                            if(t0>t1)swap(t0,t1);
                            valid.push_back({t0,t1});
                        }
                    }
                }
            }
            // else no intersection
        } else {
            ld t0=(rhs.x*(-sd.y)-(-sd.x)*rhs.y)/D;
            ld u0=(rd.x*rhs.y - rhs.x*rd.y)/D; // Cramer's rule
            // Check: u0 = (rdx*rhsy - rhsx*rdy)/(-det_original)?
            // Actually det of [[rdx,-sdx],[rdy,-sdy]] = -rdx*sdy+sdx*rdy = D
            // t = |rhs.x -sdx; rhs.y -sdy| / D = (rhs.x*(-sdy)-(-sdx)*rhs.y)/D = (-rhs.x*sdy+sdx*rhs.y)/D
            // u = |rdx rhs.x; rdy rhs.y| / D = (rdx*rhs.y - rhs.x*rdy)/D
            t0 = (-rhs.x*sd.y + sd.x*rhs.y)/D;
            u0 = (rd.x*rhs.y - rhs.x*rd.y)/D;
            if(u0>=-EPS&&u0<=1+EPS){
                valid.push_back({t0,t0});
            }
        }
    }
    
    // Now intersect cs.ivs with union of valid intervals
    // Sort and merge valid intervals
    sort(valid.begin(),valid.end());
    vector<pdd> merged;
    for(auto& iv:valid){
        if(merged.empty()||iv.first>merged.back().second+EPS)
            merged.push_back(iv);
        else
            merged.back().second=max(merged.back().second,iv.second);
    }
    
    // Intersect cs.ivs with merged
    vector<pdd> res;
    for(auto& civ:cs.ivs){
        for(auto& miv:merged){
            ld lo=max(civ.first,miv.first);
            ld hi=min(civ.second,miv.second);
            if(lo<=hi+EPS) res.push_back({lo,hi});
        }
    }
    return res;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin>>n;
    vector<Seg> segs(n);
    for(int i=0;i<n;i++){
        ld x1,y1,x2,y2;
        cin>>x1>>y1>>x2>>y2;
        segs[i]={Pt(x1,y1),Pt(x2,y2)};
    }
    
    // Generate candidate segments for first base P:
    // For each input segment i, rotation k (0..3):
    // P rotated by k must be on segment i
    // => P is on segment i rotated by (4-k)%4
    
    vector<CSeg> candidates;
    for(int i=0;i<n;i++){
        for(int k=0;k<4;k++){
            int rk=(4-k)%4;
            Pt ra=rotk(segs[i].a,rk);
            Pt rb=rotk(segs[i].b,rk);
            CSeg cs;
            cs.a=ra; cs.b=rb;
            cs.ivs={{0.0,1.0}};
            candidates.push_back(cs);
        }
    }
    
    // For each candidate, intersect with all n constraints
    // Constraint j: rotk(P,k) must be on segs[j] for some k in {0,1,2,3}
    // i.e., P must be on segs[j] rotated by (4-k)%4 for some k
    // i.e., P in union_{k=0}^{3} rotk(segs[j], (4-k)%4)
    // = union_{m=0}^{3} rotk(segs[j], m)
    
    // But the constraint is that the k-th base (rotk(P,k)) is on segs[j]
    // So constraint j means: exists k s.t. rotk(P,k) in segs[j]
    // = exists m s.t. P in rotk(segs[j], (4-... ))
    // rotk(P,k) in segs[j] <=> P in {Q: rotk(Q,k) in segs[j]} = rotk(segs[j], 4-k) rotated back
    // rotk(Q,k)=R => Q=rotk(R,4-k), so {Q: rotk(Q,k) in segs[j]} = {rotk(R,4-k): R in segs[j]} = rotk(segs[j], (4-k)%4)
    
    // So constraint j: P in union_{k=0}^{3} rotk(segs[j], (4-k)%4) = union_{m=0}^{3} rotk(segs[j], m)
    
    // For each candidate CSeg, apply all n constraints:
    
    // For constraint j, the "allowable set" for P is union of 4 rotated versions of segs[j]
    // We need to intersect candidate interval with this allowable set
    
    // The allowable set restricted to the candidate line is what we computed above
    // But now the segs to check against are the 4 rotations of segs[j]
    
    vector<CSeg> result;
    
    for(auto& cand : candidates){
        // Check if segment is degenerate
        Pt d=cand.b-cand.a;
        
        for(int j=0;j<n&&!cand.ivs.empty();j++){
            // Build the 4 rotated versions of segs[j]
            vector<Seg> rotsegs;
            for(int m=0;m<4;m++){
                rotsegs.push_back({rotk(segs[j].a,m), rotk(segs[j].b,m)});
            }
            // Intersect cand with constraint: P in union of rotsegs
            // But we need: rotk(P,k) in segs[j] for some k
            // which is P in rotk(segs[j], (4-k)%4) for some k
            // = P in union_m rotk(segs[j], m)
            // So we check if P lies on one of the rotsegs (as a point on segment)
            
            // We'll use intersectConstraint but with k=0 and rotsegs
            // Actually intersectConstraint takes k and applies rotk(P,k) in segs
            // We want P (k=0) in union of rotsegs
            // But the rotsegs are not input segs, they're rotated
            // Let me just directly compute: for each rotseg, find t-interval on candidate where P is on rotseg
            
            // Direct approach:
            vector<pdd> valid_ivs;
            for(auto& rs : rotsegs){
                Pt sd=rs.b-rs.a;
                Pt rhs=rs.a-cand.a;
                // cand.a + t*d = rs.a + u*sd
                // t*d - u*sd = rs.a - cand.a = ... wait
                // cand.a + t*d = rs.a + u*sd
                // t*d.x - u*sd.x = rhs.x... wait rhs = rs.a - cand.a
                // t*d.x - u*sd.x = rhs.x => nope
                // cand.a + t*d = rs.a + u*sd
                // t*d.x = rhs.x + u*sd.x => hmm
                // Let me redo: cand.a + t*d - rs.a = u*sd
                // (cand.a - rs.a) + t*d = u*sd
                // Let lhs = cand.a - rs.a (= -rhs from above... wait rhs=rs.a-cand.a so -rhs=cand.a-rs.a)
                // (-rhs) + t*d = u*sd
                // t*d.x - u*sd.x = rhs.x
                // t*d.y - u*sd.y = rhs.y
                // [d.x, -sd.x][t] = [rhs.x]
                // [d.y, -sd.y][u]   [rhs.y]
                // D = d.x*(-sd.y) - (-sd.x)*d.y = -d.x*sd.y + sd.x*d.y
                ld D = -d.x*sd.y + sd.x*d.y;
                if(abs(D)<EPS){
                    // parallel
                    // collinear? check rhs cross d == 0 (rhs parallel to d)
                    // rhs = rs.a - cand.a
                    if(abs(rhs.cross(d))<EPS){ // collinear (or d=0)
                        if(d.norm()<EPS){
                            // cand is a point, check if on rs
                            if(sd.norm()<EPS){
                                if((cand.a-rs.a).norm()<EPS) valid_ivs.push_back({-1e18,1e18});
                            } else {
                                ld u=sd.dot(cand.a-rs.a)/sd.dot(sd);
                                Pt q=rs.a+sd*u;
                                if((q-cand.a).norm()<EPS && u>=-EPS && u<=1+EPS)
                                    valid_ivs.push_back({-1e18,1e18});
                            }
                        } else {
                            if(sd.norm()<EPS){
                                // rs is a point, find t where cand.a+t*d = rs.a
                                ld t0=d.dot(rs.a-cand.a)/d.dot(d);
                                Pt q=cand.a+d*t0;
                                if((q-rs.a).norm()<EPS) valid_ivs.push_back({t0,t0});
                            } else {
                                // both nonzero, collinear
                                // u(t) = dot(cand.a+t*d - rs.a, sd)/dot(sd,sd)
                                ld sdd=sd.dot(sd);
                                ld u0=sd.dot(cand.a-rs.a)/sdd;
                                ld du=sd.dot(d)/sdd;
                                // u = u0 + t*du in [0,1]
                                if(abs(du)<EPS){
                                    if(u0>=-EPS&&u0<=1+EPS) valid_ivs.push_back({-1e18,1e18});
                                } else {
                                    ld ta=(0-u0)/du, tb=(1-u0)/du;
                                    if(ta>tb)swap(ta,tb);
                                    valid_ivs.push_back({ta,tb});
                                }
                            }
                        }
                    }
                } else {
                    ld t0=(-rhs.x*sd.y+sd.x*rhs.y)/D;
                    ld u0=(d.x*rhs.y - rhs.x*d.y)/D; // Cramer
                    // Verify u0 formula: 
                    // u = |d.x rhs.x; d.y rhs.y| / D = (d.x*rhs.y - rhs.x*d.y)/D
                    if(u0>=-EPS&&u0<=1+EPS){
                        valid_ivs.push_back({t0,t0});
                    }
                }
            }
            
            // Sort and merge valid_ivs
            sort(valid_ivs.begin(),valid_ivs.end());
            vector<pdd> merged;
            for(auto& iv:valid_ivs){
                if(merged.empty()||iv.first>merged.back().second+EPS)
                    merged.push_back(iv);
                else
                    merged.back().second=max(merged.back().second,iv.second);
            }
            
            // Intersect cand.ivs with merged
            vector<pdd> new_ivs;
            for(auto& civ:cand.ivs){
                for(auto& miv:merged){
                    ld lo=max(civ.first,miv.first);
                    ld hi=min(civ.second,miv.second);
                    if(lo<hi+EPS) new_ivs.push_back({lo,hi});
                }
            }
            cand.ivs=new_ivs;
        }
        if(!cand.ivs.empty()) result.push_back(cand);
    }
    
    // Now compute expected area = E[2*r^2] where r=|P|=|cand.a+t*(cand.b-cand.a)|
    // Weighted by length measure
    // Total length and total (length-weighted 2r^2)
    
    ld total_len=0, total_area=0;
    
    for(auto& cs:result){
        Pt d=cs.b-cs.a;
        ld dlen=d.norm();
        for(auto& iv:cs.ivs){
            ld t0=max(iv.first,(ld)0.0-EPS), t1=min(iv.second,(ld)1.0+EPS);
            t0=max(t0,(ld)0); t1=min(t1,(ld)1);
            if(t0>t1+EPS) continue;
            // P(t) = cs.a + t*d
            // |P(t)|^2 = (ax+t*dx)^2 + (ay+t*dy)^2
            // = ax^2+ay^2 + 2t(ax*dx+ay*dy) + t^2*(dx^2+dy^2)
            // = A + B*t + C*t^2
            ld A=cs.a.x*cs.a.x+cs.a.y*cs.a.y;
            ld B=2*(cs.a.x*d.x+cs.a.y*d.y);
            ld C=d.x*d.x+d.y*d.y;
            // integral of 2*(A+B*t+C*t^2)*dlen dt from t0 to t1
            // = 2*dlen * [A*t + B/2*t^2 + C/3*t^3] from t0 to t1
            ld seg_len=dlen*(t1-t0);
            total_len+=seg_len;
            auto F=[&](ld t)->ld{return A*t+B/2*t*t+C/3*t*t*t;};
            total_area+=2*dlen*(F(t1)-F(t0));
        }
    }
    
    ld ans = (total_len<EPS) ? 0 : total_area/total_len;
    
    cout<<fixed<<setprecision(10)<<ans<<endl;
    
    return 0;
}
