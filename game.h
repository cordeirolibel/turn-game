//directories
#define PONTO "ponto.wav"
#define FUNDO "map.png"
#define CURSOR "cursor.png"
#define MENU "menu.png"
#define HERO "hero.png"
#define FONT1 "pirulen.ttf"
#define SIZE_FONT1 14

#define MAX_TEXT 50
#define MAX_HEROES 50

class Menu;

//converting the value in range fromLow-fromHigh to range toLow-toHigh proportionally
int  mapping(int value,int fromLow, int fromHigh, int toLow, int toHigh)
{
  return (int)((value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow);
}

class Game:public Screen{
    Font *font;
    Image *hero_soldier;
    Map* mapa;
    Menu* menu;
    Hero** heroes;
    Mouse *mouse;
public:

    Game(){
    }
    ~Game(){
         //deallocate memory
        delete mapa;
        delete menu;
        delete font;
        delete mouse;
        delete hero_soldier;
        //deallocate heroes
        int n_heroes = Hero::get_num_of_heroes();
        for(int i=0;i<n_heroes;i++)
            delete heroes[i];
        delete heroes;
    }
    //initialize the initial configurations
    int initialize();
    //initialize the all heroes
    void init_heroes();
    //update the screen with new informations
    void draw_update();
    //draw the menu informations
    void draw_menu();
    //draw the grid of map
    void draw_rectangles();
    //updates game when click in tile point
    void tile_click(Point point,Point* lastTileSelected,bool* heroFlag);
    //function use recursion, animate the walk of hero
    void move_hero(Tile* actualTile, Tile* nextTile);
    //if the hero is move, animate the walk
    void move_heroes (){
        for(int i=0;i<Hero::get_num_of_heroes(); i++)
            heroes[i]->cont_move();
    }
    //move the mouse position
    void move_mouse(int x, int y){
        mouse->moves(x,y);
    }
    //return the mouse position
    Pixel_Point get_mouse_pixel(){
        return *mouse->get_pixel();
    }
};

int Game::initialize(){
    //initialize screen
    if(Screen::initialize())
        return 1;
    //open the files
    font = new Font(FONT1,SIZE_FONT1);
    mouse = new Mouse(CURSOR);
    hero_soldier = new Image(HERO);
    mapa = new Map(COLUMNS_TILE,ROWS_TILE, FUNDO);
    heroes = new Hero*[MAX_HEROES];
    menu = new Menu(MENU);
    //create the heroes
    init_heroes();
    return 0;
}
//update the screen with new informations
void Game::draw_update(){
    //clear the buffer of screen
    clear_screen();
    //print map
    al_draw_bitmap(mapa->get_bitmap(),0,BAR_OPTIONS,0);
    //print menu bar
    draw_menu();
    //draw all heroes
    for(int i=0;i<Hero::get_num_of_heroes(); i++)
        heroes[i]->draw_hero();
    //print all rectangles
    draw_rectangles();
    //print cursor
    al_draw_bitmap(mouse->get_bitmap(), mouse->get_x(),mouse->get_y(),0);
    //flip the screen
    update_screen();
}
//draw all configuration of menu
void Game::draw_menu()
{
    //draw image of menu
    al_draw_bitmap(menu->get_bitmap(),0,0,0);
    Hero *hero = menu->get_hero();
    //if hero is selected
    if(hero != NULL)
    {
        //print the rectangle of hp
        int pixels_hp = mapping(hero->get_hp(),0,hero->get_max_hp(),0,200);
        al_draw_filled_rectangle(60,30,60 + pixels_hp,60,RED);
        al_draw_rectangle(60,30,61 + 200,61,WHITE,1);
        //print the text of hp
        char text[MAX_TEXT];
        sprintf(text,"%d\\%d",hero->get_hp(),hero->get_max_hp());
        al_draw_text(font->get_font(), WHITE, 265, 45,ALLEGRO_ALIGN_LEFT, text);
    }
}

//draw the grid of map
void Game::draw_rectangles(){
    //print all rectangles
    for(int i=0;i<mapa->get_rows();i++)
        for(int j=0;j<mapa->get_columns();j++)
            al_draw_rectangle(mapa->tiles[i][j]->pixel->x,mapa->tiles[i][j]->pixel->y,mapa->tiles[i][j]->pixel->x+SIZE_TILE-1,mapa->tiles[i][j]->pixel->y+SIZE_TILE-1,mapa->tiles[i][j]->color(),1);
}

//updates mapa when click in tile point
void Game::tile_click(Point point,Point* lastTileSelected,bool* heroFlag){
    //clicked in the same position, do anything
    if((lastTileSelected->x == point.x)&&(lastTileSelected->y == point.y))
        return;
    //clear any tile selected
    mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
    //draw actual rectangle
    mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
    //if a hero into the tile
    if(mapa->tiles[point.y-1][point.x-1]->hero != NULL){
        //in the last turn no selected a hero
        if(!*heroFlag){
            menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
            //the weight in start position is 0
            mapa->tiles[point.y-1][point.x-1]->weight = 0;
            //find the space of walk for the hero
            space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed());
            //save the last click is a hero
            *heroFlag = true;
        }
        //in the last turn selected a hero and new selected hero
        else{
            //draw last rectangle
            mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(WHITE);
            return;
        }
    }
    //if the last click is a hero, and the new click in the range of hero
    else if((*heroFlag)&&(mapa->tiles[point.y-1][point.x-1]->weight<WEIGHT_MAX)){
        //move the hero
        move_hero(mapa->tiles[point.y-1][point.x-1], NULL);
        //clear the range space
        clear_space_walk(mapa,*lastTileSelected);
        //draw actual rectangle
        mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
        //the weight in start position is 0
        mapa->tiles[point.y-1][point.x-1]->weight = 0;
        //find the space of walk for the hero
        space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed());
    }
    //if the last click is a hero, and the new click out of the range of hero
    else if(*heroFlag){
        clear_space_walk(mapa,*lastTileSelected);
        *heroFlag = false;
        menu->set_hero(NULL);
    }
    //save the last click position
    lastTileSelected->x = point.x;
    lastTileSelected->y = point.y;
}

