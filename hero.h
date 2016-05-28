#define MOVE_TIME_HERO 5//in frames
#define ATTACK_TIME 7//in frames

//defined in screen.h
class Point;

enum Team{
    ONE,TWO
};
//side to lock hero
enum Side{
    RIGHT,LEFT
};

enum Class{
    MAGE, SOLDIER, ARCHER
};

class Hero{
    const int speed;
    const Team team;
    const int initHp;
    const int atk;
    const int evasion;
    const Class class_;
    static int numOfHeroes;
    int hp;
    Side side;
    int moveTime;
    Point *point;
    Image* image;
    Image** imgAttack;
public:
    Hero(Image* _image,Image* _imgAttack[4], int x, int y, int _initHp,int _atk,int _evasion, int _speed, Side initSide, Team _team, Class class__):speed(_speed),team(_team),initHp(_initHp),atk(_atk),evasion(_evasion),class_(class__){
        hp = initHp;
        side = initSide;
        image = _image;
        imgAttack = _imgAttack;
        point = new Point(x,y);
        moveTime = 0;
        //some the new hero class
        numOfHeroes++;
    }
    ~Hero(){
        //subtracted the hero
        numOfHeroes--;
        delete point;
    }
    //draw hero in your position
    void draw_hero(){
        Pixel_Point pixel = find_rec(point);
        //draw here he look
        if(side==RIGHT)
            al_draw_bitmap(image->get_bitmap(),pixel.x,pixel.y,0);
        else
            al_draw_bitmap(image->get_bitmap(),pixel.x,pixel.y,1);
    }
    Side get_side(){
        return side;
    }
    Class get_class(){
        return class_;
    }
    void set_side(Side _side){
        side = _side;
    }
    int get_atk(){
        return atk;
    }
    int get_evasion(){
        return evasion;
    }
    Team get_team(){
        return team;
    }
    int get_hp(){
        return hp;
    }
    int get_speed(){
        return speed;
    }
    int get_max_hp(){
        return initHp;
    }
    //addition or subtraction of hp
    void add_hp(int value){
        hp+=value;
        //the limits of hp
        if(hp<0)
            hp=0;
        if(hp>initHp)
            hp=initHp;
    }
    //start the count for the next move
    void set_move(){
        moveTime = MOVE_TIME_HERO;
    }
    //if the hero is ready to move (animate)
    bool is_ready_to_move(){
        return (moveTime==0);
    }
    //new count for the next move
    void cont_move(){
        if(moveTime)
            moveTime--;
    }
    //move the hero to new point
    void moves(Point _point){
        *point = _point;
    }
    //get number of class of the Hero
    static int get_num_of_heroes(){
        return numOfHeroes;
    }
};
//initialize static number
int Hero::numOfHeroes = 0;

//Find the walk space in mapa, stating in point and finish when WEIGHT_MAX it is reached
//Use the recursion and based on algorithm dijkstra
void space_walk(Map *mapa,Point point, float weight_max, Team team){
    float newWeight;
    Tile* tileCenter = mapa->tiles[point.y-1][point.x-1];
    Tile* tileAnalyze;
    //tile in Left, if not limit
    if(point.x-1>0){
        tileAnalyze = mapa->tiles[point.y-1][point.x-2];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if in the tile had a enemy
        if((tileAnalyze->hero!=NULL)&&(tileAnalyze->hero->get_team()!=team))
            newWeight=WEIGHT_MAX;
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x-1,point.y);
            space_walk(mapa,new_point,weight_max, team);
        }
    }
    //tile in Right, if not limit
    if(point.x<COLUMNS_TILE){
        tileAnalyze = mapa->tiles[point.y-1][point.x];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if in the tile had a enemy
        if((tileAnalyze->hero!=NULL)&&(tileAnalyze->hero->get_team()!=team))
            newWeight=WEIGHT_MAX;
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x+1,point.y);
            space_walk(mapa,new_point,weight_max, team);
        }
    }
    //tile in Up, if not limit
    if(point.y-1>0){
        tileAnalyze = mapa->tiles[point.y-2][point.x-1];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if in the tile had a enemy
        if((tileAnalyze->hero!=NULL)&&(tileAnalyze->hero->get_team()!=team))
            newWeight=WEIGHT_MAX;
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x,point.y-1);
            space_walk(mapa,new_point,weight_max, team);
        }
    }
    //tile in Down, if not limit
    if(point.y<ROWS_TILE){
        tileAnalyze = mapa->tiles[point.y][point.x-1];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if in the tile had a enemy
        if((tileAnalyze->hero!=NULL)&&(tileAnalyze->hero->get_team()!=team))
            newWeight=WEIGHT_MAX;
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x,point.y+1);
            space_walk(mapa,new_point,weight_max, team);
        }
    }
}



//clear the last space walk, based in space_walk() function inverse
void clear_space_walk(Map *mapa, Point point){
    //tile in Left, if not limit
    if(point.x-1>0){
        //if the tile is accessible for the point
        if(mapa->tiles[point.y-1][point.x-2]->weight!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y-1][point.x-2]->weight = WEIGHT_MAX;
            mapa->tiles[point.y-1][point.x-2]->lastTile = NULL;
            mapa->tiles[point.y-1][point.x-2]->set_color(BLACK);
            //clear the neighbors of the new point - recursion
            Point new_point(point.x-1,point.y);
            clear_space_walk(mapa,new_point);
        }
    }
    //tile in Right, if not limit
    if(point.x<COLUMNS_TILE){
        //if the tile is accessible for the point
        if(mapa->tiles[point.y-1][point.x]->weight!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y-1][point.x]->weight = WEIGHT_MAX;
            mapa->tiles[point.y-1][point.x]->lastTile = NULL;
            mapa->tiles[point.y-1][point.x]->set_color(BLACK);
            //clear the neighbors of the new point - recursion
            Point new_point(point.x+1,point.y);
            clear_space_walk(mapa,new_point);
        }
    }
    //tile in Up, if not limit
    if(point.y-1>0){
        //if the tile is accessible for the point
        if(mapa->tiles[point.y-2][point.x-1]->weight!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y-2][point.x-1]->weight = WEIGHT_MAX;
            mapa->tiles[point.y-2][point.x-1]->lastTile = NULL;
            mapa->tiles[point.y-2][point.x-1]->set_color(BLACK);
            //clear the neighbors of the new point - recursion
            Point new_point(point.x,point.y-1);
            clear_space_walk(mapa,new_point);
        }
    }
    //tile in Down, if not limit
    if(point.y<ROWS_TILE){
        //if the tile is accessible for the point
        if(mapa->tiles[point.y][point.x-1]->weight!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y][point.x-1]->weight = WEIGHT_MAX;
            mapa->tiles[point.y][point.x-1]->lastTile = NULL;
            mapa->tiles[point.y][point.x-1]->set_color(BLACK);
            //clear the neighbors of the new point - recursion
            Point new_point(point.x,point.y+1);
            clear_space_walk(mapa,new_point);
        }
    }
}
