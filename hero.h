#define MOVE_TIME_HERO 7//in frames

//defined in screen.h
class Point;

enum Team{
    UM,DOIS
};

class Hero{
    const int speed;
    const Team team;
    const int init_hp;
    const int atk;
    static int numOfHeroes;
    int hp;
    int moveTime;
    Point *point;
    Image* image;

public:
    Hero(Image* _image, int x, int y, int _init_hp,int _atk, int _speed, Team _team):speed(_speed),team(_team),init_hp(_init_hp),atk(_atk){
        hp = init_hp;
        image = _image;
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
        al_draw_bitmap(image->get_bitmap(),pixel.x,pixel.y,0);
    }
    int get_atk(){
        return atk;
    }
    int get_hp(){
        return hp;
    }
    int get_speed(){
        return speed;
    }
    int get_max_hp(){
        return init_hp;
    }
    //addition or subtraction of hp
    void add_hp(int value){
        hp+=value;
        //the limits of hp
        if(hp<0)
            hp=0;
        if(hp>init_hp)
            hp=init_hp;
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
