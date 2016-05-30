#include <stdio.h>
#include <iostream>

#include "screen.h"
#include "mapa.h"
#include "hero.h"
#include "menu.h"
#include "game.h"


using namespace std;

int main(int argc, char **argv){
    //initialize allegro and the all game
    Game *game = new Game;
    if(game->initialize())//Error
        return -1;
    //events
    ALLEGRO_EVENT event;
    while(true){
        event = game->wait_event();
        //Event of time, defined by FPS
        if(event.type == ALLEGRO_EVENT_TIMER)
            game->draw_update();
        //mouse move
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES)
            game->move_mouse(event.mouse.x,event.mouse.y);
        //mouse click
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            //copy mouse position
            Pixel_Point pixel = game->get_mouse_pixel();
            //in map, not menu options
            if(pixel.y>BAR_OPTIONS){
                //find the tile click
                Point point = find_rec(&pixel);
                game->tile_click(point);
            }
            //in menu, not map
            else{
                //if next turn is press
                if(game->menu_click(pixel)){

                }
            }
        }
        //mouse is depress
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            game->depress();
        //if press in X (close)
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
    }
    delete game;
    return 0;
}
