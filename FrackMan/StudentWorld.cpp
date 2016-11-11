#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <queue>
using namespace std;

StudentWorld::StudentWorld(std::string assetDir): GameWorld(assetDir)//all the pointers in the class are nullptr
{
    dirt_array=new Dirt**[64];
    for (int i=0; i<64; i++) {
        dirt_array[i]=new Dirt*[64];//every row is an array of Dirt pointer
    }
    player=nullptr;
}

int StudentWorld::init()
{
    player=new FrackMan(30,60,this);
    UpdateString();
    GameWorld::setGameStatText(Status);
    for (int i=0; i<64; i++) {
        for (int j=0; j<60; j++) {
            if (i>29 && i<34 && j>=4) {
                dirt_array[i][j]=nullptr;//when the pixel in the mineshaft area, it will be a nullptr
            }
            else{
                dirt_array[i][j]=new Dirt(i,j,this);
            }
        }
    }
    int Boulder_num;
    if (getLevel()/2+2<6) {
        Boulder_num=getLevel()/2+2;
    }
    else{
        Boulder_num=6;
    }
    
    for (int i=0; i<Boulder_num; i++) {
        int X=rand()%57;
        int Y=rand()%57;
        while ((X<=33 && X>=27) || checkBoulderAround(X, Y)) {
            X=rand()%57;
            Y=rand()%57;
        }
        Boulder *b=new Boulder(X,Y,this);
        VecPush(b);
        for (int i=X; i<X+4; i++) {
            for (int j=Y; j<Y+4; j++) {
                dirt_array[i][j]->SetAlive(false);
                dirt_array[i][j]->setVisible(false);
            }
        }
    }
    
    int Gold_num;
    if (5-getLevel()/2>2) {
        Gold_num=5-getLevel()/2;
    }
    else{
        Gold_num=2;
    }
    for (int i=0; i<Gold_num; i++) {
        int X=rand()%57;
        int Y=rand()%57;
        while ((X<=33 && X>=27) || checkItemAround(X, Y)) {
            X=rand()%57;
            Y=rand()%57;
        }
        Gold *b=new Gold(X,Y,false,this);
        //b->setVisible(true);
        VecPush(b);
    }
    
    int Oil_num;
    if (getLevel()+2<20) {
        Oil_num=getLevel()+2;
    }
    else{
        Oil_num=20;
    }
    Oil_left=Oil_num;
    for (int i=0; i<Oil_num; i++) {
        int X=rand()%57;
        int Y=rand()%57;
        while ((X<=33 && X>=27) || checkItemAround(X, Y)) {
            X=rand()%57;
            Y=rand()%57;
        }
        Oil *b=new Oil(X,Y,this);
        //b->setVisible(true);
        VecPush(b);
    }
    if (15 < 2 + getLevel()*1.5){
        ProtectorMaxNum= 15;
    }
    else{
        ProtectorMaxNum=2+getLevel()*1.5;
    }
    if (25 > 200 - getLevel()){
        TicksBetweenAddProtector= 25;
    }
    else{
        TicksBetweenAddProtector=200-getLevel();}
    
    /*Sonar *t=new Sonar(0, 60, this);
    VecPush(t);
    WaterPool* O=new WaterPool(60, 60, this);
    VecPush(O);*/
    ProtectorAddCount=0;
    Protester *u=new Protester(60,60,this);
    VecPush(u);
    ProtectorInField=0;
    
    //Squirt* s=new Squirt(50, 60, GraphObject::right, this);
    //VecPush(s);
   // Vec.push_back(nullptr);//this is must after
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    ProtectorAddCount++;
    UpdateString();
    GameWorld::setGameStatText(Status);
    if (player->getAlive()==false) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    if (Oil_left<=0) {
        return GWSTATUS_FINISHED_LEVEL;
    }
    player->doSomething();//attemptively move it
    VecDosomething();
    if (temp!=nullptr) {
        temp->doSomething();
        if (temp->getAlive()==false) {
            delete temp;
            temp=nullptr;
        }
    }
    if (ProtectorAddCount>TicksBetweenAddProtector && ProtectorInField<ProtectorMaxNum) {
        ProtectorInField++;
        ProtectorAddCount=0;
        Protester *p=new Protester(60,60,this);
        VecPush(p);
    }
    int Prob=25*getLevel()+300;
    if (rand()%(1+Prob)==1) {
        if (rand()%(1+5)==1) {
            if (300-10*getLevel()<100) {
                Sonar* t= new Sonar(0,60,this);
                VecPush(t);
            }
        }
        else{
            int X=rand()%61;
            int Y=rand()%61;
            while (checkDirtAround(X, Y)) {
                X=rand()%61;
                Y=rand()%61;
            }
            WaterPool *W=new WaterPool(X,Y,this);
            VecPush(W);
        }
    }

    
    cleanDirt();
    cleanVec();
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    for (int i=0; i<64; i++) {
        for (int j=0; j<60; j++) {
            if (dirt_array[i][j]!=nullptr) {
                delete dirt_array[i][j];
                dirt_array[i][j]=nullptr;
            }
        }
    }
    std::vector<Unit*>::iterator it=Vec.begin();
    while (it!=Vec.end()) {
            if ((*it)!=nullptr) {
                    //(*it)->setVisible(false);
                    delete (*it);
                    //(*it)=nullptr;
                    Vec.erase(it);
            }
            it++;
        }
    delete player;
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

bool StudentWorld::IsDirt(int X,int Y){//determine if the XY element is covered by dirt
    if (StudentWorld::dirt_array[X][Y]==nullptr) {
        return false;
    }
    else{
    return true;
    }
}

FrackMan* StudentWorld::getPlayer(){
    return player;
}
Dirt*** StudentWorld::getDirtArray(){
    return dirt_array;
}

StudentWorld::~StudentWorld(){//destructor, delete the all the objects that the pointers in the dirt array points to
    for (int i=0; i<64; i++) {
        for (int j=0; j<60; j++) {
            if (dirt_array[i][j]!=nullptr) {
                delete dirt_array[i][j];
                dirt_array[i][j]=nullptr;
            }
        }
    }
    delete player;
}

void StudentWorld::VecDosomething(){
    for (std::vector<Unit*>::iterator it=Vec.begin(); it!=Vec.end(); ++it) {
        if ((*it)!=nullptr) {
            (*it)->doSomething();
        }
    }
}

void StudentWorld::cleanDirt(){
    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            if (dirt_array[i][j]!=nullptr) {
                if (dirt_array[i][j]->getAlive()==false) {
                    delete dirt_array[i][j];
                    dirt_array[i][j]=nullptr;
                }
            }
        }
    }
}

