#include "pathfinding.h"
#include <QDebug>

pathfinding::pathfinding()
{
    qDebug()<<"Path finding object creation";
    MapSize=MAPSIZE;
    Pixsize=65.0/MapSize;
    DobsMin=6.0;
    DobsMax=12.0;
    Wobsmax=10.0;
    Dright=Pixsize;
    Ddiagonal=sqrt(2)*Pixsize;
    D2ndOrder=sqrt(5)*Pixsize;
    NBezier=200;
    Nobs=0;

    OpenSetX=QVector<int>(0);
    OpenSetY=QVector<int>(0);
    OpenSetG=QVector<float>(0);
    OpenSetF=QVector<float>(0);
    PathX=QVector<int>(0);
    PathY=QVector<int>(0);
}


float pathfinding::get_w(int x, int y)
{
    float w=0;
    for (int n=0; n<Nobs; n++){
        float dOx=Pixsize*(x-PosObsX_map[n]);
        float dOy=Pixsize*(y-PosObsY_map[n]);
        float dObs=sqrt(dOx*dOx+dOy*dOy);
        if (dObs<DobsMin){
            w=Wobsmax;
            break;
        }else if(dObs<DobsMax){
            w+=Wobsmax*(dObs-DobsMax)/(DobsMin-DobsMax);
        }
    }
    if (w>Wobsmax)
        w=Wobsmax;
    return w;
}

float pathfinding::get_h(int x, int y)
{
    float dTx=Pixsize*(x-targetX_map);
    float dTy=Pixsize*(y-targetY_map);
    return sqrt(dTx*dTx+dTy*dTy);
}

bool pathfinding::isBlocked(int x, int y)
{
    for (int n=0; n<Nobs; n++){
        float dOx=Pixsize*(x-PosObsX_map[n]);
        float dOy=Pixsize*(y-PosObsY_map[n]);
        float dObs=sqrt(dOx*dOx+dOy*dOy);
        if (dObs<DobsMin)
            return true;
    }
    return false;
}

int pathfinding::get_OpenSetIndex(int x, int y)
{
    for (int i=0; i<OpenSetX.size(); i++){
        if (OpenSetX[i]==x&&OpenSetY[i]==y)
            return i;
    }
    return -1;
}

long long pathfinding::nchoosek(int n, int k)
{
    if (k>n/2)
        return nchoosek(n, n-k);
    double res=1;
    for (int i=1; i<=k; i++)
        res*=(double)(n+1-i)/(double)i;
    return (long long int)res;
}

float pathfinding::intPow(float a, unsigned int b)
{
    float res=1;
    for (int i=0; i<b; i++)
        res*=a;
    return res;
}

void pathfinding::GetMapVariables()
{
    float scale=(MapSize-1.0)/1023.0;
    targetX_map=qRound(targetX*scale);
    targetY_map=qRound(targetY*scale);
//    qDebug()<<targetX_map<<targetY_map;
    PosiX_map=qRound(PosiX*scale);
    PosiY_map=qRound(PosiY*scale);
//    qDebug()<<"Finding Path, #Obs: "<<Nobs;
    for (int i=0; i<Nobs; i++){
        PosObsX_map[i]=PosObsX[i]*scale;
        PosObsY_map[i]=PosObsY[i]*scale;
//        qDebug()<<"Obstacles received: "<<PosObsX[i]<<PosObsY[i];
//        qDebug()<<"Obstacles maped   : "<<PosObsX_map[i]<<PosObsY_map[i];
    }
}

void pathfinding::ScaleUp()
{
    float scale=1023.0/(MapSize-1.0);
    PathX_S[0]=PosiX;
    PathY_S[0]=PosiY;
    for (int i=1; i<Nsamp; i++){
        PathX_S[i]*=scale;
        PathY_S[i]*=scale;
    }
    PathX_S[Nsamp-1]=targetX;
    PathY_S[Nsamp-1]=targetY;
}

void pathfinding::assignAllNeighbors(int x, int y, float g)
{
    assignNeighbor(x+1,y,Dright,x,y,g);
    assignNeighbor(x-1,y,Dright,x,y,g);
    assignNeighbor(x,y+1,Dright,x,y,g);
    assignNeighbor(x,y-1,Dright,x,y,g);

    assignNeighbor(x+1,y+1,Ddiagonal,x,y,g);
    assignNeighbor(x+1,y-1,Ddiagonal,x,y,g);
    assignNeighbor(x-1,y+1,Ddiagonal,x,y,g);
    assignNeighbor(x-1,y-1,Ddiagonal,x,y,g);

    assignNeighbor(x+2,y+1,D2ndOrder,x,y,g);
    assignNeighbor(x+2,y-1,D2ndOrder,x,y,g);
    assignNeighbor(x-2,y+1,D2ndOrder,x,y,g);
    assignNeighbor(x-2,y-1,D2ndOrder,x,y,g);
    assignNeighbor(x+1,y+2,D2ndOrder,x,y,g);
    assignNeighbor(x-1,y+2,D2ndOrder,x,y,g);
    assignNeighbor(x+1,y-2,D2ndOrder,x,y,g);
    assignNeighbor(x-1,y-2,D2ndOrder,x,y,g);
}

void pathfinding::assignNeighbor(int x, int y, float d, int xparent, int yparent, float gparent)
{
    if (x<0||x>=MapSize||y<0||y>=MapSize)
        return;
    if (Mat_Closed[x][y]||Mat_Blocked[x][y])
        return;
    int index=get_OpenSetIndex(x,y);
    if (index<0){
        OpenSetX.append(x);
        OpenSetY.append(y);
        Mat_Px[x][y]=xparent;
        Mat_Py[x][y]=yparent;
        float g=gparent+get_w(x,y)+d;
        float f=g+get_h(x,y);
        OpenSetG.append(g);
        OpenSetF.append(f);
        return;
    }else{
        float PosG=gparent+get_w(x,y)+d;

        if (PosG<OpenSetG[index]){
            Mat_Px[x][y]=xparent;
            Mat_Py[x][y]=yparent;
            OpenSetG[index]=PosG;
            OpenSetF[index]=PosG+get_h(x,y);
        }
    }
    return;
}


