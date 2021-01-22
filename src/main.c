#include "init.h"

int main(int argc, char *argv[]){

    initSDL();

    initOpenGL();

    
    createTriangle();
    compileShaders();
    

        drawTriangle();

    // Basic loop
    while(1){
       
        processKeyboard();
        
        clearWindow();

    }

    quitSDL();
    

}