//function use recursion, animate the walk of hero
void Game::move_hero(Tile* actualTile, Tile* nextTile){
    //recursion for find the start point
    if(actualTile->weight!=0)
        move_hero(actualTile->lastTile,actualTile);
    //if is the end
    if(nextTile==NULL)
        return;
    //set the time of move and wait
    actualTile->hero->set_move();

    ALLEGRO_EVENT event;
    //wait the time of move
    while(!actualTile->hero->is_ready_to_move()){
        event = wait_event();
        //Event of time, defined by FPS
        if(event.type == ALLEGRO_EVENT_TIMER){
            move_heroes();
            draw_update();
        }
        //mouse move
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
            move_mouse(event.mouse.x,event.mouse.y);
        }
    };
    //move the hero
    actualTile->hero->moves(find_rec(nextTile->pixel));
    nextTile->hero = actualTile->hero;
    actualTile->hero = NULL;
}

//initialize the all heroes
void Game::init_heroes(){
    int x=2,y=2;
    //soldier 1 team 1
    heroes[0] = new Hero(hero_soldier,x,y,50,7.2,UM);
    mapa->tiles[y-1][x-1]->hero = heroes[0];
    y+=2;
    //soldier 2 team 1
    heroes[1] = new Hero(hero_soldier,x,y,50,7,UM);
    mapa->tiles[y-1][x-1]->hero = heroes[1];
    y+=2;
    //soldier 3 team 1
    heroes[2] = new Hero(hero_soldier,x,y,50,7,UM);
    mapa->tiles[y-1][x-1]->hero = heroes[2];
    y+=2;
    //soldier 4 team 1
    heroes[3] = new Hero(hero_soldier,x,y,50,7,UM);
    mapa->tiles[y-1][x-1]->hero = heroes[3];
    y+=2;
    //soldier 5 team 1
    heroes[4] = new Hero(hero_soldier,x,y,50,7,UM);
    mapa->tiles[y-1][x-1]->hero = heroes[4];
    y+=2;
    //==============//
    //TEAM2
    x=38;
    y=2;
    //soldier 1 team 2
    heroes[5] = new Hero(hero_soldier,x,y,50,7,DOIS);
    mapa->tiles[y-1][x-1]->hero = heroes[5];
    y+=2;
   //soldier 2 team 2
    heroes[6] = new Hero(hero_soldier,x,y,50,7,DOIS);
    mapa->tiles[y-1][x-1]->hero = heroes[6];
    y+=2;
    //soldier 3 team 2
    heroes[7] = new Hero(hero_soldier,x,y,50,7,DOIS);
    mapa->tiles[y-1][x-1]->hero = heroes[7];
    y+=2;
    //soldier 4 team 2
    heroes[8] = new Hero(hero_soldier,x,y,50,7,DOIS);
    mapa->tiles[y-1][x-1]->hero = heroes[8];
    y+=2;
    //soldier 5 team 2
    heroes[9] = new Hero(hero_soldier,x,y,50,7,DOIS);
    mapa->tiles[y-1][x-1]->hero = heroes[9];
    y+=2;
}
