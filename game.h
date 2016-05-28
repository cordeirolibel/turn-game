#define IMGS_ANIMATE 4
#define MAX_TEXT 50
#define MAX_HEROES 50
#define SIZE_IMG_ATTACK 100

//directories
#define PONTO "ponto.wav"
#define FUNDO "map.png"
#define CURSOR "cursor.png"
#define MENU "menu.png"
#define ARCHER_BLUE "archer_blue.png"
#define ARCHER_RED "archer_red.png"
#define MAGE_RED "mage_red.png"
#define MAGE_BLUE "mage_blue.png"
const char MAGEATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"lightning1.png"},{"lightning2.png"},{"lightning3.png"},{"lightning4.png"}};
#define MAGEATTACK1 "lightning1.png"
#define MAGEATTACK2 "lightning2.png"
#define MAGEATTACK3 "lightning3.png"
#define MAGEATTACK4 "lightning4.png"
#define SOLDIER_BLUE "soldier_blue.png"
#define SOLDIER_RED "soldier_red.png"
#define FONT1 "pirulen.ttf"
#define SIZE_FONT1 18


class Menu;

//converting the value in range fromLow-fromHigh to range toLow-toHigh proportionally
int  mapping(int value,int fromLow, int fromHigh, int toLow, int toHigh)
{
  return (int)((value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow);
}

class Game:public Screen{
    Font *font;
    Image *heroSoldierRed;
    Image *heroSoldierBlue;
    Image *heroMageRed;
    Image *heroMageBlue;
    Image *imgMageAttack[IMGS_ANIMATE];
    Image *heroArcherRed;
    Image *heroArcherBlue;
    Image *attackDraw;
    Pixel_Point* attackDrawPixel;
    Map* mapa;
    Menu* menu;
    Hero** heroes;
    Mouse *mouse;
public:

    Game(){
        attackDraw = NULL;
        attackDrawPixel = new Pixel_Point(0,0);
    }
    ~Game(){
         //deallocate memory
        delete mapa;
        delete menu;
        delete font;
        delete mouse;
        delete heroSoldierRed;
        delete heroSoldierBlue;
        delete heroMageRed;
        delete heroMageBlue;
        for(int i=0;i<IMGS_ANIMATE;i++)
            delete imgMageAttack[i];
        delete attackDrawPixel;
        delete heroArcherRed;
        delete heroArcherBlue;
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
    //the best point of attack
    Point attack_point(Point* attacker, Point* defender);
    //attack of heroes
    void attack(Tile* attacker, Tile* defender);
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
    heroSoldierBlue = new Image(SOLDIER_BLUE);
    heroSoldierRed = new Image(SOLDIER_RED);
    heroMageBlue = new Image(MAGE_BLUE);
    heroMageRed = new Image(MAGE_RED);
    for(int i=0;i<IMGS_ANIMATE;i++)
            imgMageAttack[i] = new Image(MAGEATTACK[i]);
    heroArcherBlue = new Image(ARCHER_BLUE);
    heroArcherRed = new Image(ARCHER_RED);
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
    //draw attack of heroes
    if(attackDraw!=NULL)
        al_draw_bitmap(attackDraw->get_bitmap(),attackDrawPixel->x,attackDrawPixel->y,0);
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
        al_draw_filled_rectangle(90,30,60 + pixels_hp,60,RED);
        al_draw_rectangle(90,30,61 + 200,61,WHITE,1);
        //print the text of hp
        char hp[MAX_TEXT];
        sprintf(hp,"HP : %d\\%d",hero->get_hp(),hero->get_max_hp());
        al_draw_text(font->get_font(), WHITE, 265, 45,ALLEGRO_ALIGN_LEFT, hp);
        //print the atk
        char atk[MAX_TEXT];
        sprintf(atk,"ATK : %d",hero->get_atk());
        al_draw_text(font->get_font(), WHITE, 510, 45,ALLEGRO_ALIGN_LEFT, atk);
        //print the evasion
        char evasion[MAX_TEXT];
        sprintf(evasion,"EVASION : %d",hero->get_evasion());
        al_draw_text(font->get_font(), WHITE, 650, 45,ALLEGRO_ALIGN_LEFT, evasion);
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
            //set which hero draw information in menu
            menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
            //the weight in start position is 0
            mapa->tiles[point.y-1][point.x-1]->weight = 0;
            //find the space of walk for the hero
            space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed(),mapa->tiles[point.y-1][point.x-1]->hero->get_team());
            //save the last click is a hero
            *heroFlag = true;
        }
        //in the last turn selected a hero and new selected hero in the same team
        else if(mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->get_team()==mapa->tiles[point.y-1][point.x-1]->hero->get_team()){
            //clear the range space
            clear_space_walk(mapa,*lastTileSelected);
            //set which hero draw information in menu
            menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
            //the weight in start position is 0
            mapa->tiles[point.y-1][point.x-1]->weight = 0;
            //find the space of walk for the hero
            space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed(),mapa->tiles[point.y-1][point.x-1]->hero->get_team());
        }
        //in the last turn selected a hero and new selected hero in the different team, battle
        else{
            //find the best point of attack
            Point atkPoint;
            atkPoint = attack_point(lastTileSelected, &point);
            //return null, do not have speed to attack
            if((atkPoint.x==0)&&(atkPoint.y==0)){
                //draw last rectangle and make anything
                mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(WHITE);
                mapa->tiles[point.y-1][point.x-1]->set_color(BLACK);
                return;
            }
            //move the hero
            move_hero(mapa->tiles[atkPoint.y-1][atkPoint.x-1], NULL);
            //clear the range space
            clear_space_walk(mapa,*lastTileSelected);
            //attack heroes
            attack(mapa->tiles[atkPoint.y-1][atkPoint.x-1],mapa->tiles[point.y-1][point.x-1]);
            //draw actual rectangle
            mapa->tiles[point.y-1][point.x-1]->set_color(BLACK);
            mapa->tiles[atkPoint.y-1][atkPoint.x-1]->set_color(WHITE);
            //save the last click position
            *lastTileSelected = atkPoint;
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
        space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed(),mapa->tiles[point.y-1][point.x-1]->hero->get_team());
    }
    //if the last click is a hero, and the new click out of the range of hero
    else if(*heroFlag){
        clear_space_walk(mapa,*lastTileSelected);
        *heroFlag = false;
        menu->set_hero(NULL);
    }
    //save the last click position
    *lastTileSelected = point;
}

