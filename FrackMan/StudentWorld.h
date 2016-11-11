#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
//#deFrackMan
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
class FrackMan;
class Unit;
class Dirt;
class Oil;
struct Coord{
    int x_cur;
    int y_cur;
    int x_prev;
    int y_prev;
    int energy;//energy will be the index in the Lee's algorithm
};
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool IsDirt(int X,int Y);
    Dirt*** getDirtArray();
    FrackMan* getPlayer();
    void cleanDirt();//clean all the dirt set to dead
    void cleanVec();
    void UpdateString();
    void OilCollision();
    void GoldCollision();
    bool PlayerPutGold(int,int,int);
    void VecDosomething();
    int getHealth();
    int getWater();
    int getGold();
    int getSonar();
    int getOilLeft();
    std::vector<Unit*> getVec();
    std::vector<Unit*> getVecTest();
    void VecPush(Unit*);
    void VecPop();
    bool setRadiusVisible(int,int,int);
    void indexMoveGrid(int X_start, int Y_start, int X_end, int Y_end);
    bool checkDirtAround(int,int);
    int getCoord_nextX(int,int);
    int getCoord_nextY(int,int);
    bool checkBoulderAround(int,int);
    bool checkItemAround(int,int);
    bool harmProtestor(int,int,int,int);
    bool bribeProtestor(int,int,int);
    void decreaseOil();
private:
    Oil* temp;
    std::string Status;
    FrackMan *player;
    Dirt*** dirt_array;//!!!!It is a two dimensional array of dirt
    std::vector<Unit*> Vec;
    std::vector<Unit*> VecTest;
    Coord moveGrid[61][61];
    int Oil_left;
    int TicksBetweenAddProtector;
    int ProtectorMaxNum;
    int ProtectorInField;
    int ProtectorAddCount;
};

#endif



// STUDENTWORLD_H_
