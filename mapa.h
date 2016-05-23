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
    Pixel_Point* pixel;
    Tile(int _x, int _y){
        //set the variables
        pixel = new Pixel_Point(_x,_y);//left up position, in pixels
        hero = NULL;
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

//Find the walk space in mapa, stating in point and finish when WEIGHT_MAX it is reached
//Use the recursion and based on algorithm dijkstra
void space_walk(Map *mapa,Point point, float weight_max){
    float newWeight;
    Tile* tileCenter = mapa->tiles[point.y-1][point.x-1];
    Tile* tileAnalyze;
    //tile in Left, if not limit
    if(point.x-1>0){
        tileAnalyze = mapa->tiles[point.y-1][point.x-2];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x-1,point.y);
            space_walk(mapa,new_point,weight_max);
        }
    }
    //tile in Right, if not limit
    if(point.x<COLUMNS_TILE){
        tileAnalyze = mapa->tiles[point.y-1][point.x];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x+1,point.y);
            space_walk(mapa,new_point,weight_max);
        }
    }
    //tile in Up, if not limit
    if(point.y-1>0){
        tileAnalyze = mapa->tiles[point.y-2][point.x-1];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x,point.y-1);
            space_walk(mapa,new_point,weight_max);
        }
    }
    //tile in Down, if not limit
    if(point.y<ROWS_TILE){
        tileAnalyze = mapa->tiles[point.y][point.x-1];
        //the weight for walk to here and the mobility int the tile actual
        newWeight = tileCenter->weight + tileAnalyze->get_mobility();
        //if not exist way more easy and not the limit of the speed of the hero
        if((newWeight<tileAnalyze->weight)&&(newWeight<=weight_max)){
            //save the way for the tile and the new weight and color
            tileAnalyze->lastTile = tileCenter;
            tileAnalyze->weight = newWeight;
            tileAnalyze->set_color(WHITE);
            //new analysis of the space, starting the analyze tile
            Point new_point(point.x,point.y+1);
            space_walk(mapa,new_point,weight_max);
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