//the best point of attack
Point Game::attack_point(Point* attacker, Point* defender){
    Point saida(0,0);
    //save the weights of proximity of the defender, not in limits
    float weightUp=WEIGHT_MAX,weightDown=WEIGHT_MAX,weightLeft=WEIGHT_MAX,weightRight=WEIGHT_MAX;
    if(defender->y>1)
        weightUp = mapa->tiles[defender->y-2][defender->x-1]->weight;
    if(defender->y<ROWS_TILE)
        weightDown = mapa->tiles[defender->y][defender->x-1]->weight;
    if(defender->x>1)
        weightLeft = mapa->tiles[defender->y-1][defender->x-2]->weight;
    if(defender->x<COLUMNS_TILE)
        weightRight = mapa->tiles[defender->y-1][defender->x-1]->weight;
    //if the attacker do not have access the defender
    if((weightUp==WEIGHT_MAX)&&(weightDown==WEIGHT_MAX)&&(weightLeft==WEIGHT_MAX)&&(weightRight==WEIGHT_MAX))
        return saida;//(0,0) is NULL
    //if weightUp is the smaller
    if((weightUp<=weightDown)&&(weightUp<=weightLeft)&&(weightUp<=weightRight)){
        //return the position
        saida.x=defender->x;
        saida.y=defender->y-1;
        return saida;
    }
    //if weightDown is the smaller
    else if((weightDown<=weightUp)&&(weightDown<=weightLeft)&&(weightDown<=weightRight)){
        //return the position
        saida.x=defender->x;
        saida.y=defender->y+1;
        return saida;
    }
    //if weightLeft is the smaller
    else if((weightLeft<=weightUp)&&(weightLeft<=weightDown)&&(weightLeft<=weightRight)){
        //return the position
        saida.x=defender->x-1;
        saida.y=defender->y;
        return saida;
    }
    //if weightRight is the smaller
    else{
        //return the position
        saida.x=defender->x+1;
        saida.y=defender->y;
        return saida;
    }
}

//attack of heroes
void Game::attack(Tile* attacker, Tile* defender){
    ALLEGRO_EVENT event;
    Image** imgAnimete;
    //if attacker is a mage
    if(attacker->hero->get_class()==MAGE)
        imgAnimete=imgMageAttack;
    //if attacker is a Archer
//    else if(attacker->hero->get_class()==ARCHER)
//        imgAnimete=imgArcherAttack;
    //===================Animate
    int contTime=0, i=0;
    //save the image position
    attackDrawPixel->x = defender->pixel->x;
    attackDrawPixel->y = defender->pixel->y-SIZE_IMG_ATTACK;
    while(i<IMGS_ANIMATE){
        //set the image of animation
        attackDraw = imgMageAttack[i];
        //wait a event
        event = wait_event();
        if(event.type == ALLEGRO_EVENT_TIMER){
            contTime++;
            //wait the time of animate
            if(contTime >= ATTACK_TIME){
                //next image to animate
                i++;
                contTime=0;
                //more time in the last image
                if(i==(IMGS_ANIMATE-1))
                    contTime -= ATTACK_TIME;
            }
            draw_update();
        }
        //mouse move
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
            move_mouse(event.mouse.x,event.mouse.y);
        }
        attackDraw = NULL;
    }

}

