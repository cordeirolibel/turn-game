#include <time.h>

#define MAX_HEROES 50

//directories
#define FONT_BIG "bin/Carnevalee Freakshow.ttf"
#define SIZE_FONT_BIG 32
#define FONT_MEDIUM "bin/pirulen.ttf"
#define SIZE_FONT_MEDIUM 16

class Menu;

//converting the value in range fromLow-fromHigh to range toLow-toHigh proportionally
int mapping(int value,int fromLow, int fromHigh, int toLow, int toHigh){
  return (int)((value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow);
}

class Game:public Screen{
    Font *font_big;
    Font *font_medium;
    Map* mapa;
    Menu* menu;
    Hero** heroes;
    Mouse *mouse;
    Image *attackDraw;
    Animate *animate;
    Sounds* sounds;
    Pixel_Point* attackDrawPixel;
    Point* lastTileSelected;
    Team turnTeam;
    bool heroFlag;
public:
    Game(){
    }
    ~Game(){
         //deallocate memory
        delete mapa;
        delete menu;
        delete font_big;
        delete font_medium;
        delete mouse;
        delete animate;
        delete sounds;
        delete lastTileSelected;
        delete attackDrawPixel;
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
    //draw all heroes and informations
    void draw_heroes();
    //draw the grid of map
    void draw_rectangles();
    //draw all targets
    void draw_targets();
    //updates game when click in tile point
    void tile_click(Point point);
    //update the game for next turn
    void next_turn();
    //click in menu
    bool menu_click(Pixel_Point pixel){
        return menu->click(pixel);
    }
    //depress the click
    void depress(){
        menu->depress();
    }
    //the best point of attack
    Point attack_point(Point* attacker, Point* defender);
    //attack of heroes, return true if is possible the retaliates
    bool attack(Tile* attacker, Tile* defender);
    //function use recursion, animate the walk of hero
    void move_hero(Tile* actualTile, Tile* nextTile);
    //if the hero is move, animate the walk
    void move_heroes (){
        for(int i=0;i<Hero::get_num_of_heroes(); i++)
            heroes[i]->cont_move();
    }
    //delete hero of heroes
    int delete_hero();
    //check if game is finish
    bool finish();
    //move the mouse position
    void move_mouse(int x, int y){
        mouse->moves(x,y);
    }
    //buffer to screen
    void update_screen(){
        al_flip_display();
    }
    //return the mouse position
    Pixel_Point get_mouse_pixel(){
        return mouse->get_pixel();
    }
};

int Game::initialize(){
    //initialize screen
    if(Screen::initialize())
        return -1;
    //initialize random seed
    srand(time(NULL));
    //initialize variables
    attackDraw = NULL;
    attackDrawPixel = new Pixel_Point(0,0);
    animate = new Animate;
    sounds = new Sounds;
    lastTileSelected = new Point(1,1);
    //initialize sounds
    if(sounds->initialize())
        return -1;
    //set which team is ini
    int rando = rand()%2;
    if(rando)
        turnTeam = ONE;
    else
        turnTeam = TWO;
    //open the files
    font_big = new Font(FONT_BIG,SIZE_FONT_BIG);
    font_medium = new Font(FONT_MEDIUM,SIZE_FONT_MEDIUM);
    mouse = new Mouse(CURSOR);
    mapa = new Map(COLUMNS_TILE,ROWS_TILE);
    heroes = new Hero*[MAX_HEROES];
    menu = new Menu();
    //create the heroes
    init_heroes();
    for(int i=0;i<Hero::get_num_of_heroes(); i++){
        if(heroes[i]->get_team()==turnTeam)
            heroes[i]->set_attack_flag(false);
        else
            heroes[i]->set_attack_flag(true);
        heroes[i]->reset_speed(turnTeam);
    }
    return 0;
}
//update the screen with new informations
void Game::draw_update(){
    //clear the buffer of screen
    clear_screen();
    //print map
    al_draw_bitmap(animate->get_image("map")->get_bitmap(),0,BAR_OPTIONS,0);
    //print menu bar
    menu->draw_menu(font_big);
    //print all rectangles
    draw_rectangles();
    //draw all heroes and informations
    draw_heroes();
    //draw the targets
    draw_targets();
    //print cursor
    al_draw_bitmap(animate->get_image("cursor")->get_bitmap(), mouse->get_x(),mouse->get_y(),0);
    //flip the screen
    update_screen();
}
//draw all heroes and informations
void Game::draw_heroes(){
    int damage;
    //draw all heroes
    for(int i=0;i<Hero::get_num_of_heroes(); i++)
        heroes[i]->draw_hero();
    //draw attack of heroes
    if(attackDraw!=NULL)
        al_draw_bitmap(attackDraw->get_bitmap(),attackDrawPixel->x,attackDrawPixel->y,attackDraw->side);
    //draw the damage of all heroes
    for(int i=0;i<Hero::get_num_of_heroes(); i++){
        //if hero has received a damage
        damage = heroes[i]->get_damage();
        if(damage>0){
            //draw damage in hero
            Pixel_Point pixel = find_rec(heroes[i]->get_point());
            char text[MAX_TEXT];
            sprintf(text,"%d",damage);
            al_draw_text(font_medium->get_font(), RED, pixel.x+SIZE_TILE, pixel.y,ALLEGRO_ALIGN_RIGHT, text);
        }
    }

}

//draw the grid of map
void Game::draw_rectangles(){
    //print all rectangles
    for(int i=0;i<mapa->get_rows();i++)
        for(int j=0;j<mapa->get_columns();j++)
            al_draw_rectangle(mapa->tiles[i][j]->pixel->x,mapa->tiles[i][j]->pixel->y,mapa->tiles[i][j]->pixel->x+SIZE_TILE-1,mapa->tiles[i][j]->pixel->y+SIZE_TILE-1,mapa->tiles[i][j]->color(),1);
}
//draw the all targets
void Game::draw_targets(){
    for(int i=0;i<mapa->get_rows();i++)
        for(int j=0;j<mapa->get_columns();j++)
            if((mapa->tiles[i][j]->weightAtk<WEIGHT_MAX)&&(mapa->tiles[i][j]->weightAtk!=0)){
                if(mapa->tiles[i][j]->hero==NULL)
                    al_draw_bitmap(animate->get_image("target")->get_bitmap(),mapa->tiles[i][j]->pixel->x,mapa->tiles[i][j]->pixel->y,0);
                else if(mapa->tiles[i][j]->hero->get_team()!=turnTeam)
                    al_draw_bitmap(animate->get_image("target2")->get_bitmap(),mapa->tiles[i][j]->pixel->x,mapa->tiles[i][j]->pixel->y,0);
            }
}

//updates mapa when click in tile point
void Game::tile_click(Point point){
    //clicked in the same position, do anything
    if((lastTileSelected->x == point.x)&&(lastTileSelected->y == point.y))
        return;
    //if a hero into the tile
    if(mapa->tiles[point.y-1][point.x-1]->hero != NULL ){
        //hero is the same team of turnTeam
        if(mapa->tiles[point.y-1][point.x-1]->hero->get_team()==turnTeam){
            //in the last turn no selected a hero
            if(!heroFlag){
                //clear any tile selected
                mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
                //draw actual rectangle
                mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
                //set which hero draw information in menu
                menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
                //the weight in start position is 0
                mapa->tiles[point.y-1][point.x-1]->weight = 0;
                //find the space of walk for the hero
                space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed(),mapa->tiles[point.y-1][point.x-1]->hero->get_team());
                //if the attack is long distance
                if(type_attack(mapa->tiles[point.y-1][point.x-1]->hero->get_class())){
                    //the weightAtk in start position is 0
                    mapa->tiles[point.y-1][point.x-1]->weightAtk = 0;
                    //range of attack
                    space_walk(mapa,point,0.0,mapa->tiles[point.y-1][point.x-1]->hero->get_team(),mapa->tiles[point.y-1][point.x-1]->hero->get_range_atk());
                }
                //save the last click is a hero
                heroFlag = true;
            }
            //last hero and the new hero is the same team
            else if(mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->get_team()==mapa->tiles[point.y-1][point.x-1]->hero->get_team()){
                //clear the range space
                clear_space_walk(mapa,*lastTileSelected);
                //clear any tile selected
                mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
                //draw actual rectangle
                mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
                //set which hero draw information in menu
                menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
                //the weight in start position is 0
                mapa->tiles[point.y-1][point.x-1]->weight = 0;
                //find the space of walk for the hero
                space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed(),mapa->tiles[point.y-1][point.x-1]->hero->get_team());
                //if the attack is long distance
                if(type_attack(mapa->tiles[point.y-1][point.x-1]->hero->get_class())){
                    //the weightAtk in start position is 0
                    mapa->tiles[point.y-1][point.x-1]->weightAtk = 0;
                    //range of attack
                    space_walk(mapa,point,0.0,mapa->tiles[point.y-1][point.x-1]->hero->get_team(),mapa->tiles[point.y-1][point.x-1]->hero->get_range_atk());
                }
            }
            //in the last turn selected a hero and new selected hero in the different team
            else{
                //clear any tile selected
                mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
                //draw actual rectangle
                mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
                //set which hero draw information in menu
                menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
                //the weight in start position is 0
                mapa->tiles[point.y-1][point.x-1]->weight = 0;
                //find the space of walk for the hero
                space_walk(mapa,point,mapa->tiles[point.y-1][point.x-1]->hero->get_speed(),mapa->tiles[point.y-1][point.x-1]->hero->get_team());
                //if the attack is long distance
                if(type_attack(mapa->tiles[point.y-1][point.x-1]->hero->get_class())){
                    //the weightAtk in start position is 0
                    mapa->tiles[point.y-1][point.x-1]->weightAtk = 0;
                    //range of attack
                    space_walk(mapa,point,0.0,mapa->tiles[point.y-1][point.x-1]->hero->get_team(),mapa->tiles[point.y-1][point.x-1]->hero->get_range_atk());
                }
                //save the last click is a hero
                heroFlag = true;
            }
        }
        //hero is not the same team of turnTeam
        else{
            //in the last turn no selected a hero
            if(!heroFlag){
                //clear any tile selected
                mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
                //draw actual rectangle
                mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
                //set which hero draw information in menu
                menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
                //save the last click is a hero
                heroFlag = true;
            }
            //last hero and the new hero is the same team
            else if(mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->get_team()==mapa->tiles[point.y-1][point.x-1]->hero->get_team()){
                //clear any tile selected
                mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
                //draw actual rectangle
                mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
                //set which hero draw information in menu
                menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
            }
            //in the last turn selected a hero and new selected hero in the different team, battle
            else{
                //if is long distance
                if(type_attack(mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->get_class())){
                    //do not have speed to attack (out of range)
                   if((mapa->tiles[point.y-1][point.x-1]->weightAtk==WEIGHT_MAX)||mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->get_attack_flag()){
                        //clear the range space
                        clear_space_walk(mapa,*lastTileSelected);
                        //clear any tile selected
                        mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
                        //draw actual rectangle
                        mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
                        //set which hero draw information in menu
                        menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
                   }
                   //have range to attack
                   else{
                        //change the side of look heroes, for the battle
                        //attacker in the right
                        if(point.x<lastTileSelected->x){
                            //set your sides
                            mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->set_side(LEFT);
                            mapa->tiles[point.y-1][point.x-1]->hero->set_side(RIGHT);
                        }
                        //attacker in the left
                        else if(lastTileSelected->x<point.x){
                            //set your sides
                            mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->set_side(RIGHT);
                            mapa->tiles[point.y-1][point.x-1]->hero->set_side(LEFT);
                        }
                        //clear the range space
                        clear_space_walk(mapa,*lastTileSelected);
                        //set the hero is attacker in this turn
                        mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->set_attack_flag(true);
                        //attack heroes
                        if(attack(mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1],mapa->tiles[point.y-1][point.x-1]))
                            //retaliates
                            attack(mapa->tiles[point.y-1][point.x-1],mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]);
                        //draw actual rectangle
                        mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(WHITE);
                        return;
                   }
                }
                //if is short distance
                else {
                    //find the best point of attack
                    Point atkPoint;
                    atkPoint = attack_point(lastTileSelected, &point);
                    //return null(0,0), do not have speed to attack (out of range)
                    if(((atkPoint.x==0)&&(atkPoint.y==0))||mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->hero->get_attack_flag()){
                        //clear the range space
                        clear_space_walk(mapa,*lastTileSelected);
                        //clear any tile selected
                        mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
                        //draw actual rectangle
                        mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
                        //set which hero draw information in menu
                        menu->set_hero(mapa->tiles[point.y-1][point.x-1]->hero);
                    }
                    //have speed to attack
                    else{
                        //move the hero
                        move_hero(mapa->tiles[atkPoint.y-1][atkPoint.x-1], NULL);
                        //change the side of look heroes, for the battle
                        //attacker in the right
                        if(point.x<atkPoint.x){
                            //set your sides
                            mapa->tiles[atkPoint.y-1][atkPoint.x-1]->hero->set_side(LEFT);
                            mapa->tiles[point.y-1][point.x-1]->hero->set_side(RIGHT);
                        }
                        //attacker in the left
                        else if(atkPoint.x<point.x){
                            //set your sides
                            mapa->tiles[atkPoint.y-1][atkPoint.x-1]->hero->set_side(RIGHT);
                            mapa->tiles[point.y-1][point.x-1]->hero->set_side(LEFT);
                        }
                        //clear the range space
                        clear_space_walk(mapa,*lastTileSelected);
                        //set the hero is attacker in this turn
                        mapa->tiles[atkPoint.y-1][atkPoint.x-1]->hero->set_attack_flag(true);
                        //attack heroes
                        if(attack(mapa->tiles[atkPoint.y-1][atkPoint.x-1],mapa->tiles[point.y-1][point.x-1]))
                            //retaliates
                            attack(mapa->tiles[point.y-1][point.x-1],mapa->tiles[atkPoint.y-1][atkPoint.x-1]);
                        //draw actual rectangle
                        mapa->tiles[atkPoint.y-1][atkPoint.x-1]->set_color(WHITE);
                        //save the last click position
                        *lastTileSelected = atkPoint;
                        return;
                    }
                }
            }
        }
    }
    //if do not have a hero into this tile
    else{
        //in the last turn no selected a hero
        if(!heroFlag){
            //clear any tile selected
            mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
            //draw actual rectangle
            mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
        }
        //new click in the range of the hero
        else if(mapa->tiles[point.y-1][point.x-1]->weight<WEIGHT_MAX){
            //clear any tile selected
            mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
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
            //if the attack is long distance
            if(type_attack(mapa->tiles[point.y-1][point.x-1]->hero->get_class())){
                //the weightAtk in start position is 0
                mapa->tiles[point.y-1][point.x-1]->weightAtk = 0;
                //range of attack
                space_walk(mapa,point,0.0,mapa->tiles[point.y-1][point.x-1]->hero->get_team(),mapa->tiles[point.y-1][point.x-1]->hero->get_range_atk());
            }
        }
        //click out of the range of hero
        else {
            //clear all information of the hero in menu and clear your space walk
            clear_space_walk(mapa,*lastTileSelected);
            heroFlag = false;
            menu->set_hero(NULL);
            //clear any tile selected
            mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
            //draw actual rectangle
            mapa->tiles[point.y-1][point.x-1]->set_color(WHITE);
        }
    }
    //save the last click position
    *lastTileSelected = point;
}
//update the game for next turn
void Game::next_turn(){
    //sound of button click
    sounds->play("button");
    //clear any tile selected
    mapa->tiles[lastTileSelected->y-1][lastTileSelected->x-1]->set_color(BLACK);
    //in the last click selected a hero
    if(heroFlag){
        //clear all information of the hero in menu and clear your space walk
        clear_space_walk(mapa,*lastTileSelected);
        heroFlag = false;
        menu->set_hero(NULL);
    }
    //reset lastTileSelected
    lastTileSelected->x=1;
    lastTileSelected->y=1;
    //swap the turnTeam
    if(turnTeam==ONE)
        turnTeam = TWO;
    else
        turnTeam = ONE;
    //set anything hero is attacker
    for(int i=0;i<Hero::get_num_of_heroes();i++){
        if(heroes[i]->get_team()==turnTeam)
            heroes[i]->set_attack_flag(false);
        else
            heroes[i]->set_attack_flag(true);
        heroes[i]->reset_speed(turnTeam);
    }
}

