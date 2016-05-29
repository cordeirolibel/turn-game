#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#define GAME_NAME "Turno"

#ifdef __linux__
    #define LINUX true
#elif _WIN32
    #define LINUX false
#endif

#define BLACK al_map_rgb(0,0,0)
#define GREY al_map_rgb(128,128,128)
#define WHITE al_map_rgb(255,255,255)
#define RED al_map_rgb(255,0,0)
#define GREEN al_map_rgb(0,255,0)
#define BLUE al_map_rgb(0,0,255)
#define YELLOW al_map_rgb(255,255,0)
#define ORANGE al_map_rgb(255,128,0)
#define CYAN al_map_rgb(0,255,255)
#define PURPLE al_map_rgb(255,0,255)
#define LIGHTBLUE al_map_rgb(128,128,255)
#define PINK al_map_rgb(255,128,128)
#define LIGHTGREEN al_map_rgb(128,255,128)
#define DARKPURPLE al_map_rgb(128,0,128)

#define FPS  60
#define SCREEN_W  1280
#define SCREEN_H  720

using namespace std;

//side to lock hero
enum Side{
    RIGHT=0,LEFT, UP, DOWN
};

//coordinates in pixels
class Pixel_Point{
public:
    int x, y;
    Pixel_Point(){};
    Pixel_Point(int _x, int _y){
        x=_x;
        y=_y;
    }
};
//coordinates in tiles
class Point{
public:
    int x, y;
    Point(){};
    Point(int _x, int _y){
        x=_x;
        y=_y;
    }
};

//load image, open the directory in constructor
class Image{
    ALLEGRO_BITMAP *bitmap;
public:
    Side side;
    Image(const char*directory){
        side = RIGHT;
        //load bitmap directory
        while(true){
            bitmap = al_load_bitmap(directory);
            if(bitmap)//scape
                break;
            cout << "Failed to open bitmap: " << directory << endl;
            cout << "Press enter to try again" << endl;
            cin.ignore();
        }
    }
    ~Image(){
        //destroy all instance
        al_destroy_bitmap(bitmap);
    }
    ALLEGRO_BITMAP *get_bitmap(){
        return bitmap;
    }
};

//load font, open the directory in constructor
class Font{
    ALLEGRO_FONT *font;
public:
    Font(const char* directory, int _size){
        //load font directory
        while(true){
            font = al_load_ttf_font(directory,_size,0);
            if(font)//scape
                break;
            cout << "Failed to open font: " << directory << endl;
            cout << "Press enter to try again" << endl;
            cin.ignore();
        }
    }
    ~Font(){
        //destroy all instance
        al_destroy_font(font);
    }
    ALLEGRO_FONT *get_font(){
        return font;
    }
};

//load sound, open the directory in constructor
class Sound{
    ALLEGRO_SAMPLE *sound;
public:
    Sound(const char* directory){
        //load sound directory
        while(true){
            sound = al_load_sample(directory);
            if(sound)//scape
                break;
            cout << "Failed to open sound: " << directory << endl;
            cout << "Press enter to try again" << endl;
            cin.ignore();
        }
    }
    ~Sound(){
        al_destroy_sample(sound);
    }
};

//All configuration of screen
class Screen{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *eventQueue;
public:
    Screen(){

    }
    virtual ~Screen(){
        //destroy all instance
        al_destroy_timer(timer);
        al_destroy_display(display);
        al_destroy_event_queue(eventQueue);
    }
    //initialize the initial configuration of allegro
    virtual int initialize();
    //clear the screen
    void clear_screen(){
        al_set_target_bitmap(al_get_backbuffer(display));
        al_clear_to_color(al_map_rgb(0,0,0));
    }
    //buffer to screen
    void update_screen(){
        al_flip_display();
    }
    //wait the event occurs
    ALLEGRO_EVENT wait_event(){
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);
        return event;
    }
};

//initialize the initial configuration of allegro
int Screen::initialize(){
    //Initializer Allegro
    if(!al_init()) {
        cout << "Failed to initialize allegro!\n";
        return -1;
    }
    //Initialize Keyboard
    if(!al_install_keyboard()) {
        cout << "Failed to initialize the keyboard!\n";
        return -1;
    }
    //Initialize Fonts
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon
    //Initialize Images
    if(!al_init_image_addon()) {
        cout << "Failed to initialize the al_init_image_addon!\n";
        return -1;
    }
    if(!LINUX){
        //Initialize Audio
        if(!al_install_audio()){
            cout << "Failed to initialize audio!\n";
            return -1;
        }
        //Initialize Codecs
        if(!al_init_acodec_addon()){
            cout << "Failed to initialize audio codecs!\n";
            return -1;
        }
        //Reserves a number of sample instances, and creates a default mixer if one doesn't exist. This allows us to decide how many audio samples we will be creating for now we are only creating one.
        if (!al_reserve_samples(1)){
            cout << "Failed to reserve samples!\n";
            return -1;
        }
    }
    //Enable mouse
    if (!al_install_mouse())
    {
        cout << "Failed to initialize the mouse!\n";
        return -1;
    }
    //Create the timer
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
        cout << "Failed to create timer!\n";
        return -1;
    }
    //Create the display
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        cout << "Failed to create display!\n";
        return -1;
    }
    //Assigns the default cursor of the system
    if (!al_set_system_mouse_cursor(display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        cout << "Failed to assigns the mouse!\n";
        return -1;
    }
    //Initialize the primitives
    if(!al_init_primitives_addon()){
        cout << "Failed to initialize the primitives!\n";
        return -1;
    }
    //title of the game
    al_set_window_title(display, GAME_NAME);
    //create row of events
    eventQueue = al_create_event_queue();
    if(eventQueue==0) {
        cout << "Failed to create event_queue!\n";
        return -1;
    }
    //hide the cursor of the windows
    al_hide_mouse_cursor(display);
    //Register of events
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    //clear the screen
    clear_screen();
    update_screen();
    //star time
    al_start_timer(timer);
    return 0;
}

class Mouse : public Image {
    Pixel_Point *pixel;
public:
    Mouse(const char* directory):Image(directory){
        //allocate and zero
        pixel = new Pixel_Point;
        pixel->x=0;
        pixel->y=0;
    }
    void moves(int x, int y){
        pixel->x = x;
        pixel->y = y;
    }
    Pixel_Point* get_pixel(){
        return pixel;
    }
    int get_x(){
        return pixel->x;
    }
    int get_y(){
        return pixel->y;
    }
};

