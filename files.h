#define MAX_SOUNDS 100
#define PONTO "bin/ponto.wav"
class Sounds{
    Sound *sounds[MAX_SOUNDS];
public:
    Sounds(){
        //zerand
        for(int i=0;i<MAX_SOUNDS;i++)
            sounds[i] = NULL;
        sounds[0] = new Sound(PONTO);
    }

};
