#define BAR_OPTIONS 80 //options bar start, in pixels
#define SIZE_TILE 32//in pixels
#define ROWS_TILE 20//in tiles
#define COLUMNS_TILE 40//in tiles

#define WEIGHT_MAX 8000
#define MAX_TEXT 50

//defined in hero.h
class Hero;
enum Class{
    MAGE, SOLDIER, ARCHER
};
enum Team{
    ONE=1,TWO
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

class Tile{
    int type;
    ALLEGRO_COLOR colorRec;
    float mobility;
public:
    float weight;
    float weightAtk;
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
        weightAtk=WEIGHT_MAX;
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

class Map{
    const int columns, rows;
public:
    Tile*** tiles;
    Map(int _columns,int _rows): columns(_columns),rows(_rows){
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
    //return the pixel point of the Hero
    Pixel_Point get_hero_pixel_point(Hero* hero);
};