//the best point of attack
Point Game::attack_point(Point* attacker, Point* defender){
    Point saida(0,0);
    //if neighbors
    if( (((attacker->x)==(defender->x+1))&&((attacker->y)==(defender->y)))  ||
        (((attacker->x)==(defender->x-1))&&((attacker->y)==(defender->y)))  ||
        (((attacker->x)==(defender->x))  &&((attacker->y)==(defender->y+1)))||
        (((attacker->x)==(defender->x))  &&((attacker->y)==(defender->y-1))))
        return *attacker;
    //save the weights of proximity of the defender, not in limits
    float weightUp=WEIGHT_MAX,weightDown=WEIGHT_MAX,weightLeft=WEIGHT_MAX,weightRight=WEIGHT_MAX;
    if(defender->y>1)
        weightUp = mapa->tiles[defender->y-2][defender->x-1]->weight+WEIGHT_MAX*(mapa->tiles[defender->y-2][defender->x-1]->hero!=NULL);
    if(defender->y<ROWS_TILE)
        weightDown = mapa->tiles[defender->y][defender->x-1]->weight+WEIGHT_MAX*(mapa->tiles[defender->y][defender->x-1]->hero!=NULL);
    if(defender->x>1)
        weightLeft = mapa->tiles[defender->y-1][defender->x-2]->weight+WEIGHT_MAX*(mapa->tiles[defender->y-1][defender->x-2]->hero!=NULL);
    if(defender->x<COLUMNS_TILE)
        weightRight = mapa->tiles[defender->y-1][defender->x]->weight+WEIGHT_MAX*(mapa->tiles[defender->y-1][defender->x]->hero!=NULL);
    //if the attacker do not have access the defender
    if((weightUp==WEIGHT_MAX)&&(weightDown==WEIGHT_MAX)&&(weightLeft==WEIGHT_MAX)&&(weightRight==WEIGHT_MAX))
        return saida;//(0,0) is NULL
    //if weightUp is the smaller and do not have hero in this tile
    if((weightUp<=weightDown)&&(weightUp<=weightLeft)&&(weightUp<=weightRight)&&(weightUp<WEIGHT_MAX)){
        //return the position
        saida.x=defender->x;
        saida.y=defender->y-1;
        return saida;
    }
    //if weightDown is the smaller
    else if((weightDown<=weightUp)&&(weightDown<=weightLeft)&&(weightDown<=weightRight)&&(weightDown<WEIGHT_MAX)){
        //return the position
        saida.x=defender->x;
        saida.y=defender->y+1;
        return saida;
    }
    //if weightLeft is the smaller
    else if((weightLeft<=weightUp)&&(weightLeft<=weightDown)&&(weightLeft<=weightRight)&&(weightLeft<WEIGHT_MAX)){
        //return the position
        saida.x=defender->x-1;
        saida.y=defender->y;
        return saida;
    }
    //if weightRight is the smaller
    else if((weightRight<=weightUp)&&(weightRight<=weightDown)&&(weightRight<=weightLeft)&&(weightRight<WEIGHT_MAX)){
        //return the position
        saida.x=defender->x+1;
        saida.y=defender->y;
        return saida;
    }
    //do not have tile to attack
    else
        return saida;
}

