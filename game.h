//directories
#define PONTO "ponto.wav"
#define FUNDO "map.png"
#define CURSOR "cursor.png"
#define MENU "menu.png"
#define HERO "hero.png"
#define FONT1 "pirulen.ttf"
#define SIZE_FONT1 8

#define MAX_HEROES 50

class Game:public Screen{
    Font *font;
    Image *menu;
    Image *hero_img;
    Map* mapa;
    Hero** heroes;
public:
    Mouse *mouse;
    Game(){
    }
    int initialize(){
        //initialize screen
        if(Screen::initialize())
            return 1;
        //open the files
        font = new Font(FONT1,SIZE_FONT1);
        mouse = new Mouse(CURSOR);
        menu = new Image(MENU);
        hero_img = new Image(HERO);
        mapa = new Map(COLUMNS_TILE,ROWS_TILE, FUNDO);
        heroes = new Hero*[MAX_HEROES];
        //======================================TEST - DELETAR
        int x=2,y=2;
        heroes[0] = new Hero(8.2,x,y,hero_img);
        mapa->tiles[y-1][x-1]->hero = heroes[0];
        //======================================
        return 0;
    }
    ~Game(){
         //deallocate memory
        delete mapa;
        delete menu;
        delete font;
        delete mouse;
        delete hero_img;
    }
    //update the screen with new informations
    void draw_update(){
        //clear the buffer of screen
        clear_screen();
        //print map
        al_draw_bitmap(mapa->get_bitmap(),0,BAR_OPTIONS,0);
        //print menu bar
        al_draw_bitmap(menu->get_bitmap(),0,0,0);
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
    //draw the grid of map
    void draw_rectangles(){
        //print all rectangles
        for(int i=0;i<mapa->get_rows();i++)
            for(int j=0;j<mapa->get_columns();j++)
                al_draw_rectangle(mapa->tiles[i][j]->pixel->x,mapa->tiles[i][j]->pixel->y,mapa->tiles[i][j]->pixel->x+SIZE_TILE-1,mapa->tiles[i][j]->pixel->y+SIZE_TILE-1,mapa->tiles[i][j]->color(),1);
    }
    //if the hero is move, animate the walk
    void move_heroes (){
        for(int i=0;i<Hero::get_num_of_heroes(); i++)
            heroes[i]->cont_move();
    }
    //function use recursion, animate the walk of hero
    void move_hero(Tile* actualTile, Tile* nextTile){
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
    void move_mouse(int x, int y){
        mouse->moves(x,y);
    }
    //updates mapa when click in tile point
    void tile_click(Point point,Point* lastTileSelected,bool* heroFlag){
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
                //the weight in start position is 0
                mapa->tiles[point.y-1][point.x-1]->weight = 0;
                //find the space of walk for the hero
                space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed());
                //save the last click is a hero
                *heroFlag = true;
            }
            //in the last turn selected a hero and new selected hero
            else{

            }
        }
        //if the last click is a hero, and the new click in the range of hero
        else if((*heroFlag)&&(mapa->tiles[point.y-1][point.x-1]->weight<WEIGHT_MAX)){
            //move the hero
            move_hero(mapa->tiles[point.y-1][point.x-1], NULL);
            //clear the range space
            clear_space_walk(mapa,*lastTileSelected);
        }
        //if the last click is a hero, and the new click out of the range of hero
        else if(*heroFlag){
            clear_space_walk(mapa,*lastTileSelected);
            *heroFlag = false;
        }
        //save the last click position
        lastTileSelected->x = point.x;
        lastTileSelected->y = point.y;
    }
};
