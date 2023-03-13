#include <bits/stdc++.h>
using namespace std;
#define REP(i,n) for(int i=0;i<(n);i++)
#define ALL(v) v.begin(),v.end()
template<typename T> bool chmax(T &x, const T &y) {return (x<y)?(x=y,true):false;};
template<typename T> bool chmin(T &x, const T &y) {return (x>y)?(x=y,true):false;};

const bool DEBUG=0;
const double TL=1.9;

const int N=10, M=100, K=3;
vector<int> F(M);
vector<int> IDX(M);

const char DIR[4]={'F','B','L','R'};
const int dx[4]={1,0,-1,0}, dy[4]={0,1,0,-1};

unsigned int xs(){
    static unsigned int x=123456789, y=362436069, z=521288629, w=88675123;
    unsigned int t=(x^(x<<11));
    x=y; y=z; z=w;
    return (w=(w^(w>>19))^(t^(t>>8)));
}

void inputfromfile(const int& fnum){
    string name="";
    if(fnum<1000) name+='0';
    if(fnum<100) name+='0';
    if(fnum<10) name+='0';
    name+=to_string(fnum);
    name+=".txt";
    ifstream ifs(("/mnt/c/CP/work/AHC015in/"+name).c_str(),ios::in);
    assert(ifs);
    cout << name << endl;
    for(int i=0;i<M;i++) ifs >> F[i];
    for(int i=0;i<M;i++) ifs >> IDX[i];
}

void showbox(const vector<int>& box){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cout << ((box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2)) << " \n"[j==N-1];
        }
    }
}

int calcbox(const vector<int>& box){
    int ret=0;
    vector<int> used(N,0);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if((used[i]>>j)&1)
                continue;
            used[i]|=1<<j;
            int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
            if(m==0)
                continue;
            queue<int> q;
            q.push(i*N+j);
            int cnt=0;
            while(!q.empty()){
                cnt++;
                int f=q.front(); q.pop();
                int x=f/N, y=f%N;
                for(int k=0;k<4;k++){
                    int tx=x+dx[k], ty=y+dy[k];
                    if(tx<0||ty<0||tx>=N||ty>=N)
                        continue;
                    if((used[tx]>>ty)&1)
                        continue;
                    int tm=(box[tx]&(3<<((N-ty-1)*2)))>>((N-ty-1)*2);
                    if(m==tm){
                        used[tx]|=1<<ty;
                        q.push(tx*N+ty);
                    }
                }
            }
            ret+=cnt*cnt;
        }
    }
    return ret;
}

int calcbox2(const vector<int>& box){
    int ret=0;
    vector<int> used(N,0);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if((used[i]>>j)&1)
                continue;
            used[i]|=1<<j;
            int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
            if(m==0)
                continue;
            queue<int> q;
            q.push(i*N+j);
            int cnt=0;
            while(!q.empty()){
                cnt++;
                int f=q.front(); q.pop();
                int x=f/N, y=f%N;
                for(int k=0;k<4;k++){
                    int tx=x+dx[k], ty=y+dy[k];
                    if(tx<0||ty<0||tx>=N||ty>=N)
                        continue;
                    if((used[tx]>>ty)&1)
                        continue;
                    int tm=(box[tx]&(3<<((N-ty-1)*2)))>>((N-ty-1)*2);
                    if(m==tm){
                        used[tx]|=1<<ty;
                        q.push(tx*N+ty);
                    }
                }
            }
            ret+=cnt*cnt;
        }
    }
    const double k=4;
    int admax=-1e9;
    for(int p=1;p<=3;p++)for(int q=1;q<=3;q++){
        if(p==q) continue;
        int sum;
        sum=0;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
                if(m==p)
                    sum+=(N*2-2-(i+j));
                else if(m==q)
                    sum+=((i+j));
            }
        }
        chmax(admax,sum);
        sum=0;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
                if(m==p)
                    sum+=(i-j+N-1);
                else if(m==q)
                    sum+=(j-i+N-1);
            }
        }
        chmax(admax,sum);
    }
    return ret+k*admax;
}

