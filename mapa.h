#define BAR_OPTIONS 80 //options bar start, in pixels
#define SIZE_TILE 32//in pixels
#define ROWS_TILE 20//in tiles
#define COLUMNS_TILE 40//in tiles

#define WATER_MOBILITY 5
#define GRASS_MOBILITY 1
#define DIRT_MOBILITY 2
#define BRIDGE_MOBILITY 1.5
#define STONE_MOBILITY WEIGHT_MAX
#define HOUSE_MOBILITY 1

#define WEIGHT_MAX 8000
#define MAX_TEXT 50
int map_matrix[20*40] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1,1,1,1,1,1,1,1,1,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1,1,1,1,1,1,1,1,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1,1,1,1,1,1,1,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,1,1,1,1,1,1,1,1,
                        2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1,1,1,1,1,
                        2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1,1,1,1,
                        2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1,1,1,
                        0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,1,1,1,
                        0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,2,2,2,0,0,1,1,1,
                        1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,1,1,
                        1,1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,1,
                        1,1,1,0,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,
                        1,1,1,1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,
                        1,1,1,1,1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,
                        1,1,1,1,1,1,0,0,2,2,2,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,
                        1,1,1,1,1,1,1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,2,
                        1,1,1,1,1,1,1,1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        1,1,1,1,1,1,1,1,1,0,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        1,1,1,1,1,1,1,1,1,1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        1,1,1,1,1,1,1,1,1,1,1,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//defined in hero.h
class Hero;
enum Class{
    MAGE, SOLDIER, ARCHER, TOWER
};
enum Team{
    ONE=1,TWO
};
enum Terrain{
    GRASS,DIRT,WATER,BRIGDE, STONE, HOUSE
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
    ALLEGRO_COLOR colorRec;
public:
    float mobility;
    float weight;
    float weightAtk;
    Tile* lastTile;
    Hero* hero;
    Hero* moveHero;
    Pixel_Point* pixel;
    Terrain terrain;
    Tile(int _x, int _y){
        //set the variables
        pixel = new Pixel_Point(_x,_y);//left up position, in pixels
        hero = NULL;
        moveHero = NULL;
        lastTile = NULL;
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
    void set_mobility(){
        for(int i=0;i<rows;i++){
            for(int j=0;j<columns;j++){
                if(map_matrix[i*columns+j] == 0){
                    //grass tile
                    tiles[i][j]->mobility = GRASS_MOBILITY;
                    tiles[i][j]->terrain = GRASS;
                }
                if(map_matrix[i*columns+j] == 1){
                    //grass tile
                    tiles[i][j]->mobility = DIRT_MOBILITY;
                    tiles[i][j]->terrain = DIRT;
                }
                if(map_matrix[i*columns+j] == 2){
                    //grass tile
                    tiles[i][j]->mobility = WATER_MOBILITY;
                    tiles[i][j]->terrain = WATER;
                }
                if(map_matrix[i*columns+j] == 3){
                    //grass tile
                    tiles[i][j]->mobility = BRIDGE_MOBILITY;
                    tiles[i][j]->terrain = BRIGDE;
                }
                if(map_matrix[i*columns+j] == 4){
                    //grass tile
                    tiles[i][j]->mobility = STONE_MOBILITY;
                    tiles[i][j]->terrain = STONE;
                }
                if(map_matrix[i*columns+j] == 5){
                    //grass tile
                    tiles[i][j]->mobility = HOUSE_MOBILITY;
                    tiles[i][j]->terrain = HOUSE;
                }
            }
        }
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
