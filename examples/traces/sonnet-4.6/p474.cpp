#include <bits/stdc++.h>
using namespace std;
int main(){
    int t;scanf("%d",&t);
    while(t--){
        int n,m;scanf("%d%d",&n,&m);
        unordered_map<long long,int> cover;
        cover.reserve(1<<20);
        bool bad=false;
        auto encode=[&](int i,int j,int tp)->long long{
            return ((long long)i*25001+(long long)j)*2+tp;
        };
        vector<array<int,6>> tris(m);
        for(auto&a:tris)scanf("%d%d%d%d%d%d",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5]);
        for(auto&a:tris){
            int x1=a[0],y1=a[1],x2=a[2],y2=a[3],x3=a[4],y3=a[5];
            // find right angle vertex
            // cathetuses parallel to axes means two vertices share x and two share y
            // find the right angle: it's where both legs meet
            // legs are axis-parallel so right angle vertex shares x with one and y with other
            int rx,ry,k;
            bool valid=false;
            int pts[3][2]={{x1,y1},{x2,y2},{x3,y3}};
            for(int i=0;i<3&&!valid;i++){
                int a0=pts[i][0],b0=pts[i][1];
                int j=(i+1)%3,k2=(i+2)%3;
                int a1=pts[j][0],b1=pts[j][1];
                int a2=pts[k2][0],b2=pts[k2][1];
                // right angle at i if legs go to j and k2 axis-parallel
                if(a0==a1&&b0==b2){
                    int kx=b1-b0,ky=a2-a0;
                    if(kx==ky&&kx>0){rx=a0;ry=b0;k=kx;valid=true;}
                    else if(kx==-ky&&kx<0){rx=a0;ry=b0+kx;k=-kx;valid=true;}// hmm
                }
                if(a0==a2&&b0==b1){
                    int kx=b2-b0,ky=a1-a0;
                    if(kx==ky&&kx>0){rx=a0;ry=b0;k=kx;valid=true;}
                }
            }
            if(!valid){bad=true;continue;}
            // determine type: up=(rx,ry),(rx+k,ry),(rx,ry+k) or down=(rx+k,ry),(rx,ry+k),(rx+k,ry+k) right angle at rx+k,ry+k
            // Actually let me redo detection properly
            // skip for now - redo below
            (void)rx;(void)ry;(void)k;
        }
        // redo properly
        bad=false;
        cover.clear();
        for(auto&a:tris){
            int px[3]={a[0],a[2],a[4]},py[3]={a[1],a[3],a[5]};
            // find right angle vertex (where two axis-parallel legs meet)
            int ri=-1;
            for(int i=0;i<3;i++){
                int j=(i+1)%3,kk=(i+2)%3;
                // legs from i to j and i to kk must be axis-parallel
                if((px[i]==px[j]&&py[i]==py[kk])||(py[i]==py[j]&&px[i]==px[kk])){
                    // check equal length
                    int l1=abs(px[i]-px[j])+abs(py[i]-py[j]);
                    int l2=abs(px[i]-px[kk])+abs(py[i]-py[kk]);
                    if(l1==l2&&l1>0){ri=i;break;}
                }
            }
            if(ri<0){bad=true;break;}
            int ix=px[ri],iy=py[ri];
            int j=(ri+1)%3,kk=(ri+2)%3;
            // make sure leg to j is vertical and leg to kk is horizontal (or swap)
            if(py[ri]==py[j]){swap(j,kk);}// now px[ri]==px[j] (vertical leg)
            // vertical leg: ix fixed, goes from iy to py[j]
            // horizontal leg: iy fixed, goes from ix to px[kk]
            if(px[ri]!=px[j]){bad=true;break;}
            if(py[ri]!=py[kk]){bad=true;break;}
            int dy=py[j]-iy,dx=px[kk]-ix;
            if(abs(dy)!=abs(dx)||dy==0){bad=true;break;}
            // type: 
            // "up-left": right angle at bottom-left, dx>0,dy>0 -> up type
            // dx>0,dy<0: right angle at top-left
            // dx<0,dy>0: right angle at bottom-right  
            // dx<0,dy<0: right angle at top-right
            int k2=abs(dx);
            // normalize to bottom-left corner of bounding box
            int bx=min(ix,ix+dx),by=min(iy,iy+dy);
            // check inside large triangle: all points satisfy x>=0,y>=0,x+y<=n
            bool inside=true;
            for(int p=0;p<3;p++){
                if(px[p]<0||py[p]<0||px[p]+py[p]>n)inside=false;
            }
            if(!inside){bad=true;break;}
            // determine triangle type: 0=up(right angle at min corner, hyp goes up-right), 1=down
            // up: vertices (bx,by),(bx+k,by),(bx,by+k) - right angle at (bx,by), dx>0,dy>0
            // down: vertices (bx+k,by),(bx,by+k),(bx+k,by+k) - right angle at (bx+k,by+k)
            int tp;
            if(dx>0&&dy>0){// right angle at (ix,iy)=(bx,by), up type
                tp=0;
            } else if(dx<0&&dy<0){// right angle at (ix,iy)=(bx+k,by+k), down type
                tp=1;
            } else {
                bad=true;break;// diagonal goes wrong way
            }
            // now mark unit triangles
            // up triangle of size k at (bx,by): covers unit triangles
            // unit up triangle (i,j): vertices (i,j),(i+1,j),(i,j+1)
            // unit down triangle (i,j): vertices (i+1,j),(i,j+1),(i+1,j+1)
            // a size-k up triangle at (bx,by) covers:
            // all (i,j) with i>=bx, j>=by, i+j<bx+by+k, both up and down
            // specifically: up(i,j) for bx<=i,by<=j,i+j<=bx+by+k-1
            //               down(i,j) for bx<=i,by<=j,i+j<=bx+by+k-2
            // a size-k down triangle at (bx,by): vertices (bx+k,by),(bx,by+k),(bx+k,by+k)
            // covers: up(i,j) for bx<=i<bx+k, by<=j<by+k, i+j>=bx+by+k
            //         down(i,j) for bx<=i<bx+k, by<=j<by+k, i+j>=bx+by+k-1
            if(tp==0){
                for(int i=bx;i<bx+k2;i++){
                    for(int j=by;j<by+k2&&i+j<bx+by+k2;j++){
                        // up(i,j)
                        auto key=encode(i,j,0);
                        if(++cover[key]>1){bad=true;}
                        // down(i,j) if i+j<bx+by+k2-1
                        if(i+j<bx+by+k2-1){
                            key=encode(i,j,1);
                            if(++cover[key]>1){bad=true;}
                        }
                    }
                }
            } else {
                for(int i=bx;i<bx+k2;i++){
                    for(int j=by;j<by+k2;j++){
                        if(i+j>=bx+by+k2){
                            // up(i,j)
                            auto key=encode(i,j,0);
                            if(++cover[key]>1){bad=true;}
                        }
                        if(i+j>=bx+by+k2-1){
                            // down(i,j)
                            auto key=encode(i,j,1);
                            if(++cover[key]>1){bad=true;}
                        }
                    }
                }
            }
            if(bad)break;
        }
        if(!bad){
            // check total coverage = n^2
            if((long long)cover.size()!=(long long)n*n){bad=true;}
        }
        printf("%s\n",bad?"NO":"YES");
    }
}
