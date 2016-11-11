#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
#include <vector>
void Unit::SetWorldnull(){
    World=nullptr;
}

bool Unit::areYouBoulder(){
    return false;
}

bool Unit::areYouProtestor(){
    return false;
}

void Unit::annoy(int x){
    return;
}

void Unit::setLeave(){
    return;
}

FrackMan::FrackMan(int startX,int startY,StudentWorld* W) : Unit::Unit(IID_PLAYER,startX,startY,W){
    GraphObject::setVisible(true);
    health=100;
    gold=0;
    water=5;
    sonar=1;
}
FrackMan::~FrackMan(){}
bool FrackMan::doSomething(){
    //std::cout<<getWorld()->checkBoulderAround(getX(), getY());
    if (getAlive()==false) {
        return false;
    }
    if (getHealth()<=0) {
        SetAlive(false);
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return false;
    }
    int key;
    getWorld()->getKey(key);
    switch(key)
    {
        case KEY_PRESS_LEFT:
            if (getDirection()==left) {
                if (getX()-1>=0 && !getWorld()->checkBoulderAround(getX()-1, getY())) {
                    this->moveTo(getX()-1, getY());//if the new posision is within the boundary, move to the new position
                    checkAroundDirt();//set the around dirt to dead
                    return true;
                }
                else{
                    this->moveTo(getX(), getY());//if it is not, move to the same position
                    break;
                }
            }
            else{
                setDirection(left);
                break;
            }
            
        case KEY_PRESS_RIGHT:
            if (getDirection()==right && !getWorld()->checkBoulderAround(getX()+1, getY())) {
                if (getX()+1<=60) {//similar to move right
                    this->moveTo(getX()+1, getY());
                    checkAroundDirt();
                    return true;
                }
                else{
                    this->moveTo(getX(), getY());
                    break;
                }
            }
            else{
                setDirection(right);
                break;
            }
            
        case KEY_PRESS_UP:
            if (getDirection()==up) {
                if (getY()+1<=60 && !getWorld()->checkBoulderAround(getX(), getY()+1)) {
                    this->moveTo(getX(), getY()+1);
                    checkAroundDirt();
                    return true;
                }
                else{
                    this->moveTo(getX(), getY());
                    break;
                }
            }
            else{
                setDirection(up);
                break;
            }
            
        case KEY_PRESS_DOWN:
            if (getDirection()==down && !getWorld()->checkBoulderAround(getX(), getY()-1)) {
                if (getY()-1>=0) {
                    this->moveTo(getX(), getY()-1);
                    checkAroundDirt();
                    return true;
                }
                else{
                    this->moveTo(getX(), getY());
                    break;
                }
            }
            else{
                setDirection(down);
                break;
            }
        case KEY_PRESS_TAB:
            return dropgold();
        case 'Z':
            return useSonar();
        case 'z':
            return useSonar();
        case 'Q':
            SetAlive(false);
            getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
            return false;
        case 'q':
            SetAlive(false);
            getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
            return false;
        case KEY_PRESS_SPACE:
            return Squirt();
    }
    return false;
}
void FrackMan::checkAroundDirt(){//This is dig
    for (int i=getX(); i<getX()+4; i++) {
        for (int j=getY(); j<getY()+4; j++) {
            if (getWorld()->IsDirt(i,j)) {
                getWorld()->playSound(SOUND_DIG);
                getWorld()->getDirtArray()[i][j]->SetAlive(false);
            }
        }
    }
}

bool FrackMan::dropgold(){
    if (this->getGold()>0) {
        Gold *p=new Gold(getX(),getY(),true,getWorld());
        //getWorld()->getVec().push_back(p);
        //getWorld()->getVecTest().push_back(p); WHY this doesn't work?
        getWorld()->VecPush(p);
        decreaseGold();
        return true;
    }
    else{
        return false;
    }
}

bool FrackMan::useSonar(){
    if (this->getSonar()>0) {
        int X=getX();
        int Y=getY();
        decreaseSonar();
        return getWorld()->setRadiusVisible(X, Y, 12);
        
    }
    return false;
}

