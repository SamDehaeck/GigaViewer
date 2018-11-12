#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <QVector>
#define MAPSIZE 100
#define SAMPLESIZEMAX 20



class pathfinding
{
    private:
    int MapSize;
    float Pixsize;
    float targetX;
    float targetY;
    int targetX_map;
    int targetY_map;
    float PosiX;
    float PosiY;
    int PosiX_map;
    int PosiY_map;
    float PosObsX[10];
    float PosObsY[10];
    float PosObsX_map[10];
    float PosObsY_map[10];
    int Nobs;

//    bool Mat_Blocked[MAPSIZE][MAPSIZE];
    bool Mat_Opened[MAPSIZE][MAPSIZE];
    bool Mat_Closed[MAPSIZE][MAPSIZE];
    float get_w(int x, int y);
    float get_h(int x, int y);
    bool isBlocked(int x, int y);
    short int Mat_Px[MAPSIZE][MAPSIZE];
    short int Mat_Py[MAPSIZE][MAPSIZE];
    QVector<int> OpenSetX;
    QVector<int> OpenSetY;
    QVector<float> OpenSetG;
    QVector<float> OpenSetF;
    int get_OpenSetIndex(int x, int y);

    QVector<int> PathX;
    QVector<int> PathY;
    float PathX_S[SAMPLESIZEMAX];
    float PathY_S[SAMPLESIZEMAX];
    int Nsamp;

    float DobsMin, DobsMax, Wobsmax;

    float Dright,Ddiagonal, D2ndOrder;

    long long int nchoosek(int n, int k);
    float intPow(float a, unsigned int b);

    void GetMapVariables();
    void ScaleUp();
    void assignAllNeighbors(int x, int y, float g);
    void assignNeighbor(int x, int y, float d, int xparent, int yparent, float gparent);

    void createMatrix();
    bool PathFind();
    void PathReconstruct();
    void PathSampling();
    void PathBezier();
    void PathLinear();

    void cleanVar();
public:

    float PathX_B[200];
    float PathY_B[200];
    int NBezier;

    pathfinding();
    void sendPosI(float x, float y);
    void sendTarget(float x, float y);
    void addObs(float x, float y);
    void clearObs();



    bool run();

};

#endif // PATHFINDING_H
