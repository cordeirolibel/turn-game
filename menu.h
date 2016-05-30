
const char BUTTON[2][MAX_TEXT]={{"bin/button1.png"},{"bin/button2.png"}};
const Pixel_Point buttonPixel(1150,20);
#define BUTTON_SIZE_X 69
#define BUTTON_SIZE_Y 37

class Menu:public Image{
    //hero is selected, if not select is NULL
    Hero *hero;
    Image *button[2];
    bool buttonPress;
public:
    Menu(const char* directory):Image(directory){
        hero = NULL;
        buttonPress = false;
        button[0] = new Image(BUTTON[0]);
        button[1] = new Image(BUTTON[1]);
    }
    ~Menu(){
        delete button[0];
        delete button[1];
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
    void draw_menu(Font* font){
        //draw image of menu
        al_draw_bitmap(get_bitmap(),0,0,0);
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
            sprintf(evasion,"EVASION : %d%%",hero->get_evasion());
            al_draw_text(font->get_font(), WHITE, 650, 45,ALLEGRO_ALIGN_LEFT, evasion);
            //draw big hero
            al_draw_scaled_bitmap(hero->get_bitmap(),0,0,SIZE_TILE,SIZE_TILE,10,10,SIZE_TILE*2,SIZE_TILE*2,0);
        }
        //draw the button
        if(buttonPress)
            al_draw_bitmap(button[1]->get_bitmap(),buttonPixel.x,buttonPixel.y,0);
        else
            al_draw_bitmap(button[0]->get_bitmap(),buttonPixel.x,buttonPixel.y,0);
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
