#define MAX_SOUNDS 100
#define IMGS_ANIMATE 10
#define DAMAGE_FRAMES_DRAW 2

//files sound
#define BUTTON_CLICK "bin/sounds/button.wav"
#define FIRE_BOW "bin/sounds/bow-puny-fire.ogg"
#define FIRE_BOW_MISS "bin/sounds/bow-puny-fire-miss.ogg"
#define LIGHTNING "bin/sounds/lightning.ogg"
#define LIGHTNING_MISS "bin/sounds/lightning-miss.ogg"
#define SLASH "bin/sounds/axe.ogg"
#define SLASH_MISS "bin/sounds/miss-3.ogg"
#define DIRT "bin/sounds/footstep-dirt-05.ogg"

//files images
#define ARCHER_BLUE "bin/archer_blue.png"
#define ARCHER_RED "bin/archer_red.png"
#define MAGE_RED "bin/mage_red.png"
#define MAGE_BLUE "bin/mage_blue.png"
#define SOLDIER_BLUE "bin/soldier_blue.png"
#define SOLDIER_RED "bin/soldier_red.png"
const char MAGE_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/lightning1.png"},{"bin/lightning2.png"},{"bin/lightning3.png"},{"bin/lightning4.png"},{"bin/lightning5.png"},{"bin/lightning6.png"},{"bin/lightning5.png"},{"bin/lightning6.png"},{"bin/lightning5.png"},{"bin/lightning6.png"}};
//const char ARCHER_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/arrow1.png"},{"bin/arrow1.png"},{"bin/arrow1.png"},{"bin/arrow2.png"},{"bin/arrow3.png"},{"bin/arrow4.png"},{"bin/arrow5.png"},{"bin/arrow6.png"},{"bin/arrow6.png"},{"bin/arrow6.png"}};
//const char ARCHER_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"},{"bin/bola.png"}};
const char ARCHER_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/fire_arrow1.png"},{"bin/fire_arrow1.png"},{"bin/fire_arrow1.png"},{"bin/fire_arrow2.png"},{"bin/fire_arrow3.png"},{"bin/fire_arrow4.png"},{"bin/fire_arrow5.png"},{"bin/fire_arrow6.png"},{"bin/fire_arrow6.png"},{"bin/fire_arrow6.png"}};
const char SOLDIER_ATTACK[IMGS_ANIMATE][MAX_TEXT]= {{"bin/slash1.png"},{"bin/slash2.png"},{"bin/slash3.png"},{"bin/slash4.png"},{"bin/slash4.png"},{"bin/slash4.png"},{"bin/slash4.png"},{"bin/slash4.png"},{"bin/slash4.png"},{"bin/slash4.png"}};
const char SOLDIER_ATTACK_UP[IMGS_ANIMATE][MAX_TEXT]= {{"bin/slash21.png"},{"bin/slash22.png"},{"bin/slash23.png"},{"bin/slash24.png"},{"bin/slash24.png"},{"bin/slash24.png"},{"bin/slash24.png"},{"bin/slash24.png"},{"bin/slash24.png"},{"bin/slash24.png"}};


class Sounds{
    Sound *sounds[MAX_SOUNDS];
public:
    Sounds(){
        //zeroing the vector
        for(int i=0;i<MAX_SOUNDS;i++)
            sounds[i] = NULL;
        sounds[0] = new Sound(BUTTON_CLICK);
        sounds[1] = new Sound(FIRE_BOW);
        sounds[2] = new Sound(FIRE_BOW_MISS);
        sounds[3] = new Sound(LIGHTNING);
        sounds[4] = new Sound(LIGHTNING_MISS);
        sounds[5] = new Sound(SLASH);
        sounds[6] = new Sound(SLASH_MISS);
        sounds[7] = new Sound(DIRT);
    }
    ~Sounds(){
        for(int i=0;sounds[i]!=NULL;i++)
            delete sounds[i];
    }
    //play the sound defined by "name"
    void play(string name){
        if(!name.compare("button"))
            sounds[0]->play();
        else if(!name.compare("fire bow"))
            sounds[1]->play();
        else if(!name.compare("fire bow miss"))
            sounds[2]->play();
        else if(!name.compare("lightning"))
            sounds[3]->play();
        else if(!name.compare("lightning miss"))
            sounds[4]->play();
        else if(!name.compare("slash"))
            sounds[5]->play();
        else if(!name.compare("slash miss"))
            sounds[6]->play();
        else if(!name.compare("walk dirt"))
            sounds[7]->play();
        else
            cout << "Failed to play \"" << name << "\" sound\n";
    }
    //play specific sound of class defined by name
    void play(Class _class, string name){
        if(!name.compare("attack")){
            if(_class==MAGE)
                play("lightning");
            else if (_class==ARCHER)
                play("fire bow");
            else if (_class==SOLDIER)
                play("slash");
        }
        if(!name.compare("attack miss")){
            if(_class==MAGE)
                play("lightning miss");
            else if (_class==ARCHER)
                play("fire bow miss");
            else if (_class==SOLDIER)
                play("slash miss");
        }
    }
};

