

const Pixel_Point buttonPixel(1100,15);
#define BUTTON_SIZE_X 100
#define BUTTON_SIZE_Y 50

class Menu{
    //hero is selected, if not select is NULL
    Hero *hero;
    bool buttonPress;
public:
    int turn;
    Menu(){
        turn = 0;
        hero = NULL;
        buttonPress = false;
    }
    ~Menu(){
    }
    void set_hero(Hero *_hero){
        hero = _hero;
    }
    Hero *get_hero(){
        return hero;
    }
    void depress(){
        buttonPress = false;
    }
    //draw all configuration of menu
    void draw_menu(Font* font, Animate* animate){
        char text[MAX_TEXT];
        //if hero is selected
        if(hero != NULL){
            //draw image of menu
            al_draw_bitmap(animate->get_image("menu")->get_bitmap(),0,0,0);
            //print the rectangle of hp
            int pixels_hp = mapping(hero->get_hp(),0,hero->get_max_hp(),0,170);
            al_draw_filled_rectangle(90,30,90 + pixels_hp,60,RED);
            al_draw_rectangle(90,30,91 + 170,61,GREY,1);
            //print the text of hp
            sprintf(text,"HP %d\\%d",hero->get_hp(),hero->get_max_hp());
            al_draw_text(font->get_font(), WHITE, 270, 25,ALLEGRO_ALIGN_LEFT, text);
            //print the atk
            sprintf(text,"ATK  %d",hero->get_atk());
            al_draw_text(font->get_font(), WHITE, 420, 25,ALLEGRO_ALIGN_LEFT, text);
            //print the evasion
            sprintf(text,"EVASION  %d%%",hero->get_evasion());
            al_draw_text(font->get_font(), WHITE, 550, 25,ALLEGRO_ALIGN_LEFT, text);
            //print the speed
            sprintf(text,"SPEED  %.1f",hero->get_speed());
            al_draw_text(font->get_font(), WHITE, 750, 25,ALLEGRO_ALIGN_LEFT, text);
            //draw big hero
            al_draw_scaled_bitmap(hero->get_bitmap(animate),0,0,SIZE_TILE,SIZE_TILE,10,10,SIZE_TILE*2,SIZE_TILE*2,0);
        }
        else
            //draw image clear of menu
            al_draw_bitmap(animate->get_image("menu clear")->get_bitmap(),0,0,0);
        //print number of turn
        sprintf(text,"%d",turn);
        al_draw_text(font->get_font(), WHITE, 1250, 25,ALLEGRO_ALIGN_CENTER, text);
        //draw the button
        if(buttonPress)
            al_draw_bitmap(animate->get_image("button press")->get_bitmap(),buttonPixel.x,buttonPixel.y,0);
        else
            al_draw_bitmap(animate->get_image("button")->get_bitmap(),buttonPixel.x,buttonPixel.y,0);
    }
    //return 1 if next turn is press
    bool click(Pixel_Point pixel){
        //limits of button
        if((buttonPixel.x<pixel.x)&&(pixel.x<=(buttonPixel.x+BUTTON_SIZE_X))&&(buttonPixel.y<=pixel.y)&&(pixel.y<(buttonPixel.y+BUTTON_SIZE_Y))){
           buttonPress = true;
           return true;
        }
        else {
            buttonPress = false;
            return false;
        }
    }
};