//attack of heroes, return true if is possible the retaliates
bool Game::attack(Tile* attacker, Tile* defender){
    ALLEGRO_EVENT event;
    int contTime=0, i=0, damage=0;
    //set the image and position start of animation
    attackDraw = animate->animation(attacker->hero->get_class(),attacker->pixel,defender->pixel,i,attackDrawPixel);
    //damage in random
    if(defender->hero->get_evasion()<=(rand()%100)){
        damage = attacker->hero->get_atk();
        sounds->play(attacker->hero->get_class(),"attack");
    }
    else
        sounds->play(attacker->hero->get_class(),"attack miss");
    while(i<IMGS_ANIMATE){
        //wait a event
        event = wait_event();
        if(event.type == ALLEGRO_EVENT_TIMER){
            contTime++;
            //wait the time of animate
            if(contTime >= ATTACK_TIME){
                //next image to animate
                i++;
                contTime=0;
                if(i<IMGS_ANIMATE)
                    attackDraw = animate->animation(attacker->hero->get_class(),attacker->pixel,defender->pixel,i,attackDrawPixel);
                //draw the damage
                if(damage&&((IMGS_ANIMATE-i)<=DAMAGE_FRAMES_DRAW))
                    //draw damage
                    defender->hero->set_damage(damage);
            }
            draw_update();
        }
        //mouse move
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES){
            move_mouse(event.mouse.x,event.mouse.y);
        }
    }
    //no draw damage
    defender->hero->set_damage(-1);
    attackDraw = NULL;
    //speed clear
    attacker->hero->clear_speed();
    //apply damage and check if defender is dead
    if(defender->hero->damage(damage)){
        //delete the defender
        delete_hero();
        defender->hero = NULL;
    }
    //if both have attacker of long distance or short distance
    else if(type_attack(defender->hero->get_class())==type_attack(attacker->hero->get_class()))
        return true;
    //no retaliation
    return false;
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
    sounds->play("walk dirt");//========================================VER MELHOR
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
    nextTile->moveHero->walk(1);
    //change the side of hero
    if(actualTile->pixel->x < nextTile->pixel->x)
        nextTile->moveHero->set_side(RIGHT);
    else if(nextTile->pixel->x < actualTile->pixel->x)
        nextTile->moveHero->set_side(LEFT);
}

