


class Menu:public Image{
    //hero is selected, if not select is NULL
    Hero *hero;
public:
    Menu(const char* directory):Image(directory){
        hero = NULL;
    }
    void set_hero(Hero *_hero){
        hero = _hero;
    }
    Hero *get_hero(){
        return hero;
    }
};