void StudentWorld::cleanVec(){
    std::vector<Unit*>::iterator it=Vec.begin();
    while (it!=Vec.end()) {
        if ((*it)!=nullptr) {
            if (!((*it)->getAlive())) {
                //(*it)->setVisible(false);
                delete (*it);
                //(*it)=nullptr;
                Vec.erase(it);
            }
        }
        it++;
    }
}

bool StudentWorld::setRadiusVisible(int X,int Y,int R){
    std::vector<Unit*>::iterator it=Vec.begin();
    while (it!=Vec.end()) {
        if ((*it)!=nullptr) {
            int X2=(*it)->getX();
            int Y2=(*it)->getY();
            int d=(X2-X)*(X2-X)+(Y2-Y)*(Y2-Y);
            if (d<R*R) {
                (*it)->setVisible(true);
            }
        }
        it++;
    }
    return true;
}
/*void StudentWorld::cleanVec(){
 
    int count=0;
    for (std::vector<Unit*>::iterator it=Vec.begin(); it!=Vec.end() ; ++it) {
        //count++;
        if ((*it)!=nullptr) {
            if (!((*it)->getAlive())) {
                (*it)->setVisible(false);
                delete (*it);
                (*it)=nullptr;
                if ((next(it)!=Vec.end())) {
                    Vec.erase(it);
                }
                else{
                    Vec.pop_back();
                }
 
               if (count!=Vec.size()-1) {
                    Vec.erase(it);
                }
                else{
                    Vec.pop_back();
                }
                
            }
        }
    }
}*/

