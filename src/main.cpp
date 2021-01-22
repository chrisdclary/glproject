#include "init.h"

int main(){

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
