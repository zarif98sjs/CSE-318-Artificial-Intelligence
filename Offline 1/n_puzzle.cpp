
/** Which of the favors of your Lord will you deny ? **/

#include<bits/stdc++.h>
using namespace std;

#define LL long long
#define PII pair<int,int>
#define PLL pair<LL,LL>
#define F first
#define S second

#define ALL(x)      (x).begin(), (x).end()
#define READ        freopen("alu.txt", "r", stdin)
#define WRITE       freopen("vorta.txt", "w", stdout)

#ifndef ONLINE_JUDGE
#define DBG(x)      cout << __LINE__ << " says: " << #x << " = " << (x) << endl
#else
#define DBG(x)
#define endl "\n"
#endif

template<class T1, class T2>
ostream &operator <<(ostream &os, pair<T1,T2>&p);
template <class T>
ostream &operator <<(ostream &os, vector<T>&v);
template <class T>
ostream &operator <<(ostream &os, set<T>&v);

inline void optimizeIO()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}

const int nmax = 2e5+7;

vector<vector<int>> getTargetGrid(int n)
{
    vector<vector<int>> grid(n,vector<int>(n,0));

    int cnt = 1;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            grid[i][j] = cnt , cnt++;

    grid[n-1][n-1] = 0;
    return grid;
}

struct Node{
    vector<vector<int>> board;
    vector<vector<int>> parBoard;
    int MOVE;
    int PRI; /// priority

    Node(vector<vector<int>> board,vector<vector<int>> parBoard,int MOVE,int PRI)
    {
        this->board = board;
        this->parBoard = parBoard;
        this->MOVE = MOVE;
        this->PRI = PRI;
    }


};

/// Overload the < operator.
bool operator < (const Node& node1, const Node &node2)
{
    return node1.PRI < node2.PRI;
}

/// Overload the > operator.
bool operator > (const Node& node1, const Node &node2)
{
    return node1.PRI > node2.PRI;
}

vector<int> dr = {-1,0,1,0};
vector<int> dc = {0,1,0,-1};


struct Solver{

    int N;
    vector<vector<int>> board;
    vector<vector<int>> boardTarget;

    map<int,PII> posTarget;
    map<vector<vector<int>>, vector<vector<int>>> PARENT;

