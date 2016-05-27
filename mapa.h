#define BAR_OPTIONS 80 //options bar start, in pixels
#define SIZE_TILE 32
#define ROWS_TILE 20
#define COLUMNS_TILE 40

#define WEIGHT_MAX 8000

//defined in hero.h
class Hero;

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