//delete hero dead of heroes, return 0 if do not have team with all heroes is dead
int Game::delete_hero(){
    int i=0, teams[2];
    bool towers[2]={false,false};
    teams[0]=0;
    teams[1]=0;
    //find which hero is dead
    for(i=0;i<Hero::get_num_of_heroes();i++)
        //hero is dead
        if(heroes[i]->get_hp()<=0){
            delete heroes[i];
            break;
        }
    //reallocating vector
    for(;i<Hero::get_num_of_heroes();i++)
        heroes[i]=heroes[i+1];
    //cont how many heroes exist in each team and check tower is full
    for(i=0;i<Hero::get_num_of_heroes();i++){
        teams[heroes[i]->get_team()-1]++;
        if(heroes[i]->get_class()==TOWER)
            towers[heroes[i]->get_team()-1]=true;
    }
    //all heroes in team 1 is dead
    if((teams[0]==0) || (towers[0]==false))
        return 1;
    //all heroes in team 2 is dead
    else if((teams[1]==0) || (towers[1]==false))
        return 2;
    else
        return 0;
}
//check if game is finish
bool Game::finish(){
    int teamLose = delete_hero();
    //game is not finish
    if(teamLose==0)
        return false;
    //draw update screen
    menu->set_hero(NULL);
    draw_update();
    //draw victory image
    al_draw_bitmap(animate->get_image("victory")->get_bitmap(),400,150,0);
    //flip the screen
    update_screen();
    //sound of victory
    sounds->play("victory");
    //sleep 5sec
    Sleep(5000);
    return true;
}
//initialize the all heroes
void Game::init_heroes(){
    int x,y;
    //TEAM 1
    //soldiers team 1
    x=2;y=12;
    heroes[0] = new Hero(animate->get_image(SOLDIER,ONE),x,y,40,10,30,6,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[0];
    x=5;y=15;
    heroes[1] = new Hero(animate->get_image(SOLDIER,ONE),x,y,40,10,30,6,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[1];
    x=8;y=18;
    heroes[2] = new Hero(animate->get_image(SOLDIER,ONE),x,y,40,10,30,6,RIGHT,ONE,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[2];
    //mages team 1
    x=3;y=17;
    heroes[3] = new Hero(animate->get_image(MAGE,ONE),x,y,35,18,20,5,RIGHT,ONE,MAGE,3);
    mapa->tiles[y-1][x-1]->hero = heroes[3];
    //archers team 1
    x=3;y=14;
    heroes[4] = new Hero(animate->get_image(ARCHER,ONE),x,y,30,7,40,7,RIGHT,ONE,ARCHER,2);
    mapa->tiles[y-1][x-1]->hero = heroes[4];
    x=6;y=17;
    heroes[5] = new Hero(animate->get_image(ARCHER,ONE),x,y,30,7,40,7,RIGHT,ONE,ARCHER,2);
    mapa->tiles[y-1][x-1]->hero = heroes[5];
    //tower team 1
    x=2;y=18;
    heroes[6] = new Hero(animate->get_image(TOWER,ONE),x,y,100,0,0,0,RIGHT,ONE,TOWER,2);
    mapa->tiles[y-1][x-1]->hero = heroes[6];
    //TEAM 2
    //soldiers team 2
    x=33;y=2;
    heroes[7] = new Hero(animate->get_image(SOLDIER,TWO),x,y,40,10,25,6,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[7];
    x=36;y=5;
    heroes[8] = new Hero(animate->get_image(SOLDIER,TWO),x,y,40,10,30,6,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[8];
    x=39;y=8;
    heroes[9] = new Hero(animate->get_image(SOLDIER,TWO),x,y,40,10,30,6,LEFT,TWO,SOLDIER);
    mapa->tiles[y-1][x-1]->hero = heroes[9];
    //mages team 2
    x=38;y=3;
    heroes[10] = new Hero(animate->get_image(MAGE,TWO),x,y,35,18,25,5,LEFT,TWO,MAGE,3);
    mapa->tiles[y-1][x-1]->hero = heroes[10];
    //archers team 2
    x=35;y=3;
    heroes[11] = new Hero(animate->get_image(ARCHER,TWO),x,y,30,7,25,7,LEFT,TWO,ARCHER,2);
    mapa->tiles[y-1][x-1]->hero = heroes[11];
    x=38;y=6;
    heroes[12] = new Hero(animate->get_image(ARCHER,TWO),x,y,30,7,30,7,LEFT,TWO,ARCHER,2);
    mapa->tiles[y-1][x-1]->hero = heroes[12];
    //tower team 2
    x=39;y=2;
    heroes[13] = new Hero(animate->get_image(TOWER,TWO),x,y,100,0,0,0,LEFT,TWO,TOWER,2);
    mapa->tiles[y-1][x-1]->hero = heroes[13];//*/
}