//function use recursion, animate the walk of hero
void Game::move_hero(Tile* actualTile, Tile* nextTile){
    //recursion for find the start point
    if(actualTile->weight!=0)
        move_hero(actualTile->lastTile,actualTile);
    //initial tile in the route
    else{
        //change to hero to moveHero, to no conflict in the route
        actualTile->moveHero = actualTile->hero;
        actualTile->hero = NULL;
    }
    //if is the end
    if(nextTile==NULL){
        //change to moveHero to hero, to no conflict in the route
        actualTile->hero = actualTile->moveHero;
        actualTile->moveHero = NULL;
        return;
    }
    //set the time of move and wait
    actualTile->moveHero->set_move();
    ALLEGRO_EVENT event;
    //wait the time of move
    while(!actualTile->moveHero->is_ready_to_move()){
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
    actualTile->moveHero->moves(find_rec(nextTile->pixel));
    nextTile->moveHero = actualTile->moveHero;
    actualTile->moveHero = NULL;
    //change the side of hero
    if(actualTile->pixel->x < nextTile->pixel->x)
        nextTile->moveHero->set_side(RIGHT);
    else if(nextTile->pixel->x < actualTile->pixel->x)
        nextTile->moveHero->set_side(LEFT);
}

//initialize the all heroes
void Game::init_heroes(){

    //TEAM 1

    //soldiers team 1
    int x=5,y=2;
    heroes[0] = new Hero(heroSoldierRed,imgMageAttack,x,y,50,10,25,7,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[0];
    y+=4;
    heroes[1] = new Hero(heroSoldierRed,imgMageAttack,x,y,50,10,30,7,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[1];
    y+=4;
    heroes[2] = new Hero(heroSoldierRed,imgMageAttack,x,y,50,10,30,7,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[2];
    y+=4;
    heroes[3] = new Hero(heroSoldierRed,imgMageAttack,x,y,50,10,30,7,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[3];
    y+=4;
    heroes[4] = new Hero(heroSoldierRed,imgMageAttack,x,y,50,10,30,7,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[4];
    y+=4;

    //mages team 1
    x=2,y=2;
    heroes[5] = new Hero(heroMageRed,imgMageAttack,x,y,50,10,25,7,RIGHT,ONE,MAGE);
    mapa->tiles[y-1][x-1]->hero = heroes[5];
    y+=16;
    heroes[6] = new Hero(heroMageRed,imgMageAttack,x,y,50,10,30,7,RIGHT,ONE,MAGE);
    mapa->tiles[y-1][x-1]->hero = heroes[6];

    //archers team 1
    x=2,y=6;
    heroes[7] = new Hero(heroArcherRed,imgMageAttack,x,y,50,10,25,7,RIGHT,ONE,ARCHER);
    mapa->tiles[y-1][x-1]->hero = heroes[7];
    y+=4;
    heroes[8] = new Hero(heroArcherRed,imgMageAttack,x,y,50,10,30,7,RIGHT,ONE,ARCHER);
    mapa->tiles[y-1][x-1]->hero = heroes[8];
    y+=4;
    heroes[9] = new Hero(heroArcherRed,imgMageAttack,x,y,50,10,30,7,RIGHT,ONE,ARCHER);
    mapa->tiles[y-1][x-1]->hero = heroes[9];


    //============================================================================================================//

    //TEAM 2

    //soldiers team 2
    x=35,y=2;
    heroes[10] = new Hero(heroSoldierBlue,imgMageAttack,x,y,50,10,25,7,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[10];
    y+=4;
    heroes[11] = new Hero(heroSoldierBlue,imgMageAttack,x,y,50,10,30,7,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[11];
    y+=4;
    heroes[12] = new Hero(heroSoldierBlue,imgMageAttack,x,y,50,10,30,7,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[12];
    y+=4;
    heroes[13] = new Hero(heroSoldierBlue,imgMageAttack,x,y,50,10,30,7,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[13];
    y+=4;
    heroes[14] = new Hero(heroSoldierBlue,imgMageAttack,x,y,50,10,30,7,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[14];
    y+=4;

    //mages team 2
    x=38,y=2;
    heroes[15] = new Hero(heroMageBlue,imgMageAttack,x,y,50,10,25,7,LEFT,TWO,MAGE);
    mapa->tiles[y-1][x-1]->hero = heroes[15];
    y+=16;
    heroes[16] = new Hero(heroMageBlue,imgMageAttack,x,y,50,10,30,7,LEFT,TWO,MAGE);
    mapa->tiles[y-1][x-1]->hero = heroes[16];

    //archers team 2
    x=38,y=6;
    heroes[17] = new Hero(heroArcherBlue,imgMageAttack,x,y,50,10,25,7,LEFT,TWO,ARCHER);
    mapa->tiles[y-1][x-1]->hero = heroes[17];
    y+=4;
    heroes[18] = new Hero(heroArcherBlue,imgMageAttack,x,y,50,10,30,7,LEFT,TWO,ARCHER);
    mapa->tiles[y-1][x-1]->hero = heroes[18];
    y+=4;
    heroes[19] = new Hero(heroArcherBlue,imgMageAttack,x,y,50,10,30,7,LEFT,TWO,ARCHER);
    mapa->tiles[y-1][x-1]->hero = heroes[19];

}
