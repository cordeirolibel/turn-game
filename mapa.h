#define BAR_OPTIONS 80 //options bar start, in pixels
#define SIZE_TILE 32
#define ROWS_TILE 20
#define COLUMNS_TILE 40

#define WEIGHT_MAX 8000
#define IMGS_ANIMATE 10
#define DAMAGE_FRAMES_DRAW 2
#define MAX_TEXT 50

//directories
#define ARCHER_BLUE "bin/archer_blue.png"
#define ARCHER_RED "bin/archer_red.png"
#define MAGE_RED "bin/mage_red.png"
#define MAGE_BLUE "bin/mage_blue.png"
const char MAGE_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/lightning1.png"},{"bin/lightning2.png"},{"bin/lightning3.png"},{"bin/lightning4.png"},{"bin/lightning5.png"},{"bin/lightning6.png"},{"bin/lightning5.png"},{"bin/lightning6.png"},{"bin/lightning5.png"},{"bin/lightning6.png"}};
//const char ARCHER_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/arrow1.png"},{"bin/arrow1.png"},{"bin/arrow1.png"},{"bin/arrow2.png"},{"bin/arrow3.png"},{"bin/arrow4.png"},{"bin/arrow5.png"},{"bin/arrow6.png"},{"bin/arrow6.png"},{"bin/arrow6.png"}};
const char ARCHER_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/fire_arrow1.png"},{"bin/fire_arrow1.png"},{"bin/fire_arrow1.png"},{"bin/fire_arrow2.png"},{"bin/fire_arrow3.png"},{"bin/fire_arrow4.png"},{"bin/fire_arrow5.png"},{"bin/fire_arrow6.png"},{"bin/fire_arrow6.png"},{"bin/fire_arrow6.png"}};
#define SOLDIER_BLUE "bin/soldier_blue.png"
#define SOLDIER_RED "bin/soldier_red.png"

//defined in hero.h
class Hero;
enum Class{
    MAGE, SOLDIER, ARCHER
};
enum Team{
    ONE,TWO
};
//Find the position of rectangle, pixel to map point
Point find_rec(Pixel_Point* pixel){
    Point point;
    point.x = (int)(pixel->x-1)/SIZE_TILE+1;
    point.y = (int)(pixel->y-BAR_OPTIONS-1)/SIZE_TILE+1;
    return point;
}
//Find the position of rectangle, map point to pixel
Pixel_Point find_rec(Point* point){
    Pixel_Point pixel;
    pixel.x = 1+SIZE_TILE*(point->x-1);
    pixel.y = 1+SIZE_TILE*(point->y-1)+BAR_OPTIONS;
    return pixel;
}

