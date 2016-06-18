#define MOVE_TIME_HERO 7//in frames
#define ATTACK_TIME 6//in frames

#define HP_MAX 50
#define HP_MAX_BAR_DRAW 13
#define HP_HOUSE_HEART 10
#define RADIUS_MOVE_DRAW 2

//defined in screen.h
class Point;
class Pixel_Point;
class Menu;
class Animate;
//defined in game.h
int  mapping(int value,int fromLow, int fromHigh, int toLow, int toHigh);

//return true if the _class attack long distance
bool type_attack(Class _class){
    if(_class==MAGE||_class==ARCHER)
        return true;
    return false;
}

bool you_can_walk(Point* point, Map* mapa, float speed);

class Hero{
    const float initSpeed;
    float speed;
    const Team team;
    const int initHp;
    const int atk;
    const int evasion;
    const Class class_;
    const int rangeAtk;
    static int numOfHeroes;
    Side side;
    int hp;
    int moveTime;
    Point *point;
    Pixel_Point *nextPixel;
    int damageDraw;//-1 if not print
    bool attack_flag;
public:
    Hero(int x, int y, int _initHp,int _atk,int _evasion, float _speed, Side initSide, Team _team, Class class__,int _rangeAtk=0):initSpeed(_speed),team(_team),initHp(_initHp),atk(_atk),evasion(_evasion),class_(class__),rangeAtk(_rangeAtk){
        hp = initHp;
        side = initSide;
        point = new Point(x,y);
        moveTime = 0;
        damageDraw = -1;
        attack_flag = true;
        speed = initSpeed;
        nextPixel = NULL;
        //some the new hero class
        numOfHeroes++;
    }
    ~Hero(){
        //subtracted the hero
        numOfHeroes--;
        delete point;
    }
    //return the pixel point of the Hero
    friend Pixel_Point Map::get_hero_pixel_point(Hero* hero);
    //return true if the hero attacker in this turn
    bool get_attack_flag(){
        return attack_flag;
    }
    void set_attack_flag(bool flag){
        attack_flag = flag;
    }
    //draw hero in your position
    void draw_hero(Map *mapa, Animate* animate, ALLEGRO_COLOR Turncolor){
        Pixel_Point pixel = find_rec(point);
        int dx=0,dy=0, delta;
        //if hero is moving
        if(nextPixel!=NULL){
            delta = mapping((MOVE_TIME_HERO-moveTime),0,MOVE_TIME_HERO,0,SIZE_TILE);
            //calculate the increase between each tile
            if(nextPixel->x > pixel.x)//next in the right
                dx=delta;
            else if(nextPixel->x < pixel.x)//next in the left
                dx=-delta;
            if(nextPixel->y > pixel.y)//next in the down
                dy=delta;
            else if(nextPixel->y < pixel.y)//next in the up
                dy=-delta;
        }
        //if hero in a house1
        if(mapa->tiles[point->y-1][point->x-1]->terrain==HOUSE1){
            if(team==ONE)
                al_draw_tinted_bitmap(animate->get_image("house1 blue")->get_bitmap(),Turncolor,pixel.x-1+dx,pixel.y-1+dy,0);
            else
                al_draw_tinted_bitmap(animate->get_image("house1 red")->get_bitmap(),Turncolor,pixel.x-1+dx,pixel.y-1+dy,0);
        }
        //if hero in a house2
        else if (mapa->tiles[point->y-1][point->x-1]->terrain==HOUSE2){
            if(team==ONE)
                al_draw_tinted_bitmap(animate->get_image("house2 blue")->get_bitmap(),Turncolor,pixel.x-1+dx,pixel.y-1+dy,0);
            else
                al_draw_tinted_bitmap(animate->get_image("house2 red")->get_bitmap(),Turncolor,pixel.x-1+dx,pixel.y-1+dy,0);
        }
        else{
            //draw here he look
            if(side==RIGHT)
                al_draw_tinted_bitmap(get_bitmap(animate),Turncolor,pixel.x-1+dx,pixel.y-1+dy,0);
            else
                al_draw_tinted_bitmap(get_bitmap(animate),Turncolor,pixel.x-1+dx,pixel.y-1+dy,1);
        }
        //draw hp live
        int fit = mapping(hp,0,HP_MAX,0,HP_MAX_BAR_DRAW);
        if(side==RIGHT)
            al_draw_rectangle(pixel.x+2+dx,pixel.y+2+dy,pixel.x+3+dx,pixel.y+fit+2+dy,RED,1);
        else
            al_draw_rectangle(pixel.x-3+SIZE_TILE+dx,pixel.y+1+dy,pixel.x-4+SIZE_TILE+dx,pixel.y+fit+2+dy,RED,1);
        //draw in different if is possible to move
        ALLEGRO_COLOR color;
        if(speed==initSpeed)
            color=GREEN;
        else if(you_can_walk(point,mapa,speed))
            color=YELLOW;
        else if(attack_flag==false)
            color=GREY;
        else
            color=BLACK;
        //draw the circle
        if(initSpeed!=0){
            if(side==RIGHT)
                al_draw_filled_circle(pixel.x+SIZE_TILE-4+dx, pixel.y+2+dy, RADIUS_MOVE_DRAW,color);
            else
                al_draw_filled_circle(pixel.x+2+dx, pixel.y+2+dy, RADIUS_MOVE_DRAW,color);
        }
    }
    ALLEGRO_BITMAP *get_bitmap(Animate* animate){
        return animate->get_image(class_,team)->get_bitmap();
    }
    void live(Map* mapa){
        //if hero in a house
        if(mapa->tiles[point->y-1][point->x-1]->terrain==HOUSE1||
           mapa->tiles[point->y-1][point->x-1]->terrain==HOUSE2){
            hp+=HP_HOUSE_HEART;
            if(hp>initHp)
                hp=initHp;
        }

    }
    int get_damage(){
        return damageDraw;
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
    void set_damage(int _damage){
        damageDraw = _damage;
    }
    int get_atk(){
        return atk;
    }
    int get_range_atk(){
        return rangeAtk;
    }
    Point* get_point(){
        return point;
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
    float get_speed(){
        return speed;
    }
    int get_max_hp(){
        return initHp;
    }
    int damage(int _damage){
        hp-=_damage;
        //dead
        if(hp<=0){
            hp=0;
            return 1;
        }
        return 0;
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
    void set_move(Pixel_Point* pixel){
        moveTime = MOVE_TIME_HERO;
        nextPixel = pixel;
    }
    //if the hero is ready to move (animate)
    bool is_ready_to_move(){
        if(moveTime==0){
            nextPixel=NULL;
            return true;
        }
        return false;
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
    //degrees speed
    void walk(float value){
        speed -= value;
        if(speed<0)
            speed=0;
    }
    void clear_speed(){
        speed = 0;
    }
    //reset the speed of hero, in change of turn
    void reset_speed(Team turnTeam){
        if(turnTeam==team)
            speed = initSpeed;
        else
            speed=0;
    }
    //get number of class of the Hero
    static int get_num_of_heroes(){
        return numOfHeroes;
    }
};

//initialize static number
int Hero::numOfHeroes = 0;
//return the pixel point of the Hero
Pixel_Point Map::get_hero_pixel_point(Hero* hero){
    return (find_rec(hero->point));
}
//return true if you can walk
bool you_can_walk(Point* point, Map* mapa, float speed){
    float weightMin = WEIGHT_MAX;
    //tile in Left, if not limit
    if(point->x-1>0){
        if(mapa->tiles[point->y-1][point->x-2]->mobility < weightMin)
            weightMin=mapa->tiles[point->y-1][point->x-2]->mobility;
    }
    //tile in Right, if not limit
    if(point->x<COLUMNS_TILE){
        if(mapa->tiles[point->y-1][point->x]->mobility < weightMin)
            weightMin=mapa->tiles[point->y-1][point->x]->mobility;
    }
    //tile in Up, if not limit
    if(point->y-1>0){
        if(mapa->tiles[point->y-2][point->x-1]->mobility < weightMin)
            weightMin=mapa->tiles[point->y-2][point->x-1]->mobility;
    }
    //tile in Down, if not limit
    if(point->y<ROWS_TILE){
        if(mapa->tiles[point->y][point->x-1]->mobility < weightMin)
            weightMin=mapa->tiles[point->y][point->x-1]->mobility;
    }
    return (speed>=weightMin);
}
//Find the walk space in mapa, stating in point and finish when WEIGHT_MAX it is reached
//Use the recursion and based on algorithm dijkstra
void space_walk(Map *mapa,Point point, float weight_max, Team team, int rangeAtk=0){
    float newWeight;
    Tile* tileCenter = mapa->tiles[point.y-1][point.x-1];
    Tile* tileAnalyze;
    //tile in Left, if not limit
    if(point.x-1>0){
        tileAnalyze = mapa->tiles[point.y-1][point.x-2];
        //if is not range to long attack
        if(rangeAtk==0){
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
        //range to long attack
        else{
            //weight to attack
            newWeight = tileCenter->weightAtk + 1;
            if((newWeight<tileAnalyze->weightAtk)&&(newWeight<=rangeAtk)){
                //save the way for the tile and the new weight and color
                tileAnalyze->weightAtk = newWeight;
                //new analysis of the space, starting the analyze tile
                Point new_point(point.x-1,point.y);
                space_walk(mapa,new_point,weight_max, team,rangeAtk);
            }
        }
    }
    //tile in Right, if not limit
    if(point.x<COLUMNS_TILE){
        tileAnalyze = mapa->tiles[point.y-1][point.x];
        //if is not range to long attack
        if(rangeAtk==0){
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
        //range to long attack
        else{
            //weight to attack
            newWeight = tileCenter->weightAtk + 1;
            if((newWeight<tileAnalyze->weightAtk)&&(newWeight<=rangeAtk)){
                //save the way for the tile and the new weight and color
                tileAnalyze->weightAtk = newWeight;
                //new analysis of the space, starting the analyze tile
                Point new_point(point.x+1,point.y);
                space_walk(mapa,new_point,weight_max, team,rangeAtk);
            }
        }
    }
    //tile in Up, if not limit
    if(point.y-1>0){
        tileAnalyze = mapa->tiles[point.y-2][point.x-1];
        //if is not range to long attack
        if(rangeAtk==0){
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
        //range to long attack
        else{
            //weight to attack
            newWeight = tileCenter->weightAtk + 1;
            if((newWeight<tileAnalyze->weightAtk)&&(newWeight<=rangeAtk)){
                //save the way for the tile and the new weight and color
                tileAnalyze->weightAtk = newWeight;
                //new analysis of the space, starting the analyze tile
                Point new_point(point.x,point.y-1);
                space_walk(mapa,new_point,weight_max, team,rangeAtk);
            }
        }
    }
    //tile in Down, if not limit
    if(point.y<ROWS_TILE){
        tileAnalyze = mapa->tiles[point.y][point.x-1];
        //if is not range to long attack
        if(rangeAtk==0){
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
        //range to long attack
        else{
            //weight to attack
            newWeight = tileCenter->weightAtk + 1;
            if((newWeight<tileAnalyze->weightAtk)&&(newWeight<=rangeAtk)){
                //save the way for the tile and the new weight and color
                tileAnalyze->weightAtk = newWeight;
                //new analysis of the space, starting the analyze tile
                Point new_point(point.x,point.y+1);
                space_walk(mapa,new_point,weight_max, team,rangeAtk);
            }
        }
    }
}

//clear the last space walk, based in space_walk() function inverse
void clear_space_walk(Map *mapa, Point point){
    //tile in Left, if not limit
    if(point.x-1>0){
        //if the tile is accessible for the point
        if(mapa->tiles[point.y-1][point.x-2]->weight!=WEIGHT_MAX||mapa->tiles[point.y-1][point.x-2]->weightAtk!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y-1][point.x-2]->weight = WEIGHT_MAX;
            mapa->tiles[point.y-1][point.x-2]->weightAtk = WEIGHT_MAX;
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
        if(mapa->tiles[point.y-1][point.x]->weight!=WEIGHT_MAX||mapa->tiles[point.y-1][point.x]->weightAtk!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y-1][point.x]->weight = WEIGHT_MAX;
            mapa->tiles[point.y-1][point.x]->weightAtk = WEIGHT_MAX;
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
        if(mapa->tiles[point.y-2][point.x-1]->weight!=WEIGHT_MAX||mapa->tiles[point.y-2][point.x-1]->weightAtk!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y-2][point.x-1]->weight = WEIGHT_MAX;
            mapa->tiles[point.y-2][point.x-1]->weightAtk = WEIGHT_MAX;
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
        if(mapa->tiles[point.y][point.x-1]->weight!=WEIGHT_MAX||mapa->tiles[point.y][point.x-1]->weightAtk!=WEIGHT_MAX){
            //set the infinity distance, clear the all values
            mapa->tiles[point.y][point.x-1]->weight = WEIGHT_MAX;
            mapa->tiles[point.y][point.x-1]->weightAtk = WEIGHT_MAX;
            mapa->tiles[point.y][point.x-1]->lastTile = NULL;
            mapa->tiles[point.y][point.x-1]->set_color(BLACK);
            //clear the neighbors of the new point - recursion
            Point new_point(point.x,point.y+1);
            clear_space_walk(mapa,new_point);
        }
    }
}
