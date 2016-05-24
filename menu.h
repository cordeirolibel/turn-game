class Menu:public Image
{

public:

    Hero *hero;

    Menu(const char* directory):Image(directory)
    {
        hero = NULL;
    }

    void set_hero(Hero *_hero)
    {
        hero = _hero;
    }

};
