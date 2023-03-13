#include <bits/stdc++.h>
using namespace std;
#define REP(i,n) for(int i=0;i<(n);i++)
#define ALL(v) v.begin(),v.end()
template<typename T> bool chmax(T &x, const T &y) {return (x<y)?(x=y,true):false;};
template<typename T> bool chmin(T &x, const T &y) {return (x>y)?(x=y,true):false;};

const bool DEBUG=0;

const double TL=4.8;
const double T0=1000;
const double T1=30;

unsigned int xs(){
    static unsigned int x=123456789, y=362436069, z=521288629, w=88675123;
    unsigned int t=(x^(x<<11));
    x=y; y=z; z=w;
    return (w=(w^(w>>19))^(t^(t>>8)));
}

struct Point{
    int x;
    int y;
    bool operator<(const Point& r)const{
        if(x!=r.x)
            return x<r.x;
        else
            return y<r.y;
    };
    bool operator>(const Point& r)const{
        if(x!=r.x)
            return x>r.x;
        else
            return y>r.y;
    };
};

int N, M;
vector<int> X, Y;

void input(){
    cin >> N >> M;
    X.resize(M);
    Y.resize(M);
    for(int i=0;i<M;i++)
        cin >> X[i] >> Y[i];
}
 
void inputfromfile(const int& fnum){
    string name="";
    if(fnum<1000) name+='0';
    if(fnum<100) name+='0';
    if(fnum<10) name+='0';
    name+=to_string(fnum);
    name+=".txt";
    ifstream ifs(("/mnt/c/CP/work/AHC014in/"+name).c_str(),ios::in);
    assert(ifs);
    cout << name << endl;
    ifs >> N >> M;
    X.resize(M);
    Y.resize(M);
    for(int i=0;i<M;i++)
        ifs >> X[i] >> Y[i];
    ifs.close();
}

void output(const vector<vector<Point>>& ans){
    cout << ans.size() << endl;
    for(auto ps:ans){
        for(int i=0;i<4;i++)
            cout << ps[i].x << " " << ps[i].y << " \n"[i==3];
    }
}

void outputtofile(const vector<vector<Point>>& ans, const int& fnum){
    string name="";
    if(fnum<1000) name+='0';
    if(fnum<100) name+='0';
    if(fnum<10) name+='0';
    name+=to_string(fnum);
    name+=".txt";
    ofstream ofs(("/mnt/c/CP/work/AHC014out/"+name).c_str(),ios::out);
    assert(ofs);
    ofs << ans.size() << endl;
    for(auto ps:ans){
        for(int i=0;i<4;i++)
            ofs << ps[i].x << " " << ps[i].y << " \n"[i==3];
    }
    ofs.close();
}

inline int pid(int x, int y){
    //assert(x>=0&&x<N&&y>=0&&y<N);
    return x*N+y;
}

inline int w(int x, int y){
    return (x-(N/2))*(x-(N/2))+(y-(N/2))*(y-(N/2))+1;
}

struct State{
    set<int> rectids;
    set<int> rrectids;
    set<Point> sp;
    vector<vector<int>> lines;/*  0:-, 1:|, 2:/, 3:\  */
    vector<vector<Point>> rects;
    vector<int> rectords;
    vector<set<int>> dependgraph;
    vector<set<int>> rdependgraph;
    vector<int> grid;
    vector<vector<set<Point>>> sgrid;
    vector<set<Point>> candrect;
    vector<pair<int,vector<Point>>> erasedrects;

    int score;
    int linenum;