bool FrackMan::blockSquirt(){
    int X=getX();
    int Y=getY();
    Direction dir=getDirection();
    switch (dir) {
        case GraphObject::up:
            if (getWorld()->checkBoulderAround(X, Y+4)) {
                return true;
            }
            for (int i=X; i<X+4; i++) {
                for (int j=Y+4; j<Y+8; j++) {
                    if (j>64) {
                        return true;
                    }
                    else{
                        if (getWorld()->IsDirt(i, j) ) {
                            return true;
                        }
                        continue;
                    }
                }
            }
            break;
        case GraphObject::down:
            if (getWorld()->checkBoulderAround(X, Y-4)) {
                return true;
            }
            for (int i=X; i<X+4; i++) {
                for (int j=Y-4; j<Y; j++) {
                    if (j<0) {
                        return true;
                    }
                    else{
                        if (getWorld()->IsDirt(i, j) ) {
                            return true;
                        }
                        continue;
                    }
                }
            }
            break;
        case GraphObject::left:
            if (getWorld()->checkBoulderAround(X-4, Y)) {
                return true;
            }
            for (int i=X-4; i<X; i++) {
                for (int j=Y; j<Y+4; j++) {
                    if (i<0) {
                        return true;
                    }
                    else{
                        if (getWorld()->IsDirt(i, j) ) {
                            return true;
                        }
                        continue;
                    }
                }
            }
            break;
        case GraphObject::right:
            if (getWorld()->checkBoulderAround(X+4, Y)) {
                //std::cout<<"b block";
                return true;
            }
            for (int i=X+4; i<X+8; i++) {
                for (int j=Y; j<Y+4; j++) {
                    if (i>64) {
                        return true;
                    }
                    else{
                        if (getWorld()->IsDirt(i, j) ) {
                            return true;
                        }
                        continue;
                    }
                }
            }
            break;
        default:
            break;
    }
    return false;
}

bool FrackMan::Squirt(){
    if (blockSquirt()==false) {
        Direction dir=getDirection();
        class Squirt* s;
        switch (dir) {
            case GraphObject::up:
                //std::cout<<getY()+4;
                s=new class Squirt(getX(),getY()+4,dir,getWorld());
                getWorld()->VecPush(s);
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                decreaseWater();
                return true;
                break;
            case GraphObject::down:
                s=new class Squirt(getX(),getY()-4,dir,getWorld());
                getWorld()->VecPush(s);
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                decreaseWater();
                return true;
                break;
            case GraphObject::left:
                s=new class Squirt(getX()-4,getY(),dir,getWorld());
                getWorld()->VecPush(s);
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                decreaseWater();
                return true;
                break;
            case GraphObject::right:
                s=new class Squirt(getX()+4,getY(),dir,getWorld());
                getWorld()->VecPush(s);
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                decreaseWater();
                return true;
                break;
            default:
                break;
        }
        return false;
    }
    else{
        return false;
    }
}

int FrackMan::getHealth(){
    return health;
}

int FrackMan::getWater(){
    return water;
}

int FrackMan::getGold(){
    return gold;
}

int FrackMan::getSonar(){
    return sonar;
}

void FrackMan::annoy(int x){
    health=health-x;
}

void FrackMan::increaseGold(){
    gold++;
}
void FrackMan::decreaseGold(){
    gold--;
}

void FrackMan::increaseSonar(){
    sonar++;
}

void FrackMan::decreaseSonar(){
    sonar--;
}

void FrackMan::increaseWater(){
    water=water+5;
}

void FrackMan::decreaseWater(){
    water--;
}


Dirt::Dirt(int startX,int startY,StudentWorld* W) : Unit::Unit(IID_DIRT,startX,startY,W,right,0.25)
{
    setVisible(true);//Set dirt visible when constructed
}
Dirt::~Dirt(){setVisible(false);}//Set dirt invisible when destructed

InvisibleItem::InvisibleItem(int startX,int startY,const int imageID,StudentWorld* W) : Unit::Unit(imageID,startX,startY,W)
{
    setVisible(false);
}

InvisibleItem::~InvisibleItem(){}

bool InvisibleItem::IsDetected(FrackMan* p){
    int ply_X=p->getX();
    int ply_Y=p->getY();
    int d=((this->getX()-ply_X)*(this->getX()-ply_X)+(this->getY()-ply_Y)*(this->getY()-ply_Y));
    if  (d<=16){
        setVisible(true);
        return true;
    }
    else{
        return false;
    }
}

bool InvisibleItem::IsConsumed(FrackMan* p){
    int ply_X=p->getX();
    int ply_Y=p->getY();
    int d=((this->getX()-ply_X)*(this->getX()-ply_X)+(this->getY()-ply_Y)*(this->getY()-ply_Y));
    if  (d<=9){
        setVisible(false);
        SetAlive(false);
        return true;
    }
    else{
        return false;
    }
}

Oil::Oil(int startX,int startY, StudentWorld* W) : InvisibleItem::InvisibleItem(startX,startY,IID_BARREL,W)
{
}
Oil::~Oil(){}
bool Oil::doSomething(){
    if (getAlive()) {
        FrackMan* t=getWorld()->getPlayer();
        if (IsDetected(t)) {
            if (IsConsumed(t)) {
                getWorld()->playSound(SOUND_FOUND_OIL);
                getWorld()->increaseScore(1000);
                getWorld()->decreaseOil();
                return true;
            }
        }
    }
    return false;
}

