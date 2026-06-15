#include <bits/stdc++.h>
using namespace std;

int R, C;
char arrows[30][31];
char letters[30][31];
int nxt[900]; // next cell index, -1 if off grid
char letter[900];

int cellId(int r, int c) { return r * C + c; }

// For a query, simulate matching. Returns starting cell or -1.
// path_pos: current cell (-1 = off grid)
// Try to match string s[0..len-1] starting from path_pos, update path_pos
bool matchSegment(const string& seg, int reps, int& path_pos) {
    int L = seg.size();
    for (int rep = 0; rep < reps; rep++) {
        for (int i = 0; i < L; i++) {
            if (path_pos == -1) return false;
            if (letter[path_pos] != seg[i]) return false;
            path_pos = nxt[path_pos];
        }
    }
    return true;
}

int main() {
    scanf("%d %d", &R, &C);
    for (int i = 0; i < R; i++) scanf("%s", arrows[i]);
    for (int i = 0; i < R; i++) scanf("%s", letters[i]);
    for (int id = 0; id < R*C; id++) {
        int r = id/C, c = id%C;
        letter[id] = letters[r][c];
        int nr=r,nc=c;
        char a = arrows[r][c];
        if(a=='<') nc--; else if(a=='>') nc++; else if(a=='^') nr--; else nr++;
        if(nr<0||nr>=R||nc<0||nc>=C) nxt[id]=-1;
        else nxt[id]=cellId(nr,nc);
    }
    int Q; scanf("%d",&Q);
    while(Q--) {
        char buf[2001]; scanf("%s",buf);
        string s(buf);
        // Parse into segments: (text, reps)
        vector<pair<string,long long>> segs;
        int i=0,n=s.size();
        while(i<n){
            if(s[i]=='('){int j=s.find(')',i);string f=s.substr(i+1,j-i-1);i=j+1;long long k=0;while(i<n&&isdigit(s[i]))k=k*10+(s[i++]-'0');segs.push_back({f,k});}
            else{string f="";while(i<n&&s[i]!='('&&!isdigit(s[i]))f+=s[i++];if(i<n&&isdigit(s[i])){long long k=0;while(i<n&&isdigit(s[i]))k=k*10+(s[i++]-'0');segs.push_back({f,k});}else segs.push_back({f,1});}
        }
        int ans=-1;
        for(int st=0;st<R*C&&ans==-1;st++){
            int pp=st;
            bool ok=true;
            for(auto&[seg,reps]:segs){if(!ok)break;int L=seg.size();if(L==0)continue;// handle large reps with cycle detection
            int start_pp=pp;vector<int> states;map<int,int> seen;long long rem=reps;while(rem>0){if(pp==-1){ok=false;break;}if(seen.count(pp)){int cyc=states.size()-seen[pp];long long skip=(rem/cyc)*cyc;rem-=skip;if(rem==0)break;seen.clear();states.clear();}seen[pp]=states.size();states.push_back(pp);// match one seg
            int tmp=pp;for(int ci=0;ci<L;ci++){if(tmp==-1){ok=false;break;}if(letter[tmp]!=seg[ci]){ok=false;break;}tmp=nxt[tmp];}if(!ok)break;pp=tmp;rem--;}}
            if(ok)ans=st;
        }
        if(ans==-1)printf("NO\n");
        else printf("YES (%d,%d)\n",ans/C+1,ans%C+1);
    }
}