    int calcf(){
        return score-linenum;
    }
    void sgridadd(int x, int y){
        Point p={x,y};
        sp.insert(p);
        sgrid[0][x].insert(p);
        sgrid[1][y].insert(p);
        sgrid[2][x+y].insert(p);
        sgrid[3][x-y+N-1].insert(p);
        score+=w(x,y);
    }
    void sgriderase(int x, int y){
        Point p={x,y};
        sp.erase(p);
        sgrid[0][x].erase(p);
        sgrid[1][y].erase(p);
        sgrid[2][x+y].erase(p);
        sgrid[3][x-y+N-1].erase(p);
        score-=w(x,y);
    }
    vector<vector<Point>> searchrect(int x, int y){
        Point p0={x,y};
        vector<set<Point>::iterator> sit(4);
        sit[0]=sgrid[0][x].find(p0);
        //assert(sit[0]!=sgrid[0][x].end());
        sit[1]=sgrid[1][y].find(p0);
        //assert(sit[1]!=sgrid[1][y].end());
        sit[2]=sgrid[2][x+y].find(p0);
        //assert(sit[2]!=sgrid[2][x+y].end());
        sit[3]=sgrid[3][x-y+N-1].find(p0);
        //assert(sit[3]!=sgrid[3][x-y+N-1].end());
        vector<vector<Point>> ret;
        if(sit[0]!=sgrid[0][x].begin()&&sit[1]!=sgrid[1][y].begin()){
            Point p1=*prev(sit[0]);
            Point p2=*prev(sit[1]);
            auto it1=sgrid[1][p1.y].find({p1.x,p1.y});
            auto it2=sgrid[0][p2.x].find({p2.x,p2.y});
            Point p3={p2.x,p1.y};
            if((it1==sgrid[1][p1.y].begin()||*prev(it1)<p3)&&(it2==sgrid[0][p2.x].begin()||*prev(it2)<p3)){
                if(lines[0][pid(x,y-1)]+lines[1][pid(x-1,y)]+lines[0][pid(p2.x,p1.y)]+lines[1][pid(p2.x,p1.y)]==0){
                    ret.push_back({p3,p1,p0,p2});
                }
            }
        }
        if(next(sit[0])!=sgrid[0][x].end()&&sit[1]!=sgrid[1][y].begin()){
            Point p1=*next(sit[0]);
            Point p2=*prev(sit[1]);
            auto it1=sgrid[1][p1.y].find({p1.x,p1.y});
            auto it2=sgrid[0][p2.x].find({p2.x,p2.y});
            Point p3={p2.x,p1.y};
            if((it1==sgrid[1][p1.y].begin()||*prev(it1)<p3)&&(next(it2)==sgrid[0][p2.x].end()||*next(it2)>p3)){
                if(lines[0][pid(x,y)]+lines[1][pid(x-1,y)]+lines[0][pid(p2.x,p1.y-1)]+lines[1][pid(p2.x,p1.y)]==0){
                    ret.push_back({p3,p1,p0,p2});
                }
            }
        }
        if(sit[0]!=sgrid[0][x].begin()&&next(sit[1])!=sgrid[1][y].end()){
            Point p1=*prev(sit[0]);
            Point p2=*next(sit[1]);
            auto it1=sgrid[1][p1.y].find({p1.x,p1.y});
            auto it2=sgrid[0][p2.x].find({p2.x,p2.y});
            Point p3={p2.x,p1.y};
            if((next(it1)==sgrid[1][p1.y].end()||*next(it1)>p3)&&(it2==sgrid[0][p2.x].begin()||*prev(it2)<p3)){
                if(lines[0][pid(x,y-1)]+lines[1][pid(x,y)]+lines[0][pid(p2.x,p1.y)]+lines[1][pid(p2.x-1,p1.y)]==0){
                    ret.push_back({p3,p1,p0,p2});
                }
            }
        }
        if(next(sit[0])!=sgrid[0][x].end()&&next(sit[1])!=sgrid[1][y].end()){
            Point p1=*next(sit[0]);
            Point p2=*next(sit[1]);
            auto it1=sgrid[1][p1.y].find({p1.x,p1.y});
            auto it2=sgrid[0][p2.x].find({p2.x,p2.y});
            Point p3={p2.x,p1.y};
            if((next(it1)==sgrid[1][p1.y].end()||*next(it1)>p3)&&(next(it2)==sgrid[0][p2.x].end()||*next(it2)>p3)){
                if(lines[0][pid(x,y)]+lines[1][pid(x,y)]+lines[0][pid(p2.x,p1.y-1)]+lines[1][pid(p2.x-1,p1.y)]==0){
                    ret.push_back({p3,p1,p0,p2});
                }
            }
        }
        if(sit[2]!=sgrid[2][x+y].begin()&&sit[3]!=sgrid[3][x-y+N-1].begin()){
            Point p1=*prev(sit[2]);
            Point p2=*prev(sit[3]);
            auto it1=sgrid[3][p1.x-p1.y+N-1].find({p1.x,p1.y});
            auto it2=sgrid[2][p2.x+p2.y].find({p2.x,p2.y});
            //assert((p2.x+p2.y+p1.x-p1.y)%2==0);
            //assert((p2.x+p2.y-p1.x+p1.y)%2==0);
            int tx=(p2.x+p2.y+p1.x-p1.y)/2;
            int ty=(p2.x+p2.y-p1.x+p1.y)/2;
            Point p3={tx,ty};
            if((it1==sgrid[3][p1.x-p1.y+N-1].begin()||*prev(it1)<p3)&&(it2==sgrid[2][p2.x+p2.y].begin()||*prev(it2)<p3)){
                if(tx>=0&&tx<N&&ty-1>=0&&ty<N){
                    if(lines[2][pid(x-1,y)]+lines[3][pid(x-1,y-1)]+lines[2][pid(tx,ty-1)]+lines[3][pid(tx,ty)]==0){
                        ret.push_back({p3,p1,p0,p2});
                    }
                }
            }
        }
        if(next(sit[2])!=sgrid[2][x+y].end()&&sit[3]!=sgrid[3][x-y+N-1].begin()){
            Point p1=*next(sit[2]);
            Point p2=*prev(sit[3]);
            auto it1=sgrid[3][p1.x-p1.y+N-1].find({p1.x,p1.y});
            auto it2=sgrid[2][p2.x+p2.y].find({p2.x,p2.y});
            //assert((p2.x+p2.y+p1.x-p1.y)%2==0);
            //assert((p2.x+p2.y-p1.x+p1.y)%2==0);
            int tx=(p2.x+p2.y+p1.x-p1.y)/2;
            int ty=(p2.x+p2.y-p1.x+p1.y)/2;
            Point p3={tx,ty};
            if((it1==sgrid[3][p1.x-p1.y+N-1].begin()||*prev(it1)<p3)&&(next(it2)==sgrid[2][p2.x+p2.y].end()||*next(it2)>p3)){
                if(tx-1>=0&&tx<N&&ty-1>=0&&ty<N){
                    if(lines[2][pid(x-1,y)]+lines[3][pid(x,y)]+lines[2][pid(tx,ty-1)]+lines[3][pid(tx-1,ty-1)]==0){
                        ret.push_back({p3,p1,p0,p2});
                    }
                }
            }
        }
        if(sit[2]!=sgrid[2][x+y].begin()&&next(sit[3])!=sgrid[3][x-y+N-1].end()){
            Point p1=*prev(sit[2]);
            Point p2=*next(sit[3]);
            auto it1=sgrid[3][p1.x-p1.y+N-1].find({p1.x,p1.y});
            auto it2=sgrid[2][p2.x+p2.y].find({p2.x,p2.y});
            //assert((p2.x+p2.y+p1.x-p1.y)%2==0);
            //assert((p2.x+p2.y-p1.x+p1.y)%2==0);
            int tx=(p2.x+p2.y+p1.x-p1.y)/2;
            int ty=(p2.x+p2.y-p1.x+p1.y)/2;
            Point p3={tx,ty};
            if((next(it1)==sgrid[3][p1.x-p1.y+N-1].end()||*next(it1)>p3)&&(it2==sgrid[2][p2.x+p2.y].begin()||*prev(it2)<p3)){
                if(tx-1>=0&&tx<N&&ty>=0&&ty<N){
                    if(lines[2][pid(x,y-1)]+lines[3][pid(x-1,y-1)]+lines[2][pid(tx-1,ty)]+lines[3][pid(tx,ty)]==0){
                        ret.push_back({p3,p1,p0,p2});
                    }
                }
            }
        }
        if(next(sit[2])!=sgrid[2][x+y].end()&&next(sit[3])!=sgrid[3][x-y+N-1].end()){
            Point p1=*next(sit[2]);
            Point p2=*next(sit[3]);
            auto it1=sgrid[3][p1.x-p1.y+N-1].find({p1.x,p1.y});
            auto it2=sgrid[2][p2.x+p2.y].find({p2.x,p2.y});
            //assert((p2.x+p2.y+p1.x-p1.y)%2==0);
            //assert((p2.x+p2.y-p1.x+p1.y)%2==0);
            int tx=(p2.x+p2.y+p1.x-p1.y)/2;
            int ty=(p2.x+p2.y-p1.x+p1.y)/2;
            Point p3={tx,ty};
            if((next(it1)==sgrid[3][p1.x-p1.y+N-1].end()||*next(it1)>p3)&&(next(it2)==sgrid[2][p2.x+p2.y].end()||*next(it2)>p3)){
                if(tx-1>=0&&tx<N&&ty-1>=0&&ty<N){
                    if(lines[2][pid(x,y-1)]+lines[3][pid(x,y)]+lines[2][pid(tx-1,ty)]+lines[3][pid(tx-1,ty-1)]==0){
                        ret.push_back({p3,p1,p0,p2});
                    }
                }
            }
        }
        return ret;
    }
    pair<int,Point> recttype(const vector<Point>& p){
        //0 軸に平行 1 ななめ 2 それ以外
        vector<int> xs, ys;
        for(int i=0;i<4;i++){
            xs.push_back(p[i].x);
            ys.push_back(p[i].y);
        }
        sort(ALL(xs));
        sort(ALL(ys));
        if(xs[0]==xs[1]&&xs[1]+1==xs[2]&&xs[2]==xs[3]){
            if(ys[0]==ys[1]&&ys[1]+1==ys[2]&&ys[2]==ys[3]){
                return {0,Point{xs[0],ys[0]}};
            }
        }
        if(xs[0]+1==xs[1]&&xs[1]==xs[2]&&xs[2]+1==xs[3]){
            if(ys[0]+1==ys[1]&&ys[1]==ys[2]&&ys[2]+1==ys[3]){
                return {1,Point{xs[0]+1,ys[0]+1}};
            }
        }
        return {2,Point{-1,-1}};
    }
    vector<pair<int,int>> recttoline(const vector<Point>& p){
        vector<pair<int,int>> ret;
        if(p[0].x==p[1].x||p[0].y==p[1].y){
            for(int i=0;i<4;i++){
                Point f=p[i], t=p[(i+1)%4];
                if(f.x==t.x){
                    if(f.y>t.y)
                        swap(f,t);
                    for(int j=f.y;j<t.y;j++)
                        ret.push_back({0,pid(f.x,j)});
                }
                else{
                    if(f.x>t.x)
                        swap(f,t);
                    for(int j=f.x;j<t.x;j++)
                        ret.push_back({1,pid(j,f.y)});
                }
            }
        }
        else{
            for(int i=0;i<4;i++){
                Point f=p[i], t=p[(i+1)%4];
                if(f.x+f.y==t.x+t.y){
                    if(f.y>t.y)
                        swap(f,t);
                    for(int j=0;j<t.y-f.y;j++)
                        ret.push_back({2,pid(f.x-1-j,f.y+j)});
                }
                else{
                    if(f.x>t.x)
                        swap(f,t);
                    for(int j=0;j<t.x-f.x;j++)
                        ret.push_back({3,pid(f.x+j,f.y+j)});
                }
            }
        }
        return ret;
    }
    void updcandrect(int x, int y){
        int tdx0[4]={-1,-1,0,0}, tdy0[4]={-1,0,0,-1};
        for(int i=0;i<4;i++){
            int tx=x+tdx0[i];
            int ty=y+tdy0[i];
            if(tx<0||tx+1>=N||ty<0||ty+1>=N)
                continue;
            int cnt=0;
            for(int j=0;j<4;j++){
                if(grid[pid(tx-tdx0[j],ty-tdy0[j])]==-1)
                    cnt++;
            }
            if(cnt!=1)
                continue;
            if(lines[0][pid(tx,ty)]==0&&lines[0][pid(tx+1,ty)]==0&&lines[1][pid(tx,ty)]==0&&lines[1][pid(tx,ty+1)]==0)
                candrect[0].insert(Point{tx,ty});
        }
        int tdx1[4]={1,0,-1,0}, tdy1[4]={0,1,0,-1};
        for(int i=0;i<4;i++){
            int tx=x+tdx1[i];
            int ty=y+tdy1[i];
            if(tx-1<0||tx+1>=N||ty-1<0||ty+1>=N)
                continue;
            int cnt=0;
            for(int j=0;j<4;j++){
                if(grid[pid(tx-tdx1[j],ty-tdy1[j])]==-1)
                    cnt++;
            }
            if(cnt!=1)
                continue;
            if(lines[2][pid(tx-1,ty-1)]==0&&lines[2][pid(tx,ty)]==0&&lines[3][pid(tx-1,ty)]==0&&lines[3][pid(tx,ty-1)]==0)
                candrect[1].insert(Point{tx,ty});
        }
    }
    void eraserect(int r){
        erasedrects.clear();
        queue<int> que;
        que.push(r);
        while(!que.empty()){
            int x=que.front();
            que.pop();
            if(rectids.count(x)) continue;
            erasedrects.push_back({rectords[x],rects[x]});
            vector<int> tos;
            for(auto to:dependgraph[x]){
                tos.push_back(to);
            }
            for(auto to:rdependgraph[x]){
                dependgraph[to].erase(x);
            }
            for(auto to:tos){
                que.push(to);
            }
            auto lids=recttoline(rects[x]);
            linenum-=lids.size();
            for(auto [i,j]:lids) lines[i][j]=0;
            sgriderase(rects[x][0].x,rects[x][0].y);
            grid[pid(rects[x][0].x,rects[x][0].y)]=-1;
            rects[x].clear();
            rectids.insert(x);
            rrectids.erase(x);
            rectords[x]=-1;
            dependgraph[x].clear();
            rdependgraph[x].clear();
        }
    }
    int addrect(const vector<Point>& p, int n){
        //まず長方形おけるか判定
        if(p.size()!=4)
            return 0;
        int spcnt=0;
        for(int i=0;i<4;i++){
            if(p[i].x<0||p[i].x>=N||p[i].y<0||p[i].y>=N)
                return 0;
            int gid=grid[pid(p[i].x,p[i].y)];
            if(gid==-1){
                spcnt++;
                if(i!=0)
                    return 0;
            }
        }
        if(spcnt!=1)
            return 0;
        auto lids=recttoline(p);
        for(auto [i,j]:lids){
            if(lines[i][j]!=0)
                return 0;
        }
        //---
        int id=*rectids.begin();
        rectids.erase(id);
        rrectids.insert(id);
        rects[id]=p;
        rectords[id]=n;
        for(int i=0;i<4;i++){
            int gid=grid[pid(p[i].x,p[i].y)];
            if(gid==-1)
                grid[pid(p[i].x,p[i].y)]=id;
            else if(gid>0){
                dependgraph[gid].insert(id);
                rdependgraph[id].insert(gid);
            }
        }
        linenum+=lids.size();
        for(auto [i,j]:lids) lines[i][j]=id;
        sgridadd(p[0].x,p[0].y);
        updcandrect(p[0].x,p[0].y);
        return id;
    }
};