Gold::Gold(int startX,int startY,bool drop,StudentWorld* W) : InvisibleItem::InvisibleItem(startX,startY,IID_GOLD,W)
{
    IsDropped=drop;
    ElapsedRound=0;
    if (drop==true) {
        setVisible(true);
    }
}
Gold::~Gold(){}
bool Gold::doSomething(){
    if (getAlive()) {
        FrackMan* t=getWorld()->getPlayer();
        if (IsDropped==false) {
            if (IsDetected(t)) {
                if (IsConsumed(t)) {
                    getWorld()->playSound(SOUND_GOT_GOODIE);
                    t->increaseGold();
                    getWorld()->increaseScore(10);
                    return true;
                }
            }
        }
        if (IsDropped==true) {
            ElapsedRound++;
            if (getWorld()->bribeProtestor(getX(), getY(), 3)) {
                SetAlive(false);
            }
            //getWorld()->playSound(SOUND_PLAYER_ANNOYED);
            if (ElapsedRound>70) {
                //getWorld()->playSound(SOUND_PLAYER_ANNOYED);
                SetAlive(false);
            }
        }
    }
    return false;
}
void Gold::setDropped(bool drop){
    IsDropped=drop;
}

Sonar::Sonar(int startX,int startY, StudentWorld* W) : InvisibleItem::InvisibleItem(startX,startY,IID_SONAR,W)
{
    setVisible(true);
}
Sonar::~Sonar(){}
bool Sonar::doSomething(){
    ElapsedRound++;
    if (getAlive()) {
        FrackMan* t=getWorld()->getPlayer();
        if (IsDetected(t)) {
            if (IsConsumed(t)) {
                getWorld()->playSound(SOUND_GOT_GOODIE);
                getWorld()->increaseScore(75);
                t->increaseSonar();
                return true;
            }
        }
    }
    if (ElapsedRound>400) {
        SetAlive(false);
    }
    return false;
}

WaterPool::WaterPool(int startX,int startY, StudentWorld* W) : InvisibleItem::InvisibleItem(startX,startY,IID_WATER_POOL,W)
{
    ElapsedRound=0;
    setVisible(true);
}
WaterPool::~WaterPool(){}
bool WaterPool::doSomething(){
    ElapsedRound++;
    if (getAlive()) {
        FrackMan* t=getWorld()->getPlayer();
        if (IsDetected(t)) {
            if (IsConsumed(t)) {
                getWorld()->playSound(SOUND_GOT_GOODIE);
                getWorld()->increaseScore(100);
                t->increaseWater();
                return true;
            }
        }
    }
    if (ElapsedRound>400) {
        SetAlive(false);
    }
    return true;
}

Protester::Protester(int startX,int startY,StudentWorld* w) : Unit::Unit(IID_PROTESTER,startX,startY,w,left){
    GraphObject::setVisible(true);
    hitPoint=5;
    state=0;
    numSquaresToMoveInCurrentDirection=rand()%60+8;
    ticksToWaitBetweenMoves=1;
    ticksToWaitBetweenTurns=0;
    ticksToWaitBetweenShouts=0;
}

/*bool Protester::doSomething(){
 if (ticksToWaitBetweenMoves==0) {
 follow();
 ticksToWaitBetweenMoves=1;
 }
 else{
 ticksToWaitBetweenMoves--;
 }
 return true;
 }*/

bool Protester::doSomething(){
    if (getAlive()==false) {
        return false;
    }
    if (hitPoint<=0 || state==1) {
        state=1;//1 is leaving field state
        follow(60, 60);
        if (getX()==60&&getY()==60) {
            SetAlive(false);
        }
        return true;
    }
    else{
        if (ticksToWaitBetweenMoves==0) {
            ticksToWaitBetweenMoves=1;
            if (ticksToWaitBetweenTurns>0) {
                ticksToWaitBetweenTurns--;
                //std::cout<<ticksToWaitBetweenTurns<<std::endl;
            }
            if (turntoFrackman()) {
                Shout();
                //std::cout<<ticksToWaitBetweenShouts<<std::endl;
                return true;
            }
            else if (turnIntersection()){
                Shout();
                //std::cout<<ticksToWaitBetweenShouts<<std::endl;
                return true;
            }
            else{
                Walk();
                Shout();
                //std::cout<<ticksToWaitBetweenShouts<<std::endl;
            }
            return true;
        }
        else{
            if (ticksToWaitBetweenShouts>0) {
                ticksToWaitBetweenShouts--;
            }
            //std::cout<<ticksToWaitBetweenShouts<<std::endl;
            ticksToWaitBetweenMoves--;
            if (ticksToWaitBetweenTurns>0) {
                ticksToWaitBetweenTurns--;
                //std::cout<<ticksToWaitBetweenTurns<<std::endl;
            }
            return false;
        }
    }
}

