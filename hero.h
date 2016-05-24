#define MOVE_TIME_HERO 7//in frames

//defined in screen.h
class Point;

enum Team{
    UM,DOIS
};

class Hero{
    static int numOfHeroes;
    const int speed;
    int moveTime;
    Point *point;
    Image* image;
    const Team team;
public:
    int hp;
    Hero(int _speed, int x, int y, Image* _image, Team _team):speed(_speed),team(_team){

        hp = 100;
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
    int get_speed(){
        return speed;
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
