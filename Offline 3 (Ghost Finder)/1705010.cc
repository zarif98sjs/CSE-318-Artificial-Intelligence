
/** Which of the favors of your Lord will you deny ? **/

#include<bits/stdc++.h>
using namespace std;

#define LL long long
#define PII pair<int,int>
#define PLL pair<LL,LL>
#define F first
#define S second

#define ALL(x)      (x).begin(), (x).end()


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

#define EPSILON    (1.0E-8)

inline void optimizeIO()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
}

const int nmax = 2e5+7;

struct Move{
    int newR,newC;
    double transitionP;

    Move(int newR,int newC,double transitionP)
    {
        this->newR = newR;
        this->newC = newC;
        this->transitionP = transitionP;
    }
};

class GhostFinder{

    int numR,numC,nobstacle;
    vector<vector<bool>> obstacle;
    vector<vector<double>> belief;

    double mostProbableTotP = 0.9;
    double leastProbableTotP = 1.0 - mostProbableTotP;

    double sensorYesP = 0.85;
    double sensorNoP = 1.0 - sensorYesP;

    int TIME = 0;


    /**
        0 -1 : down
        1  0 : right
        0 +1 : up
       -1  0 : left
    **/

    vector<int> dcMostProbable = {0,1,0,-1};
    vector<int> drMostProbable = {-1,0,1,0};

    /**
        -1 -1 : top left corner
        -1 +1 : bottom left corner
        +1 -1 : top right corner
        +1 +1 : bottom right corner
         0  0 : own
    **/

    vector<int> dcLeastProbable = {-1,-1,+1,+1,0};
    vector<int> drLeastProbable = {-1,+1,-1,+1,0};

public:

    GhostFinder(int numR,int numC,int nobstacle)
    {
        this->numR = numR;
        this->numC = numC;
        this->nobstacle = nobstacle;

        obstacle = vector<vector<bool>>(numR,vector<bool>(numC,false));
        belief = vector<vector<double>>(numR,vector<double>(numC,0.0));

        takeObstacleInputs();
        init();
        printBelief();
    }

    void takeObstacleInputs()
    {
        for(int i=0;i<nobstacle;i++)
        {
            int r,c;
            cin>>r>>c;

            obstacle[r][c] = true;
        }
    }

    void init()
    {
        for(int i=0;i<numR;i++)
        {
            for(int j=0;j<numC;j++)
            {
                if(!obstacle[i][j])
                {
                    belief[i][j] = (1.0/(numR*numC - nobstacle));
                }
            }
        }
    }

    bool ok(int r,int c)
    {
        if(r>=0 && r<numR && c>=0 && c<numC && !obstacle[r][c]) return true;
        return false;
    }

    vector<Move> getMoves(int r,int c)
    {
        vector<Move>moves;

        // most probable
        int mostProbableCount = 0;
        for(int k=0;k<4;k++)
        {
            int nr = r + drMostProbable[k];
            int nc = c + dcMostProbable[k];

            if(ok(nr,nc)) mostProbableCount++;
        }

        for(int k=0;k<4;k++)
        {
            int nr = r + drMostProbable[k];
            int nc = c + dcMostProbable[k];

            if(ok(nr,nc)){
                Move m(nr,nc,mostProbableTotP/mostProbableCount);
                moves.push_back(m);
            }
        }

        // least probable
        int leastProbableCount = 0;
        for(int k=0;k<5;k++)
        {
            int nr = r + drLeastProbable[k];
            int nc = c + dcLeastProbable[k];

            if(ok(nr,nc)) leastProbableCount++;
        }

        for(int k=0;k<5;k++)
        {
            int nr = r + drLeastProbable[k];
            int nc = c + dcLeastProbable[k];

            if(ok(nr,nc)){

                if(mostProbableCount != 0)
                {
                    Move m(nr,nc,leastProbableTotP/leastProbableCount);
                    moves.push_back(m);
                }
                else
                {
                    Move m(nr,nc,1.0/leastProbableCount);
                    moves.push_back(m);
                }
            }
        }

        return moves;
    }