/*bool Protester::doSomething(){
 if (ticksToWaitBetweenMoves==0) {
 ticksToWaitBetweenMoves=1;
 if (ticksToWaitBetweenTurns>0) {
 ticksToWaitBetweenTurns--;
 //std::cout<<ticksToWaitBetweenTurns<<std::endl;
 }
 if (turntoFrackman()) {
 Shout();
 //std::cout<<ticksToWaitBetweenShouts<<std::endl;
 return true;
 }
 else if (turnIntersection()){
 Shout();
 //std::cout<<ticksToWaitBetweenShouts<<std::endl;
 return true;
 }
 else{
 Walk();
 Shout();
 //std::cout<<ticksToWaitBetweenShouts<<std::endl;
 }
 return true;
 }
 else{
 if (ticksToWaitBetweenShouts>0) {
 ticksToWaitBetweenShouts--;
 }
 //std::cout<<ticksToWaitBetweenShouts<<std::endl;
 ticksToWaitBetweenMoves--;
 if (ticksToWaitBetweenTurns>0) {
 ticksToWaitBetweenTurns--;
 //std::cout<<ticksToWaitBetweenTurns<<std::endl;
 }
 return false;
 }
 }*/

bool Protester::Shout(){
    FrackMan* p=getWorld()->getPlayer();
    int ply_X=p->getX();
    int ply_Y=p->getY();
    int d=((this->getX()-ply_X)*(this->getX()-ply_X)+(this->getY()-ply_Y)*(this->getY()-ply_Y));
    if  (d<=16&&ticksToWaitBetweenShouts==0){
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->getPlayer()->annoy(20);
        ticksToWaitBetweenShouts=15;
        return true;
    }
    else{
        if (ticksToWaitBetweenShouts>0) {
            ticksToWaitBetweenShouts--;
        }
        return false;
    }
}