void pathfinding::sendPosI(float x, float y)
{
    PosiX=x;
    PosiY=y;
}

void pathfinding::sendTarget(float x, float y)
{
    targetX=x;
    targetY=y;
}

void pathfinding::addObs(float x, float y)
{
    PosObsX[Nobs]=x;
    PosObsY[Nobs]=y;
    Nobs++;
}

void pathfinding::clearObs()
{
    Nobs=0;
}

void pathfinding::createMatrix()
{
    for (int i=0; i<MapSize; i++){
        for (int j=0; j<MapSize; j++)
        {
            Mat_Blocked[i][j]=false;
            Mat_Closed[i][j]=false;
            Mat_Px[i][j]=-1;
            Mat_Py[i][j]=-1;
            for (int n=0; n<Nobs; n++){
                float dOx=Pixsize*(i-PosObsX_map[n]);
                float dOy=Pixsize*(j-PosObsY_map[n]);
                float dObs=sqrt(dOx*dOx+dOy*dOy);
                if (dObs<DobsMin){
                    Mat_Blocked[i][j]=true;
                    break;
                }
            }
        }
    }
}

bool pathfinding::PathFind()
{
//    qDebug()<<"Searching path, N° Obs: "<<Nobs;
    float g=get_w(PosiX_map,PosiY_map);
    float f=get_h(PosiX_map,PosiY_map)+g;
    OpenSetX.append(PosiX_map);
    OpenSetY.append(PosiY_map);
    OpenSetG.append(g);
    OpenSetF.append(f);
    while (OpenSetX.size()>0){
        int IndexMin=0;
        float Fimin=OpenSetF[IndexMin];
        for (int i=1; i<OpenSetF.size(); i++){
            float Fop=OpenSetF[i];
            if (Fop<Fimin){
                IndexMin=i;
                Fimin=Fop;
            }
        }
        int Ximin=OpenSetX[IndexMin];
        int Yimin=OpenSetY[IndexMin];
        float Gimin=OpenSetG[IndexMin];

        if (Ximin==targetX_map&&Yimin==targetY_map)
            return 1;

        Mat_Closed[Ximin][Yimin]=true;
        OpenSetX.remove(IndexMin);
        OpenSetY.remove(IndexMin);
        OpenSetG.remove(IndexMin);
        OpenSetF.remove(IndexMin);

        assignAllNeighbors(Ximin,Yimin,Gimin);
//        qDebug()<Ximin<<Yimin<<OpenSetX.size()<<OpenSetY.size();
    }
    qDebug()<<"No path found";
    return 0;
}

void pathfinding::PathReconstruct()
{
    int x=targetX_map;
    int y=targetY_map;
    while (x>=0 && y>=0){
        PathX.prepend(x);
        PathY.prepend(y);
        int xnew=Mat_Px[x][y];
        int ynew=Mat_Py[x][y];
        x=xnew;
        y=ynew;
    }
}

void pathfinding::PathSampling()
{
    if (PathX.size()<20){
        Nsamp=PathX.size();
        for (int i=0; i<Nsamp; i++){
            PathX_S[i]=(float)PathX[i];
            PathY_S[i]=(float)PathY[i];
        }

    }else{
        Nsamp=20;
        for (int i=0; i<Nsamp; i++){
            int index=qRound(i*PathX.size()/(Nsamp-1.0));
            PathX_S[i]=(float)PathX[index];
            PathY_S[i]=(float)PathY[index];
        }
    }
}

void pathfinding::PathBezier()
{
    int n=Nsamp-1;
    for (int j=0; j<NBezier;j++){
        float t=(float)j/(float)(NBezier-1);
        PathX_B[j]=0;
        PathY_B[j]=0;
        for (int i=0; i<Nsamp; i++){
            float coef=nchoosek(n, i)*intPow(1-t,n-i)*intPow(t,i);
            PathX_B[j]+=coef*PathX_S[i];
            PathY_B[j]+=coef*PathY_S[i];
        }
    }
}

void pathfinding::PathLinear()
{
    for (int j=0; j<NBezier;j++){
        float t=j/(NBezier-1);
        for (int i=0; i<Nsamp; i++){
            PathX_B[j]=(1-t)*PosiX+t*targetX;
            PathY_B[j]=(1-t)*PosiY+t*targetY;
        }
    }
}

bool pathfinding::run()
{
//    qDebug()<<"Map Variables Calculation";
    GetMapVariables();
//        qDebug()<<"Matrix Creation";
    createMatrix();
//        qDebug()<<"Path Finding";
    bool found=PathFind();
    if (found){
//            qDebug()<<"Path Reconstruction";
        PathReconstruct();
//            qDebug()<<"Path Sampling";
        PathSampling();
//            qDebug()<<"Path ScaleUp";
        ScaleUp();
//            qDebug()<<"Path Smoothing";
        PathBezier();
    }
//        qDebug()<<"Clear Variables";
    cleanVar();
//        qDebug()<<"Clear Obstacles";
    clearObs();
    return found;
}

void pathfinding::cleanVar()
{
    OpenSetX.clear();
    OpenSetY.clear();
    OpenSetG.clear();
    OpenSetF.clear();
    PathX.clear();
    PathY.clear();
}