void StudentWorld::UpdateString(){
    std::string str1="Scr: ";
    std::string str2="  Lvl:  ";
    std::string str3="  Lives:";
    std::string str4="  Hlth: ";
    std::string str5="%  Wtr:  ";
    std::string str6="  Gld:  ";
    std::string str7="  Sonar:  ";
    std::string str8="  Oil Left:  ";
    Status=str1+std::to_string(this->getScore())+str2+std::to_string(this->getLevel())+str3+std::to_string(this->getLives())+str4+std::to_string(this->getHealth())+str5+std::to_string(getWater())+str6+std::to_string(this->getGold())+str7+std::to_string(this->getSonar())+str8+std::to_string(this->getOilLeft());
}

int StudentWorld::getHealth(){
    return player->getHealth();
}
int StudentWorld::getWater(){
    return player->getWater();
}
int StudentWorld::getGold(){
    return player->getGold();
}
int StudentWorld::getSonar(){
    return player->getSonar();
}
int StudentWorld::getOilLeft(){
    return Oil_left;
}
std::vector<Unit*> StudentWorld::getVec(){
    return Vec;
}
std::vector<Unit*> StudentWorld::getVecTest(){
    return VecTest;
}
void StudentWorld::VecPush(Unit *t){
    if (Vec.size()==0) {
        Vec.push_back(t);
        Vec.push_back(nullptr);
    }
    else{
        Vec.pop_back();
        Vec.push_back(t);
        Vec.push_back(nullptr);
    }
}
void StudentWorld::VecPop(){
    Vec.pop_back();
}

void StudentWorld::indexMoveGrid(int X_start, int Y_start, int X_end, int Y_end){
    //Coord moveGrid[61][61];
    std::queue<Coord> myqueue;
    for (int i=0; i<61; i++) {
        for (int j=0; j<61; j++) {
            moveGrid[i][j].x_cur=i;
            moveGrid[i][j].y_cur=j;
            moveGrid[i][j].energy=9999;
        }
    }
    moveGrid[X_start][Y_start].energy=0;
    moveGrid[X_start][Y_start].x_prev=X_start;
    moveGrid[X_start][Y_start].y_prev=Y_start;
    myqueue.push(moveGrid[X_start][Y_start]);
    //cout<<"gg"<<X_start<<endl;
    //int i=0;
    //cout<<myqueue.front().x_cur;
    while (!myqueue.empty()) {
        Coord coord_cur=myqueue.front();
        myqueue.pop();
        int x=coord_cur.x_cur;
        int y=coord_cur.y_cur;
        int e=coord_cur.energy;
        //i++;
        //cout<<"xx"<<x<<endl;
        //cout<<"yy"<<y<<endl;
        //cout<<"ee"<<e<<endl;
        //cout<<"yy"<<y<<endl;
        //cout<<i<<endl;
        //cout<<checkDirtAround(0, 0);
        if (x-1>=0 && !checkDirtAround(x-1, y) && moveGrid[x-1][y].energy==9999) {
            
            moveGrid[x-1][y].x_prev=x;
            moveGrid[x-1][y].y_prev=y;
            //cout<<e<<endl;
            moveGrid[x-1][y].energy=e+1;
            myqueue.push(moveGrid[x-1][y]);
            //cout<<e+1<<endl;
        }
        if (x+1<=60 && !checkDirtAround(x+1, y) && moveGrid[x+1][y].energy==9999) {
            moveGrid[x+1][y].x_prev=x;
            moveGrid[x+1][y].y_prev=y;
            moveGrid[x+1][y].energy=e+1;
            myqueue.push(moveGrid[x+1][y]);
        }
        if (y+1<=60 && !checkDirtAround(x, y+1) && moveGrid[x][y+1].energy==9999) {
            
            moveGrid[x][y+1].x_prev=x;
            moveGrid[x][y+1].y_prev=y;
            moveGrid[x][y+1].energy=e+1;
            myqueue.push(moveGrid[x][y+1]);
        }
        if (y-1>=0 && !checkDirtAround(x, y-1) && moveGrid[x][y-1].energy==9999) {
            
            moveGrid[x][y-1].x_prev=x;
            moveGrid[x][y-1].y_prev=y;
            moveGrid[x][y-1].energy=e+1;
            myqueue.push(moveGrid[x][y-1]);
        }
        //cout<<"push"<<endl;
    }
    //cout<<i<<endl;
    //cout<<moveGrid[0][0].energy;
}