bool Protester::Walk(){
    Direction current_direction=this->getDirection();
    if (numSquaresToMoveInCurrentDirection>0) {
        //getWorld()->playSound(SOUND_FALLING_ROCK);
        switch (current_direction) {
            case GraphObject::right:
                if (checkBlock()==false&&(getX()+1)<=60) {
                    moveTo(getX()+1, getY());
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            case GraphObject::left:
                if (checkBlock()==false&&(getX()-1)>=0) {
                    moveTo(getX()-1, getY());
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            case GraphObject::up:
                if (checkBlock()==false&&(getY()+1)<=60) {
                    moveTo(getX(), getY()+1);
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            case GraphObject::down:
                if (checkBlock()==false&&(getY()-1)>=0) {
                    moveTo(getX(), getY()-1);
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            default:
                break;
        }
    }
    //setDirection(pickupDirection());
    //numSquaresToMoveInCurrentDirection=rand()%60+8;
    //getWorld()->playSound(SOUND_FOUND_OIL);
    
    do{
        setDirection(pickupDirection());
    }
    while (faceBound() || checkBlock());//random pick up direction at least for once
    if (IsPerpendicular(current_direction, getDirection())) {
        ticksToWaitBetweenTurns=200;
    }
    numSquaresToMoveInCurrentDirection=rand()%60+8;
    return false;
}

bool Protester::checkBlock(){
    int Pro_X=this->getX();
    int Pro_Y=this->getY();
    Direction current_direction=this->getDirection();
    switch (current_direction) {
        case GraphObject::right:
            for (int y=Pro_Y; y<Pro_Y+4; y++) {
                if ((Pro_X+4)>=0&&(Pro_X+4)<64&&y>=0&&y<64) {
                    if (getWorld()->IsDirt(Pro_X+4, y)) {
                        return true;
                    }
                }
            }
            break;
        case GraphObject::left:
            for (int y=Pro_Y; y<Pro_Y+4; y++) {
                if ((Pro_X-1)>=0&&(Pro_X-1)<64&&y>=0&&y<64) {
                    if (getWorld()->IsDirt(Pro_X-1, y)) {
                        return true;
                    }
                }
            }
            break;
        case GraphObject::up:
            for (int x=Pro_X; x<Pro_X+4; x++) {
                if (x>=0&&x<64&&(Pro_Y+4)>0&&(Pro_Y+4)<64) {
                    if (getWorld()->IsDirt(x, Pro_Y+4)) {
                        return true;
                    }
                }
            }
            break;
        case GraphObject::down:
            for (int x=Pro_X; x<Pro_X+4; x++) {
                if (x>=0&&x<64&&(Pro_Y-1)>0&&(Pro_Y-1)<64) {
                    if (getWorld()->IsDirt(x, Pro_Y-1)) {
                        return true;
                    }
                }
            }
            break;
        default:
            break;
    }
    return false;
}

bool Protester::Blockin1Step(){
    int Pro_X=this->getX();
    int Pro_Y=this->getY();
    Direction current_direction=this->getDirection();
    switch (current_direction) {
        case GraphObject::right:
            for (int y=Pro_Y; y<Pro_Y+4; y++) {
                for (int x=Pro_X+4; x<Pro_X+8; x++) {
                    if (x>=0&&x<64&&y>=0&&y<64) {
                        if (getWorld()->IsDirt(x, y)) {
                            //std::cout<<"Y";
                            return true;
                        }
                    }
                }
            }
            break;
        case GraphObject::left:
            for (int y=Pro_Y; y<Pro_Y+4; y++) {
                for (int x=Pro_X-4; x<Pro_X; x++) {
                    if (x>=0&&x<64&&y>=0&&y<64) {
                        if (getWorld()->IsDirt(x, y)) {
                            return true;
                        }
                    }
                }
            }
            break;
        case GraphObject::up:
            for (int x=Pro_X; x<Pro_X+4; x++) {
                for (int y=Pro_Y+4; Pro_Y+8; y++) {
                    if (x>=0&&x<64&&y>=0&&y<64) {
                        if (getWorld()->IsDirt(x, y)) {
                            return true;
                        }
                    }
                }
            }
            break;
        case GraphObject::down:
            for (int x=Pro_X; x<Pro_X+4; x++) {
                for (int y=Pro_Y-4; y<Pro_Y; y++) {
                    if (x>=0&&x<64&&y>=0&&y<64) {
                        if (getWorld()->IsDirt(x, y)) {
                            return true;
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
    return false;
}


bool Protester::faceBound(){
    Direction current_direction=getDirection();
    switch (current_direction) {
        case GraphObject::up:
            if ((getY()+1)>60) {
                return true;
            }
            break;
        case GraphObject::down:
            if ((getY()-1)<0) {
                return true;
            }
            break;
        case GraphObject::left:
            if ((getX()-1)<0) {
                return true;
            }
            break;
        case GraphObject::right:
            if ((getX()+1)>60) {
                return true;
            }
        default:
            break;
    }
    return false;
}

bool Protester::checkPlayer(){
    int Pro_X=this->getX();
    int Pro_Y=this->getY();
    Direction current_direction=this->getDirection();
    FrackMan *t=getWorld()->getPlayer();
    int Ply_X=t->getX();
    int Ply_Y=t->getY();
    int d=(Pro_X-Ply_X)*(Pro_X-Ply_X)+(Ply_Y-Pro_Y)*(Ply_Y-Pro_Y);
    if (d<16) {
        switch (current_direction) {
            case GraphObject::right:
                if (Ply_X>Pro_X) {
                    return true;
                }
            case GraphObject::left:
                if (Ply_X<Pro_X) {
                    return true;
                }
            case GraphObject::up:
                if (Ply_Y>Pro_Y) {
                    return true;
                }
            case GraphObject::down:
                if (Ply_Y<Pro_Y) {
                    return true;
                }
            default:
                break;
        }
    }
    return false;
}

GraphObject::Direction Protester::pickupDirection(){
    int sw=rand()%4;
    switch (sw) {
        case 0:
            return up;
        case 1:
            return down;
        case 2:
            return left;
        case 3:
            return right;
        default:
            break;
    }
    return left;
}

bool Protester::IsPerpendicular(GraphObject::Direction oldDirection,GraphObject::Direction newDirection){
    switch (oldDirection) {
        case GraphObject::up:
            if (newDirection==left||newDirection==right) {
                return true;
            }
            else{
                return false;
            }
            break;
        case GraphObject::down:
            if (newDirection==left||newDirection==right) {
                return true;
            }
            else{
                return false;
            }
            break;
        case GraphObject::left:
            if (newDirection==up||newDirection==down) {
                return true;
            }
            else{
                return false;
            }
            break;
        case GraphObject::right:
            if (newDirection==up||newDirection==down) {
                return true;
            }
            else{
                return false;
            }
            break;
        default:
            break;
    }
    return false;
}

bool Protester::seeFrackman(){
    FrackMan* t=getWorld()->getPlayer();
    int Ply_X=t->getX();
    int Ply_Y=t->getY();
    int Pro_X=getX();
    int Pro_Y=getY();
    if (Ply_X!=getX()&&Ply_Y!=getY()) {
        return false;
    }
    else{
        if (Ply_X==Pro_X) {
            for (int i= std::min(Ply_Y, Pro_Y);i<std::max(Ply_Y, Pro_Y);i++) {
                if (getWorld()->IsDirt(Pro_X, i)) {
                    return false;
                }
            }
        }
        if (Ply_Y==Pro_Y) {
            for (int i= std::min(Ply_X, Pro_X);i<std::max(Ply_X, Pro_X);i++) {
                if (getWorld()->IsDirt(i, Pro_Y)) {
                    return false;
                }
            }
        }
        return true;
    }
}

/*bool Protester::turntoFrackman(){
 FrackMan* t=getWorld()->getPlayer();
 Direction current_direction=getDirection();
 int Ply_X=t->getX();
 int Ply_Y=t->getY();
 int Pro_X=getX();
 int Pro_Y=getY();
 if (seeFrackman()) {
 if (Pro_X==Ply_X) {
 if (Ply_Y>Pro_Y) {
 setDirection(GraphObject::up);
 moveTo(Pro_X, Pro_Y+1);
 numSquaresToMoveInCurrentDirection=0;
 if (IsPerpendicular( current_direction, up)) {
 ticksToWaitBetweenTurns=200;
 }
 return true;
 }
 else{
 setDirection(GraphObject::down);
 moveTo(Pro_X, Pro_Y-1);
 numSquaresToMoveInCurrentDirection=0;
 if (IsPerpendicular( current_direction, down)) {
 ticksToWaitBetweenTurns=200;
 }
 return true;
 }
 }
 if (Pro_Y==Ply_Y) {
 if (Pro_X<Ply_X) {
 setDirection(GraphObject::right);
 moveTo(Pro_X+1, Pro_Y);
 numSquaresToMoveInCurrentDirection=0;
 if (IsPerpendicular( current_direction, right)) {
 ticksToWaitBetweenTurns=200;
 }
 return true;
 }
 else{
 setDirection(GraphObject::left);
 moveTo(Pro_X-1, Pro_Y);
 numSquaresToMoveInCurrentDirection=0;
 if (IsPerpendicular( current_direction, left)) {
 ticksToWaitBetweenTurns=200;
 }
 return true;
 }
 }
 }
 return false;
 }*/

bool Protester::turntoFrackman(){
    FrackMan* t=getWorld()->getPlayer();
    Direction current_direction=getDirection();
    int Ply_X=t->getX();
    int Ply_Y=t->getY();
    int Pro_X=getX();
    int Pro_Y=getY();
    if (seeFrackman()) {
        if (Pro_X==Ply_X) {
            if (Ply_Y>Pro_Y+4) {
                setDirection(GraphObject::up);
                moveTo(Pro_X, Pro_Y+1);
                numSquaresToMoveInCurrentDirection=0;
                if (IsPerpendicular( current_direction, up)) {
                    ticksToWaitBetweenTurns=200;
                }
                return true;
            }
            else if(Ply_Y<Pro_Y-4){
                setDirection(GraphObject::down);
                moveTo(Pro_X, Pro_Y-1);
                numSquaresToMoveInCurrentDirection=0;
                if (IsPerpendicular( current_direction, down)) {
                    ticksToWaitBetweenTurns=200;
                }
                return true;
            }
            else{
                return true;
            }
        }
        if (Pro_Y==Ply_Y) {
            if (Pro_X<Ply_X-4) {
                setDirection(GraphObject::right);
                moveTo(Pro_X+1, Pro_Y);
                numSquaresToMoveInCurrentDirection=0;
                if (IsPerpendicular( current_direction, right)) {
                    ticksToWaitBetweenTurns=200;
                }
                return true;
            }
            else if(Pro_X>Ply_X+4){
                setDirection(GraphObject::left);
                moveTo(Pro_X-1, Pro_Y);
                numSquaresToMoveInCurrentDirection=0;
                if (IsPerpendicular( current_direction, left)) {
                    ticksToWaitBetweenTurns=200;
                }
                return true;
            }
            else{
                return true;
            }
        }
    }
    return false;
}

bool Protester::sitIntersection(){
    Direction current_direction=this->getDirection();
    switch (current_direction) {
        case GraphObject::left:
            setDirection(GraphObject::up);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            setDirection(GraphObject::down);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            break;
        case GraphObject::right:
            setDirection(GraphObject::up);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            setDirection(GraphObject::down);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            break;
        case GraphObject::up:
            setDirection(GraphObject::left);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            setDirection(GraphObject::right);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            break;
        case GraphObject::down:
            setDirection(GraphObject::left);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            setDirection(GraphObject::right);
            if (checkBlock()==false&&faceBound()==false) {
                setDirection(current_direction);
                return true;
            }
            break;
        default:
            break;
    }
    setDirection(current_direction);
    return false;
}

bool Protester::turnIntersection(){
    if (sitIntersection()&&ticksToWaitBetweenTurns==0) {
        Direction olddirection=getDirection();
        Direction newdirection=getDirection();
        ticksToWaitBetweenTurns=200;////////////ADD a function to judge whether it is perpendicular;
        do{
            newdirection=pickupDirection();
            setDirection(newdirection);
        }
        while (checkBlock()||faceBound()||IsPerpendicular(olddirection, newdirection)==false);//random pick up direction at least for once
        //getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        numSquaresToMoveInCurrentDirection=rand()%60+8;
        Direction new_direction=getDirection();
        switch (new_direction) {
            case GraphObject::right:
                if (checkBlock()==false&&(getX()+1)<=60) {
                    moveTo(getX()+1, getY());//This is neccessary to get out of intersection
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            case GraphObject::left:
                if (checkBlock()==false&&(getX()-1)>=0) {
                    moveTo(getX()-1, getY());
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            case GraphObject::up:
                if (checkBlock()==false&&(getY()+1)<=60) {
                    moveTo(getX(), getY()+1);
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            case GraphObject::down:
                if (checkBlock()==false&&(getY()-1)>=0) {
                    moveTo(getX(), getY()-1);
                    numSquaresToMoveInCurrentDirection--;
                    return true;
                }
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

void Protester::annoy(int x){
    getWorld()->playSound(SOUND_PLAYER_ANNOYED);
    //std::cout<<hitPoint<<std::endl;
    hitPoint=hitPoint-x;
    if (hitPoint<=0) {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
    }
}

void Protester::follow(int X_start,int Y_start){
    int X_end=getX();
    int Y_end=getY();
    //int X_start=getWorld()->getPlayer()->getX();//This is player's location, the chased location
    //int Y_start=getWorld()->getPlayer()->getY();
    //std::cout<<"ppp"<<X_start<<std::endl;
    getWorld()->indexMoveGrid(X_start, Y_start, X_end, Y_end);
    int X_next=getWorld()->getCoord_nextX(X_end, Y_end);
    int Y_next=getWorld()->getCoord_nextY(X_end, Y_end);
    if (X_next>X_end) {
        setDirection(right);
    }
    else if (X_next<X_end){
        setDirection(left);
    }
    else{
        if(Y_next<Y_end){
            setDirection(down);
        }
        else{
            setDirection(up);
        }
    }
    
    //std::cout<<X_next<<"  "<<Y_next<<std::endl;
    moveTo(X_next, Y_next);
}

bool Protester::areYouProtestor(){
    return true;
}

void Protester::setLeave(){
    state=1;
}

Protester::~Protester(){
    
}

Boulder::Boulder(int startX,int startY,StudentWorld* W) : InvisibleItem::InvisibleItem(startX,startY,IID_BOULDER,W){
    setVisible(true);
    FallingState=false;
}

Boulder::~Boulder(){
    
}

void Boulder::setFalling(int fall){
    FallingState=fall;
}

bool Boulder::doSomething(){
    if (getAlive()==false) {
        return false;
    }
    else{
        return falling();
    }
    return false;
}

bool Boulder::checkDirtBeneath(){
    int Boulder_X=getX();
    int Boulder_Y=getY();
    if (Boulder_Y==0) {
        return true;//when the Boulder is on the ground, gound is beneath it.
    }
    else{
        for (int x=Boulder_X; x<Boulder_X+4; x++) {
            if (getWorld()->IsDirt(x,getY()-1)==true) {
                return true;
            }
        }
        return false;
    }
}

bool Boulder::falling(){
    switch (FallingState) {
        case 0:
            if(checkDirtBeneath()==false){
                setFalling(1);
                ticksBeforeFalling=30;
                return true;
            }
            break;
        case 1:
            if (ticksBeforeFalling<=0) {
                setFalling(2);
                return true;
            }
            else{
                ticksBeforeFalling--;
                return true;
            }
            break;
        case 2:
            if (checkDirtBeneath()==false) {
                getWorld()->harmProtestor(getX(), getY(), 3, 1000);
                moveTo(getX(), getY()-1);
                getWorld()->playSound(SOUND_FALLING_ROCK);
                int X_ply=getWorld()->getPlayer()->getX();
                int Y_ply=getWorld()->getPlayer()->getY();
                int d=(X_ply-getX())*(X_ply-getX())+(Y_ply-getY())*(Y_ply-getY());
                if (d<9) {
                    getWorld()->getPlayer()->SetAlive(false);
                    getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
                }
                return true;
            }
            else{
                SetAlive(false);
                return true;
            }
            break;
            
        default:
            break;
    }
    return false;
}

bool Boulder::areYouBoulder(){
    return true;
}

Squirt::Squirt(int startX,int startY,GraphObject::Direction d,StudentWorld* w) : InvisibleItem::InvisibleItem(startX,startY,IID_WATER_SPURT,w){
    setDirection(d);
    setVisible(true);
    Origin_X=startX;
    Origin_Y=startY;
}

Squirt::~Squirt(){
    
}

int Squirt::DistanceFromOrigin(){
    int current_X=getX();
    int current_Y=getY();
    return (current_X-Origin_X)*(current_X-Origin_X)+(current_Y-Origin_Y)*(current_Y-Origin_Y);
}

/*bool Squirt::doSomething(){
 if (getAlive()==false) {
 return false;
 }
 else{
 if (DistanceFromOrigin()>16) {
 SetAlive(false);
 }
 else{
 Direction current_direction=getDirection();
 int X=getX();
 int Y=getY();
 std::cout<<X<<"  "<<Y<<std::endl;
 switch (current_direction) {
 case GraphObject::up:
 if (Y+1<=60 && !getWorld()->checkBoulderAround(X, Y+1)) {
 std::cout<<"live"<<std::endl;
 if (getWorld()->IsDirt(X, Y+1)==false) {
 moveTo(X, Y+1);
 return true;
 }
 }
 std::cout<<"dead"<<std::endl;
 SetAlive(false);
 return false;
 break;
 case GraphObject::down:
 if (Y-1>=0 && !getWorld()->checkBoulderAround(X, Y-1)) {
 std::cout<<"live"<<std::endl;
 if (getWorld()->IsDirt(X, Y-1)==false) {
 moveTo(X, Y-1);
 return true;
 }
 }
 std::cout<<"dead"<<std::endl;
 SetAlive(false);
 return false;
 break;
 case GraphObject::left:
 if (X-1>=0 && !getWorld()->checkBoulderAround(X-1, Y)) {
 //std::cout<<"live"<<std::endl;
 if (getWorld()->IsDirt(X-1, Y)==false) {
 std::cout<<"live"<<std::endl;
 moveTo(X-1, Y);
 return true;
 }
 }
 std::cout<<"dead"<<std::endl;
 SetAlive(false);
 return false;
 break;
 case GraphObject::right:
 if (X+1<=60 && !getWorld()->checkBoulderAround(X+1, Y)) {
 //std::cout<<"live"<<std::endl;
 if (getWorld()->IsDirt(X+1, Y)==false) {
 std::cout<<"live"<<std::endl;
 moveTo(X+1, Y);
 return true;
 }
 }
 std::cout<<"dead"<<std::endl;
 SetAlive(false);
 return false;
 break;
 default:
 break;
 }
 }
 }
 return false;
 }*/

bool Squirt::doSomething(){
    if (getAlive()==false) {
        return false;
    }
    else{
        if (DistanceFromOrigin()>16) {
            SetAlive(false);
        }
        else{
            Direction current_direction=getDirection();
            int X=getX();
            int Y=getY();
            //std::cout<<X<<"  "<<Y<<std::endl;
            if(harmProtestorWrap()){
                SetAlive(false);
                return true;
            }
            
            //Move to next
            switch (current_direction) {
                case GraphObject::up:
                    if (!MoveBlocked()) {
                        //std::cout<<"live"<<std::endl;
                        if (getWorld()->IsDirt(X, Y+1)==false) {
                            moveTo(X, Y+1);
                            return true;
                        }
                    }
                    //std::cout<<"dead"<<std::endl;
                    SetAlive(false);
                    return false;
                    break;
                case GraphObject::down:
                    if (!MoveBlocked()) {
                        //std::cout<<"live"<<std::endl;
                        if (getWorld()->IsDirt(X, Y-1)==false) {
                            moveTo(X, Y-1);
                            return true;
                        }
                    }
                    //std::cout<<"dead"<<std::endl;
                    SetAlive(false);
                    return false;
                    break;
                case GraphObject::left:
                    if (!MoveBlocked()) {
                        //std::cout<<"live"<<std::endl;
                        if (getWorld()->IsDirt(X-1, Y)==false) {
                            //std::cout<<"live"<<std::endl;
                            moveTo(X-1, Y);
                            return true;
                        }
                    }
                    //std::cout<<"dead"<<std::endl;
                    SetAlive(false);
                    return false;
                    break;
                case GraphObject::right:
                    if (!MoveBlocked()) {
                        //std::cout<<"live"<<std::endl;
                        if (getWorld()->IsDirt(X+1, Y)==false) {
                            //std::cout<<"live"<<std::endl;
                            moveTo(X+1, Y);
                            return true;
                        }
                    }
                    //std::cout<<"dead"<<std::endl;
                    SetAlive(false);
                    return false;
                    break;
                default:
                    break;
            }
        }
    }
    return false;
}

bool Squirt::MoveBlocked(){
    int X=getX();
    int Y=getY();
    Direction dir=getDirection();
    switch (dir) {
        case GraphObject::up:
            if (getWorld()->checkBoulderAround(X, Y+4)) {
                return true;
            }
            for (int i=X; i<X+4; i++) {
                int j=Y+4;
                if (j>64) {
                    return true;
                }
                else{
                    if (getWorld()->IsDirt(i, j) ) {
                        return true;
                    }
                    continue;
                }
            }
            break;
        case GraphObject::down:
            if (getWorld()->checkBoulderAround(X, Y-1)) {
                return true;
            }
            for (int i=X; i<X+4; i++) {
                int j=Y-1;
                if (j<0) {
                    return true;
                }
                else{
                    if (getWorld()->IsDirt(i, j) ) {
                        return true;
                    }
                    continue;
                }
            }
            break;
        case GraphObject::left:
            if (getWorld()->checkBoulderAround(X-1, Y)) {
                return true;
            }
            for (int j=Y; j<Y+4; j++) {
                int i=X-1;
                if (i<0) {
                    return true;
                }
                else{
                    if (getWorld()->IsDirt(i, j) ) {
                        return true;
                    }
                    continue;
                }
            }
            break;
        case GraphObject::right:
            if (getWorld()->checkBoulderAround(X+4, Y)) {
                //std::cout<<"b block";
                return true;
            }
            for (int j=Y; j<Y+4; j++) {
                int i=X+4;
                if (i>64) {
                    return true;
                }
                else{
                    if (getWorld()->IsDirt(i, j) ) {
                        return true;
                    }
                    continue;
                }
            }
            break;
        default:
            break;
    }
    return false;
}

bool Squirt::harmProtestorWrap(){
    int X=getX();
    int Y=getY();
    return getWorld()->harmProtestor(X, Y, 3, 2);
}