    Solver(int N,vector<vector<int>> board,vector<vector<int>> boardTarget)
    {
        this->N = N;
        this->board = board;
        this->boardTarget = boardTarget;

        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                int d = boardTarget[i][j];

                if(d == 0)
                    continue;

                posTarget[d] = {i,j};
            }
        }
    }

    bool isSolvable()
    {
        vector<int>v;
        for(auto row:board)
            for(int el:row)
                if(el != 0)
                    v.push_back(el);

        int inv = 0;

        int n = (int)v.size();
        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++)
            {
                if(v[j] < v[i])
                    inv++;
            }
        }

        /// inversion count done

        if(N%2 == 1)
        {
            /// odd
            if(inv % 2 == 1) return false;
            else return true;
        }
        else
        {
            int whichRow = 0;

            bool gotIt = false;
            for(int i=N-1;i>=0;i--)
            {
                whichRow++;
                for(int j=N-1;j>=0;j--)
                {
                    if(board[i][j] == 0)
                    {
                        gotIt = true;
                        break;
                    }
                }

                if(gotIt)
                    break;
            }

            if(whichRow % 2 == 0 && inv % 2 == 1) return true;
            else if(whichRow % 2 == 1 && inv % 2 == 0) return true;
            else return false;
        }
    }

    bool isOk(int r,int c)
    {
        if (r < 0 || c <0) return false;
        if (r >= N || c >= N) return false;
        return true;
    }

    int h1_hammingDistance(vector<vector<int>> grid)
    {
        int cnt = 0;
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                if(boardTarget[i][j] == 0)
                    continue;

                if(grid[i][j] != boardTarget[i][j])
                    cnt++;
            }
        }

        return cnt;
    }

    int h2_manhattanDistance(vector<vector<int>> grid)
    {
        map<int,PII> posNow;

        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                int d = grid[i][j];
                posNow[d] = {i,j};
            }
        }

        int cnt = 0;
        for(int d=1;d<N*N;d++)
        {
            cnt += abs(posNow[d].F-posTarget[d].F) + abs(posNow[d].S-posTarget[d].S);
        }

        return cnt;
    }

    int linearConflict(vector<vector<int>> grid)
    {
        int cnt = 0;
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                for(int k=j+1;k<N;k++)
                {
                    int d1 = grid[i][j];
                    int d2 = grid[i][k];

                    if(d1 == 0 || d2 == 0)
                        continue;

                    if(i == posTarget[d1].F && i == posTarget[d2].F && posTarget[d1].S > posTarget[d2].S && d1>d2)
                        cnt++;
                }
            }
        }
        return cnt;
    }

    int h3_linearConflict(vector<vector<int>> grid)
    {
        return h2_manhattanDistance(grid) + 2 * linearConflict(grid);
    }


    void solveAStar_1()
    {
        priority_queue< Node,vector<Node>,greater<Node> >pq;
        PARENT.clear();

        vector<vector<int>>par(N,vector<int>(N,0));

        set<vector<vector<int>>>closed;
        pq.push(Node(board,par,0,0));
        PARENT[board] = par;

        int exploredCount = 1;

        while(!pq.empty())
        {
            Node curNode = pq.top();
            pq.pop();

            vector<vector<int>> curBoard = curNode.board;

            if(closed.count(curBoard))
                continue;

            if(curBoard == boardTarget)
            {
                cout<<"Number of moves : "<<curNode.MOVE<<endl;
                break;
            }

            closed.insert(curBoard);

            for(int r=0;r<N;r++)
            {
                for(int c=0;c<N;c++)
                {
                    if(curBoard[r][c] == 0)
                    {
                        for(int i=0;i<4;i++)
                        {
                            int newR = r + dr[i];
                            int newC = c + dc[i];

                            if(isOk(newR,newC))
                            {
                                vector<vector<int>> nextBoard = curBoard;
                                swap(nextBoard[r][c],nextBoard[newR][newC]);

                                int newMOVE = curNode.MOVE + 1;
                                int newPRI = newMOVE + h1_hammingDistance(nextBoard);

                                if(closed.count(nextBoard))
                                    continue;

//                                closed.insert(nextBoard);
                                pq.push(Node(nextBoard,curBoard,newMOVE,newPRI));
                                PARENT[nextBoard] = curBoard;
                                exploredCount++;
                            }
                        }
                    }
                }
            }
        }

        cout<<"Expanded nodes : "<<closed.size()<<endl;
        cout<<"Explored nodes : "<<exploredCount<<endl;

//        printMoves();
    }

    void solveAStar_2()
    {
        priority_queue< Node,vector<Node>,greater<Node> >pq;
        PARENT.clear();

        vector<vector<int>>par(N,vector<int>(N,0));

        set<vector<vector<int>>>closed;
        pq.push(Node(board,par,0,0));
        PARENT[board] = par;

        int exploredCount = 1;

        while(!pq.empty())
        {
            Node curNode = pq.top();
            pq.pop();

            vector<vector<int>> curBoard = curNode.board;

            if(closed.count(curBoard))
                continue;

            if(curBoard == boardTarget)
            {
                cout<<"Number of moves : "<<curNode.MOVE<<endl;
                break;
            }

            closed.insert(curBoard);

            for(int r=0;r<N;r++)
            {
                for(int c=0;c<N;c++)
                {
                    if(curBoard[r][c] == 0)
                    {
                        for(int i=0;i<4;i++)
                        {
                            int newR = r + dr[i];
                            int newC = c + dc[i];

                            if(isOk(newR,newC))
                            {
                                vector<vector<int>> nextBoard = curBoard;
                                swap(nextBoard[r][c],nextBoard[newR][newC]);

                                int newMOVE = curNode.MOVE + 1;
                                int newPRI = newMOVE + h2_manhattanDistance(nextBoard);

                                if(closed.count(nextBoard))
                                    continue;

//                                closed.insert(nextBoard);
                                pq.push(Node(nextBoard,curBoard,newMOVE,newPRI));
                                PARENT[nextBoard] = curBoard;
                                exploredCount++;
                            }
                        }
                    }
                }
            }
        }

        cout<<"Expanded nodes : "<<closed.size()<<endl;
        cout<<"Explored nodes : "<<exploredCount<<endl;

//        printMoves();
    }


    void solveAStar_3()
    {
        priority_queue< Node,vector<Node>,greater<Node> >pq;
        PARENT.clear();

        vector<vector<int>>par(N,vector<int>(N,0));

        set<vector<vector<int>>>closed;
        pq.push(Node(board,par,0,0));
        PARENT[board] = par;

        int exploredCount = 1;

        while(!pq.empty())
        {
            Node curNode = pq.top();
            pq.pop();

            vector<vector<int>> curBoard = curNode.board;

            if(closed.count(curBoard))
                continue;

            if(curBoard == boardTarget)
            {
                cout<<"Number of moves : "<<curNode.MOVE<<endl;
                break;
            }

            closed.insert(curBoard);

            for(int r=0;r<N;r++)
            {
                for(int c=0;c<N;c++)
                {
                    if(curBoard[r][c] == 0)
                    {
                        for(int i=0;i<4;i++)
                        {
                            int newR = r + dr[i];
                            int newC = c + dc[i];

                            if(isOk(newR,newC))
                            {
                                vector<vector<int>> nextBoard = curBoard;
                                swap(nextBoard[r][c],nextBoard[newR][newC]);

                                int newMOVE = curNode.MOVE + 1;
                                int newPRI = newMOVE + h3_linearConflict(nextBoard);

                                if(closed.count(nextBoard))
                                    continue;

//                                closed.insert(nextBoard);
                                pq.push(Node(nextBoard,curBoard,newMOVE,newPRI));
                                PARENT[nextBoard] = curBoard;
                                exploredCount++;
                            }
                        }
                    }
                }
            }
        }

        cout<<"Expanded nodes : "<<closed.size()<<endl;
        cout<<"Explored nodes : "<<exploredCount<<endl;

//        printMoves();
    }

    void printMoves()
    {
        vector<vector<int>> now = boardTarget;
        vector<vector<int>>par(N,vector<int>(N,0));

        stack<vector<vector<int>>> stk;

        while(now != par)
        {
            stk.push(now);
            now = PARENT[now];
        }
        cout<<"Steps ... "<<endl;
        while(!stk.empty())
        {
            cout<<stk.top()<<endl;
            stk.pop();
        }
    }


    void solve()
    {
        bool ok = isSolvable();
        if(!ok)
        {
            cout<<"Not Solvable"<<endl;
            return;
        }


        cout<<"<< Hamming Distance >>"<<endl;
        solveAStar_1();
        cout<<"<< Manhattan Distance >>"<<endl;
        solveAStar_2();
        cout<<"<< Linear Conflict >>"<<endl;
        solveAStar_3();
    }

};