class Animate{
    Image *heroSoldierRed;
    Image *heroSoldierBlue;
    Image *heroMageRed;
    Image *heroMageBlue;
    Image *imgMageAttack[IMGS_ANIMATE];
    Image *imgArcherAttack[IMGS_ANIMATE];
    Image *heroArcherRed;
    Image *heroArcherBlue;
public:
    Animate(){
        heroSoldierBlue = new Image(SOLDIER_BLUE);
        heroSoldierRed = new Image(SOLDIER_RED);
        heroMageBlue = new Image(MAGE_BLUE);
        heroMageRed = new Image(MAGE_RED);
        for(int i=0;i<IMGS_ANIMATE;i++){
            imgMageAttack[i] = new Image(MAGE_ATTACK[i]);
            imgArcherAttack[i] = new Image(ARCHER_ATTACK[i]);
        }
        heroArcherBlue = new Image(ARCHER_BLUE);
        heroArcherRed = new Image(ARCHER_RED);
    }
    ~Animate(){
        delete heroSoldierRed;
        delete heroSoldierBlue;
        delete heroMageRed;
        delete heroMageBlue;
        for(int i=0;i<IMGS_ANIMATE;i++){
            delete imgMageAttack[i];
            delete imgArcherAttack[i];
        }
        delete heroArcherRed;
        delete heroArcherBlue;
    }
    //return the bitmap of Class and Team defined
    Image *get_image(Class _class, Team team){
        if(_class==MAGE&&team==ONE)
            return heroMageBlue;
        else if(_class==MAGE&&team==TWO)
            return heroMageRed;
        else if(_class==SOLDIER&&team==ONE)
            return heroSoldierBlue;
        else if(_class==SOLDIER&&team==TWO)
            return heroSoldierRed;
        else if(_class==ARCHER&&team==ONE)
            return heroArcherBlue;
        else if(_class==ARCHER&&team==TWO)
            return heroArcherRed;
        else
            return NULL;
    }
    //return image and or position of animation of the class
    Image* animation(Class _class, Pixel_Point* attack,Pixel_Point* defender,int frame, Pixel_Point* imgPosition){
        //if class is a mage
        if(_class==MAGE){
            //set position of image
            imgPosition->x = defender->x;
            imgPosition->y = defender->y-100;
            //return image of animation
            if(frame>=IMGS_ANIMATE)
                return NULL;
            else
                return (imgMageAttack[frame]);
        }
        //if class is a Archer
        else if(_class==ARCHER){
            //set position of image
            int fit = (defender->x-attack->x)*(frame)/(IMGS_ANIMATE-1);
            imgPosition->x = attack->x+fit;
            //set side of arrow
            if(attack->x < defender->x)
                imgArcherAttack[frame]->side = RIGHT;
            else
                imgArcherAttack[frame]->side = LEFT;
            //arrow is up
            if((frame)<(IMGS_ANIMATE/2))
                imgPosition->y = attack->y-20*frame;
            //arrow is down
            else if((frame)>(IMGS_ANIMATE/2)){
                int maxHeight = attack->y-20*(IMGS_ANIMATE/2-1);
                imgPosition->y += (defender->y-maxHeight)/(IMGS_ANIMATE/2-1);
            }
            //return image of animation
            if(frame>=IMGS_ANIMATE)
                return NULL;
            else
                return (imgArcherAttack[frame]);
        }
        //if class is a Archer
        else if(_class==SOLDIER){

        }
        return NULL;
    }
};
class Tile{
    int type;
    ALLEGRO_COLOR colorRec;
    float mobility;
public:
    float weight;
    Tile* lastTile;
    Hero* hero;
    Hero* moveHero;
    Pixel_Point* pixel;
    Tile(int _x, int _y){
        //set the variables
        pixel = new Pixel_Point(_x,_y);//left up position, in pixels
        hero = NULL;
        moveHero = NULL;
        lastTile = NULL;
        mobility = 1;//============================================================VER MELHOR
        weight = WEIGHT_MAX;
        //set the color and the Left up position
        colorRec = BLACK;
    }
    ~Tile(){
        delete pixel;
    }
    float get_mobility(){
        return mobility;
    }
    void set_color(ALLEGRO_COLOR color){
        colorRec = color;
    }
    ALLEGRO_COLOR color(){
        return colorRec;
    }
};

class Map: public Image{
    const int columns, rows;
public:
    Tile*** tiles;
    Map(int _columns,int _rows,const char* directory):Image(directory), columns(_columns),rows(_rows){
        //allocate tiles
        tiles = new Tile**[rows];
        for(int i=0;i<rows;i++)
            tiles[i] = new Tile*[columns];
        for(int i=0;i<rows;i++)
            for(int j=0;j<columns;j++)
                //set position, all tiles have SIZE_TILE*SIZE_TILE pixels
                tiles[i][j] = new Tile(SIZE_TILE*j+1,BAR_OPTIONS+SIZE_TILE*i+1);
    }
    int get_columns(){
        return columns;
    }
    int get_rows(){
        return rows;
    }
    ~Map(){
        //deallocate tiles
        for(int i=0;i<rows;i++)
            for(int j=0;j<columns;j++)
                 delete tiles[i][j];
        for(int i=0;i<rows;i++)
            delete [] tiles[i];
        delete [] tiles;
    }
};