    void normalize()
    {
        double sum = 0;
        for(int i=0;i<numR;i++)
            for(int j=0;j<numC;j++)
                sum += belief[i][j];

        for(int i=0;i<numR;i++)
            for(int j=0;j<numC;j++)
                belief[i][j] /= sum;
    }

    void advanceTime()
    {
        TIME++;
        vector<vector<double>> newBelief = vector<vector<double>>(numR,vector<double>(numC,0.0));

        for(int i=0;i<numR;i++)
        {
            for(int j=0;j<numC;j++)
            {
                vector<Move> moves = getMoves(i,j);

                for(auto m:moves)
                {
                    newBelief[m.newR][m.newC] += (belief[i][j] * m.transitionP);
                }
            }
        }

        belief = newBelief;
    }

    void updateWithSensorReading(int r,int c,bool hasGhost)
    {
        vector<vector<bool>> isNearBy(numR,vector<bool>(numC,false));

        int countNear = 0;
        for(int i=-1;i<=1;i++)
        {
            for(int j=-1;j<=1;j++)
            {
                int nr = r+i, nc = c+j;

                if(ok(nr,nc))
                {
                    isNearBy[nr][nc] = true;
                    countNear++;
                }

            }
        }

        int countFar = 0;
        for(int i=0;i<numR;i++)
            for(int j=0;j<numC;j++)
                if(ok(i,j) && !isNearBy[i][j]) countFar++;

//        assert((countNear+countFar) == (numR*numC-nobstacle));

        if(hasGhost)
        {
            for(int i=0;i<numR;i++)
            {
                for(int j=0;j<numC;j++)
                {
                    if(isNearBy[i][j]) belief[i][j] *= (sensorYesP);
                    else belief[i][j] *= (sensorNoP);
                }
            }
        }
        else
        {
            for(int i=0;i<numR;i++)
            {
                for(int j=0;j<numC;j++)
                {
                    if(isNearBy[i][j]) belief[i][j] *= (sensorNoP);
                    else belief[i][j] *= (sensorYesP);
                }
            }
        }

        normalize();
    }

    void getMostProbableCellsForGhost()
    {

        double mx = 0.0;
        for(int i=0;i<numR;i++)
        {
            for(int j=0;j<numC;j++)
            {
                if(belief[i][j] > mx) mx = belief[i][j];
            }
        }

        cout<<"Most Probable Cells"<<endl;

        for(int i=0;i<numR;i++)
        {
            for(int j=0;j<numC;j++)
            {
                if(fabs(belief[i][j]-mx)<EPSILON)
                {
                    cout<<"{"<<i<<","<<j<<"}"<<endl;
                }
            }
        }

        cout<<"================================================================"<<endl;

    }

    void printBelief()
    {
        cout<<"Time : "<<TIME<<endl;

        double sum = 0.0;
        for(int i=0;i<numR;i++)
        {
            for(int j=0;j<numC;j++)
            {
                cout << setprecision(4)<<fixed<<belief[i][j]*100.0<<"\t";
                sum += belief[i][j];
            }
            cout<<endl;
        }
        cout<<"Probability Sum : "<<sum*100.0<<endl;
        cout<<"-------------------------------------------------------"<<endl;
    }
};

#define READ        freopen("case1_In.txt", "r", stdin)
#define WRITE       freopen("case1_Out____.txt", "w", stdout)

int main()
{
    optimizeIO();

    READ;
    WRITE;

    int numR,numC,nobstacle;
    cin>>numR>>numC>>nobstacle;

    GhostFinder ghostFinder(numR,numC,nobstacle);

    while(true)
    {
        string type;
        cin>>type;
        if(type == "C")
        {
            ghostFinder.getMostProbableCellsForGhost();
        }
        else if (type == "R")
        {
            int r,c;
            bool hasGhost;

            cin>>r>>c>>hasGhost;

            ghostFinder.advanceTime();
            ghostFinder.updateWithSensorReading(r,c,hasGhost);
            ghostFinder.printBelief();
        }
        else if(type == "T")
        {
            ghostFinder.advanceTime();
            ghostFinder.printBelief();
        }
        else{
            cout<<"Bye Casper!"<<endl;
            break;
        }
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