int to_num(string str)
{
    stringstream ss(str);

    int x;
    ss>>x;
    return x;
}

void solveTC()
{
    int n;
    cin>>n;

    vector<vector<int>> grid(n,vector<int>(n,0));

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            string s;
            cin>>s;

            if(s == "*") grid[i][j] = 0;
            else grid[i][j] = to_num(s);
        }
    }

//    cout<<grid<<endl;
    vector<vector<int>> gridTarget = getTargetGrid(n);
//    cout<<gridTarget<<endl;

    Solver solver(n,grid,gridTarget);
    solver.solve();

}

int32_t main()
{
    optimizeIO();

    int tc;
    cin>>tc;

    for(int i=1;i<=tc;i++)
    {
        cout<<"Case : "<<i<<endl;
        solveTC();
    }

    return 0;
}

/**

**/

template<class T1, class T2>
ostream &operator <<(ostream &os, pair<T1,T2>&p)
{
    os<<"{"<<p.first<<", "<<p.second<<"} ";
    return os;
}
template <class T>
ostream &operator <<(ostream &os, vector<T>&v)
{
    os<<"[ ";
    for(T i:v)
    {
        os<<i<<" " ;
    }
    os<<" ]";
    return os;
}

template <class T>
ostream &operator <<(ostream &os, set<T>&v)
{
    os<<"[ ";
    for(T i:v)
    {
        os<<i<<" ";
    }
    os<<" ]";
    return os;
}