class Animate{
    Image *heroSoldierRed;
    Image *heroSoldierBlue;
    Image *heroMageRed;
    Image *heroMageBlue;
    Image *imgMageAttack[IMGS_ANIMATE];
    Image *imgArcherAttack[IMGS_ANIMATE];
    Image *imgSoldierAttack[IMGS_ANIMATE];
    Image *imgSoldierAttackUp[IMGS_ANIMATE];
    Image *heroArcherRed;
    Image *heroArcherBlue;
public:
    Animate(){
        //allocate all images
        heroSoldierBlue = new Image(SOLDIER_BLUE);
        heroSoldierRed = new Image(SOLDIER_RED);
        heroMageBlue = new Image(MAGE_BLUE);
        heroMageRed = new Image(MAGE_RED);
        for(int i=0;i<IMGS_ANIMATE;i++){
            imgMageAttack[i] = new Image(MAGE_ATTACK[i]);
            imgArcherAttack[i] = new Image(ARCHER_ATTACK[i]);
            imgSoldierAttack[i] = new Image(SOLDIER_ATTACK[i]);
            imgSoldierAttackUp[i] = new Image(SOLDIER_ATTACK_UP[i]);
        }
        heroArcherBlue = new Image(ARCHER_BLUE);
        heroArcherRed = new Image(ARCHER_RED);
    }
    ~Animate(){
        //deallocate all images
        delete heroSoldierRed;
        delete heroSoldierBlue;
        delete heroMageRed;
        delete heroMageBlue;
        for(int i=0;i<IMGS_ANIMATE;i++){
            delete imgMageAttack[i];
            delete imgArcherAttack[i];
            delete imgSoldierAttack[i];
            delete imgSoldierAttackUp[i];
        }
        delete heroArcherRed;
        delete heroArcherBlue;
    }
    //return the bitmap of Class and Team defined
    Image *get_image(Class _class, Team team){
        if(_class==MAGE&&team==ONE)
            return heroMageBlue;
        else if(_class==MAGE&&team==TWO)
            return heroMageRed;
        else if(_class==SOLDIER&&team==ONE)
            return heroSoldierBlue;
        else if(_class==SOLDIER&&team==TWO)
            return heroSoldierRed;
        else if(_class==ARCHER&&team==ONE)
            return heroArcherBlue;
        else if(_class==ARCHER&&team==TWO)
            return heroArcherRed;
        else
            return NULL;
    }
    //return image and or position of animation of the class
    Image* animation(Class _class, Pixel_Point* attack,Pixel_Point* defender,int frame, Pixel_Point* imgPosition){
        if(frame>=IMGS_ANIMATE)
            return NULL;
        //if class is a mage
        if(_class==MAGE){
            //set position of image
            imgPosition->x = defender->x;
            imgPosition->y = defender->y-100;
            //return image of animation
            return (imgMageAttack[frame]);
        }
        //if class is a Archer
        else if(_class==ARCHER){
            //set position of image
            int fit = (defender->x-attack->x)*(frame)/(IMGS_ANIMATE-1);
            imgPosition->x = attack->x+fit;
            //set side of arrow
            if(attack->x < defender->x)
                imgArcherAttack[frame]->side = RIGHT;
            else
                imgArcherAttack[frame]->side = LEFT;
            //arrow is up
            if((frame)<(IMGS_ANIMATE/2))
                imgPosition->y = attack->y-20*frame;
            //arrow is down
            else if((frame)>(IMGS_ANIMATE/2)){
                int maxHeight = attack->y-20*(IMGS_ANIMATE/2-1);
                imgPosition->y += (defender->y-maxHeight)/(IMGS_ANIMATE/2-1);
            }
            //return image of animation
            return (imgArcherAttack[frame]);
        }
        //if class is a Soldier
        else if(_class==SOLDIER){
            //return image of animation
            if(frame>=IMGS_ANIMATE)
                return NULL;
            //set position of image
            imgPosition->x = attack->x+frame*(defender->x-attack->x)/IMGS_ANIMATE;
            imgPosition->y = attack->y+frame*(defender->y-attack->y)/IMGS_ANIMATE;
            //set side of slash
            if(attack->x < defender->x){
                imgSoldierAttack[frame]->side = RIGHT;
                return imgSoldierAttack[frame];
            }
            else if(attack->x > defender->x){
                imgSoldierAttack[frame]->side = LEFT;
                return imgSoldierAttack[frame];
            }
            else if(attack->y > defender->y){
                imgSoldierAttackUp[frame]->side = UP;
                return imgSoldierAttackUp[frame];
            }
            else{
                imgSoldierAttackUp[frame]->side = RIGHT;
                return imgSoldierAttackUp[frame];
            }
        }
        return NULL;
    }
};
