#ifndef UNIT_H_
#define UNIT_H_

#include "GraphObject.h"
class StudentWorld;

class Unit : public GraphObject{
public:
    Unit(int imageID, int startX, int startY, StudentWorld* W, Direction dir = right, double size = 1.0, unsigned int depth = 0):GraphObject::GraphObject(imageID,startX,startY, dir, size, depth){World=W;alive=true;}
    ~Unit(){World=nullptr;}
    virtual bool doSomething(){return false;}
    bool SetXY(int X,int Y){
        moveTo(X, Y);
        return false;//to be change
    }
    StudentWorld* getWorld(){return World;}
    void SetAlive(bool state){alive=state;}
    bool getAlive(){return alive;}
    void SetWorldnull();
    virtual bool areYouBoulder();
    virtual bool areYouProtestor();
    //bool checkBoulderAround();
    virtual void annoy(int);
    virtual void setLeave();
private:
    StudentWorld* World;
    bool alive;
};


class FrackMan : public Unit{
public:
    FrackMan(int,int,StudentWorld*);
    ~FrackMan();
    virtual bool doSomething();//read the key value and do something according to the key value
    void checkAroundDirt();
    int getHealth();
    int getWater();
    int getGold();
    int getSonar();
    void increaseGold();
    void decreaseGold();
    bool dropgold();
    void increaseSonar();
    void decreaseSonar();
    bool useSonar();
    void increaseWater();
    void decreaseWater();
    bool blockSquirt();
    bool Squirt();
    virtual void annoy(int x);
private:
    int health;
    int water;
    int gold;
    int sonar;
};


class Dirt: public Unit{
public:
    Dirt(int,int,StudentWorld*);
    ~Dirt();
private:
    
};

class InvisibleItem: public Unit{
public:
    InvisibleItem(int,int, const int,StudentWorld*);
    ~InvisibleItem();
    bool IsDetected(FrackMan* p);
    bool IsConsumed(FrackMan* p);
private:
};

class Oil: public InvisibleItem{
public:
    Oil(int,int,StudentWorld*);
    ~Oil();
    virtual bool doSomething();
};

class Gold: public InvisibleItem{
public:
    Gold(int,int,bool,StudentWorld*);
    ~Gold();
    virtual bool doSomething();
    void setDropped(bool);
private:
    bool IsDropped;
    int ElapsedRound;
};

class Sonar: public InvisibleItem{
public:
    Sonar(int,int,StudentWorld*);
    ~Sonar();
    virtual bool doSomething();
private:
    int ElapsedRound;
};

class WaterPool: public InvisibleItem{
public:
    WaterPool(int,int,StudentWorld*);
    ~WaterPool();
    virtual bool doSomething();
private:
    int ElapsedRound;
};

class Protester: public Unit{
public:
    Protester(int,int,StudentWorld*);
    ~Protester();
    bool Walk();
    bool checkBlock();//check the Dirt around
    bool checkBlock2();
    bool faceBound();
    bool checkPlayer();
    Direction pickupDirection();
    bool IsPerpendicular(Direction,Direction);
    bool Shout();
    virtual bool doSomething();
    bool seeFrackman();
    bool turntoFrackman();
    bool sitIntersection();
    bool turnIntersection();
    bool Blockin1Step();
    void follow(int,int);
    virtual void annoy(int);
    virtual bool areYouProtestor();
    virtual void setLeave();
private:
    int hitPoint;
    int state;// 0 is regular state
    int numSquaresToMoveInCurrentDirection;
    int ticksToWaitBetweenMoves;
    int ticksToWaitBetweenTurns;
    int ticksToWaitBetweenShouts;
};

class Boulder: public InvisibleItem{
public:
    Boulder(int,int,StudentWorld*);
    ~Boulder();
    virtual bool doSomething();
    void setFalling(int);
    bool checkDirtBeneath();
    bool falling();
    virtual bool areYouBoulder();
private:
    int FallingState;//FallingState, if it is 0, it is stable, 1, ready to fall, 2, falling
    int ticksBeforeFalling;
};

class Squirt: public InvisibleItem{
public:
    Squirt(int,int,Direction,StudentWorld*);
    ~Squirt();
    virtual bool doSomething();
    int DistanceFromOrigin();
    bool MoveBlocked();
    bool harmProtestorWrap();
private:
    int Origin_X;
    int Origin_Y;
};

#endif
// ACTOR_H_