bool StudentWorld::checkDirtAround(int x,int y){
    for (int i=x; i<x+4; i++) {
        for (int j=y; j<y+4; j++) {
            if (IsDirt(i, j)) {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::checkBoulderAround(int X,int Y){
    std::vector<Unit*>::iterator it=Vec.begin();
    while (it!=Vec.end()) {
        if ((*it)!=nullptr) {
            if (((*it)->areYouBoulder())) {
                int Boulder_X=(*it)->getX();
                int Boulder_Y=(*it)->getY();
                int d=(X-Boulder_X)*(X-Boulder_X)+(Y-Boulder_Y)*(Y-Boulder_Y);
                if (d<16) {
                    return true;
                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::checkItemAround(int X,int Y){
    std::vector<Unit*>::iterator it=Vec.begin();
    while (it!=Vec.end()) {
        if ((*it)!=nullptr) {
                int Boulder_X=(*it)->getX();
                int Boulder_Y=(*it)->getY();
                int d=(X-Boulder_X)*(X-Boulder_X)+(Y-Boulder_Y)*(Y-Boulder_Y);
                if (d<16) {
                    return true;
                }
        }
        it++;
    }
    return false;
}

bool StudentWorld::harmProtestor(int X, int Y, int r,int harm){
    std::vector<Unit*>::iterator it=Vec.begin();
    while (it!=Vec.end()) {
        if ((*it)!=nullptr) {
            if (((*it)->areYouProtestor())) {
                int Protestor_X=(*it)->getX();
                int Protestor_Y=(*it)->getY();
                int d2=(Protestor_X-X)*(Protestor_X-X)+(Protestor_Y-Y)*(Protestor_Y-Y);
                //std::cout<<d2<<std::endl;
                if (d2<r*r) {   //||(*it)->getstate()!=1 ??? why call base class function
                    (*it)->annoy(harm);
                    return true;
                }
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::bribeProtestor(int X, int Y, int r){
    std::vector<Unit*>::iterator it=Vec.begin();
    while (it!=Vec.end()) {
        if ((*it)!=nullptr) {
            if (((*it)->areYouProtestor())) {
                int Protestor_X=(*it)->getX();
                int Protestor_Y=(*it)->getY();
                int d2=(Protestor_X-X)*(Protestor_X-X)+(Protestor_Y-Y)*(Protestor_Y-Y);
                //std::cout<<d2<<std::endl;
                if (d2<r*r) {   //||(*it)->getstate()!=1 ??? why call base class function
                    (*it)->setLeave();
                    playSound(SOUND_PROTESTER_FOUND_GOLD);
                    return true;
                }
            }
        }
        it++;
    }
    return false;
}

int StudentWorld::getCoord_nextX(int current_X,int current_Y){
    return moveGrid[current_X][current_Y].x_prev;
}
int StudentWorld::getCoord_nextY(int current_X,int current_Y){
    return moveGrid[current_X][current_Y].y_prev;
}

void StudentWorld::decreaseOil(){
    Oil_left--;
}




// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