void tilt(vector<int>& box, int dir){
    if(dir==0){
        vector<int> nbox(N,0);
        vector<int> idx(N,0);
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                int m=box[i];
                m&=(3<<(j*2));
                if(m)
                    nbox[idx[j]++]|=m;
            }
        }
        box=nbox;
    }
    else if(dir==1){
        vector<int> nbox(N,0);
        vector<int> idx(N,N-1);
        for(int i=N-1;i>=0;i--){
            for(int j=0;j<N;j++){
                int m=box[i];
                m&=(3<<(j*2));
                if(m)
                    nbox[idx[j]--]|=m;
            }
        }
        box=nbox;
    }
    else if(dir==2){
        for(int i=0;i<N;i++){
            int x=0, cnt=0;
            for(int j=0;j<N;j++){
                int m=box[i];
                m&=(3<<((N-j-1)*2));
                m>>=(N-j-1)*2;
                if(m){
                    x<<=2;
                    x|=m;
                    cnt++;
                }
            }
            x<<=(N-cnt)*2;
            box[i]=x;
        }
    }
    else{
        for(int i=0;i<N;i++){
            int x=0, cnt=0;
            for(int j=0;j<N;j++){
                int m=box[i];
                m&=(3<<((N-j-1)*2));
                m>>=(N-j-1)*2;
                if(m){
                    x<<=2;
                    x|=m;
                    cnt++;
                }
            }
            box[i]=x;
        }
    }
}

double dfs(const vector<int>& box, int predir, int t, int depth, int maxdepth){
    if(depth==maxdepth||t==M){
        if(t<80)
            return calcbox2(box);
        else
            return calcbox(box);
    }
    double ret=0;
    if(predir==0){
        for(int j=0;j<N;j++){
            for(int i=N-1;i>=0;i--){
                int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
                if(m) break;
                double mx=0;
                for(int dir=0;dir<4;dir++){
                    vector<int> nbox=box;
                    nbox[i]|=F[t]<<((N-j-1)*2);
                    tilt(nbox,dir);
                    chmax(mx,dfs(nbox,dir,t+1,depth+1,maxdepth));
                }
                ret+=mx;
            }
        }
    }
    else if(predir==1){
        for(int j=0;j<N;j++){
            for(int i=0;i<N;i++){
                int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
                if(m) break;
                double mx=0;
                for(int dir=0;dir<4;dir++){
                    vector<int> nbox=box;
                    nbox[i]|=F[t]<<((N-j-1)*2);
                    tilt(nbox,dir);
                    chmax(mx,dfs(nbox,dir,t+1,depth+1,maxdepth));
                }
                ret+=mx;
            }
        }
    }
    else if(predir==2){
        for(int i=0;i<N;i++){
            for(int j=N-1;j>=0;j--){
                int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
                if(m) break;
                double mx=0;
                for(int dir=0;dir<4;dir++){
                    vector<int> nbox=box;
                    nbox[i]|=F[t]<<((N-j-1)*2);
                    tilt(nbox,dir);
                    chmax(mx,dfs(nbox,dir,t+1,depth+1,maxdepth));
                }
                ret+=mx;
            }
        }
    }
    else{
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
                if(m) break;
                double mx=0;
                for(int dir=0;dir<4;dir++){
                    vector<int> nbox=box;
                    nbox[i]|=F[t]<<((N-j-1)*2);
                    tilt(nbox,dir);
                    chmax(mx,dfs(nbox,dir,t+1,depth+1,maxdepth));
                }
                ret+=mx;
            }
        }
    }
    return ret/(M-t);
}

int solve(){
    double start=clock();
    if(!DEBUG)
        for(int i=0;i<M;i++) cin >> F[i];
    vector<int> box(N,0);
    int idx;
    for(int t=0;t<M;t++){
        if(!DEBUG)
            cin >> idx;
        else
            idx=IDX[t];
        int cnt=0;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                int m=(box[i]&(3<<((N-j-1)*2)))>>((N-j-1)*2);
                if(m==0){
                    if(++cnt==idx){
                        box[i]|=F[t]<<((N-j-1)*2);
                        break;
                    }
                }
            }
            if(cnt==idx)
                break;
        }
        vector<pair<double,int>> p(4);
        for(int dir=0;dir<4;dir++){
            vector<int> nbox=box;
            tilt(nbox,dir);
            double ret=dfs(nbox,dir,t+1,0,(t<75?1:2));
            //cout << ret << " ";
            p[dir]={ret,dir};
        }
        //cout << endl;
        sort(ALL(p));
        tilt(box,p[3].second);
        if(!DEBUG)
            cout << DIR[p[3].second] << endl;
    }
    vector<int> fcnt(3,0);
    for(int i=0;i<M;i++) fcnt[F[i]-1]++;
    double fcnt2=0;
    for(int i=0;i<3;i++) fcnt2+=fcnt[i]*fcnt[i];
    if(DEBUG)
        showbox(box);
    double score=1e6*calcbox(box)/fcnt2;
    return score;
}

int main(){
    if(!DEBUG){
        solve();
    }
    else{
        long long scoresum=0;
        for(int fnum=0;fnum<100;fnum++){
            inputfromfile(fnum);
            int score=solve();
            scoresum+=score;
            cout << score << endl;
        }
        cout << scoresum << endl;
    }
    return 0;
}