int solve(int fnum){
    if(DEBUG)
        inputfromfile(fnum);
    else
        input();
    double start=clock();
    double Temp=T0;
    State S={
        {},
        {},
        {},
        vector<vector<int>>(4,vector<int>(N*N,0)),
        vector<vector<Point>>(N*N,vector<Point>(0)),
        vector<int>(N*N,-1),
        vector<set<int>>(N*N),
        vector<set<int>>(N*N),
        vector<int>(N*N,-1),
        vector<vector<set<Point>>>(4,vector<set<Point>>(2*N+1)),
        vector<set<Point>>(2),
        vector<pair<int,vector<Point>>>(),
        0,
        0
    };
    for(int i=1;i<N*N;i++){
        S.rectids.insert(i);
    }
    for(int i=0;i<M;i++){
        S.sp.insert({X[i],Y[i]});
        S.grid[pid(X[i],Y[i])]=0;
        S.sgridadd(X[i],Y[i]);
    }
    for(int i=0;i<M;i++){
        S.updcandrect(X[i],Y[i]);
    }
    int maxscore=-1e9;
    vector<vector<Point>> maxrects;
    vector<int> maxrectords;
    int rectidcnt=0;
    int ridxcnt=0;
    for(int rp=0;true;rp++){
        if(rp%200==0){
            //cout << rp << endl;
            double t=(double)(clock()-start)/CLOCKS_PER_SEC/TL;
            if(t>=1.0)
                break;
            Temp=pow(T0,1.0-t)*pow(T1,t);
        }
        int prescore=S.calcf();
        int rid=1;
        if(S.rrectids.size()!=0){
            //int ridx=xs()%S.rrectids.size();
            int ridx=ridxcnt++;
            if(ridx>=(int)S.rrectids.size())
                ridxcnt=0;
            rid=*next(S.rrectids.begin(),ridx);
        }
        S.eraserect(rid);
        auto ers=S.erasedrects;
        set<Point> psp;
        for(auto pss:ers){
            for(int i=0;i<4;i++)
                psp.insert(pss.second[i]);
        }
        for(auto p:psp) S.updcandrect(p.x,p.y);
        vector<int> ads;
        int ts=5;
        for(int j=0;j<ts;j++){
            vector<Point> ps;
            if(j<=3||S.candrect[0].size()+S.candrect[1].size()==0){
                int k=xs()%S.sp.size();
                Point p=*next(S.sp.begin(),k);
                auto ret=S.searchrect(p.x,p.y);
                if(ret.size()==0) continue;
                /*
                sort(ALL(ret),[](const vector<Point>& l, const vector<Point>& r){
                    return w(l[0].x,l[0].y)>w(r[0].x,r[0].y);
                });
                */
                int l=xs()%ret.size();
                ps=ret[l];
                int res=S.addrect(ps,rectidcnt++);
                if(res>0){
                    ads.push_back(res);
                }
            }
            else{
                ts++;
                int r=xs()%2;
                if(S.candrect[r].size()==0) r=!r;
                int k=xs()%S.candrect[r].size();
                Point p=*next(S.candrect[r].begin(),k);
                S.candrect[r].erase(p);
                int tdx[2][4]={{0,0,1,1},{1,0,-1,0}};
                int tdy[2][4]={{0,1,1,0},{0,1,0,-1}};
                int f=0;
                for(int i=0;i<8;i++){
                    int tx=p.x+tdx[r][i%4];
                    int ty=p.y+tdy[r][i%4];
                    if(tx<0||tx>=N||ty<0||ty>=N)
                        break;
                    if(S.grid[pid(tx,ty)]==-1)
                        f++;
                    if(f)
                        ps.push_back({tx,ty});
                    if(ps.size()==4)
                        break;
                }
                if(f!=1) continue;
                int res=S.addrect(ps,rectidcnt++);
                if(res>0){
                    ads.push_back(res);
                }
            }
        }
        int newscore=S.calcf();
        if(chmax(maxscore,S.score)){
            maxrects=S.rects;
            maxrectords=S.rectords;
        }
        if(xs()%1000000<=exp((double)(newscore-prescore)/Temp)*1e6){
            continue;
        }
        else{
            for(auto x:ads) S.eraserect(x);
            sort(ALL(ers));
            for(auto ps:ers) S.addrect(ps.second,ps.first);
        }
    }
    vector<pair<int,vector<Point>>> ansp;
    for(int i=1;i<N*N;i++){
        if(maxrectords[i]!=-1){
            ansp.push_back({maxrectords[i],maxrects[i]});
        }
    }
    sort(ALL(ansp));
    vector<vector<Point>> ans;
    for(auto ps:ansp) ans.push_back(ps.second);
    if(DEBUG)
        outputtofile(ans,fnum);
    else
        output(ans);
    if(DEBUG){
        int gpsum=0;
        for(int i=0;i<N;i++)for(int j=0;j<N;j++){
            gpsum+=w(i,j);
        }
        int finalscore=round(1e6*N*N*maxscore/M/gpsum);
        return finalscore;
    }
    else
        return 0;
}

int main(){
    if(!DEBUG){
        solve(-1);
    }
    else{
        ios::sync_with_stdio(false); cin.tie(nullptr);
        long long scoresum=0;
        for(int fnum=0;fnum<100;fnum++){
            int score=solve(fnum);
            scoresum+=score;
            cout << score << endl;
        }
        cout << scoresum << endl;
    }
    return 0;
}
