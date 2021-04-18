#include "draw.h"

void setupBoot()
{
    drawBootLogo(); //draw ASCII art
    interrupt(0x10, 0x0013, 0, 0, 0); //reset screen
    drawGraphic();
    interrupt(0x10, 0x0003, 0, 0, 0);
}

void drawBootLogo() //Bonus ASCII art
{
    interrupt(0x10, 0x0003, 0, 0, 0); //reset screen
    interrupt(0x10, 0x0100, 0, 0x2607, 0); //invisible cursor
    drawString("Now Loading...");
    drawString(" ");
    drawString("            $$$$$$$$\\        $$\\        $$$$$$\\   $$$$$$\\  ");
    drawString("            \\__$$  __|       $$ |      $$  __$$\\ $$  __$$\\ ");
    drawString("               $$ |$$\\   $$\\ $$$$$$$\\  $$ /  $$ |$$ /  \\__|");
    drawString("               $$ |$$ |  $$ |$$  __$$\\ $$ |  $$ |\\$$$$$$\\  ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ | \\____$$\\ ");
    drawString("               $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$\\   $$ |");
    drawString("               $$ |\\$$$$$$  |$$$$$$$  | $$$$$$  |\\$$$$$$  |");
    drawString("               \\__| \\______/ \\_______/  \\______/  \\______/ ");
    delay(5000, 5000); //delay
    TEXT_HEIGHT = 0; //mengeset text height jadi 0
}

void drawGraphic() //BONUS draw pixel
{
    drawBox(40, 279, 25, 174, 40, COLOR_DARK_GRAY);
    delay(2500, 2500);
    drawBox(80, 239, 50, 149, 80, COLOR_LIGHT_GRAY);
    delay(2500, 2500);
    drawBox(120, 199, 75, 124, 120, COLOR_WHITE);    
    delay(2500, 2500);
    drawBox(130, 155, 80, 97, 130, COLOR_BLACK);
    delay(2000, 2000);
    drawBox(130, 155, 102, 119, 130, COLOR_BLACK);  
    delay(2000, 2000);
    drawBox(164, 189, 102, 119, 164, COLOR_BLACK);     
    delay(2000, 2000);
    drawBox(164, 189, 80, 97, 164, COLOR_BLACK);         
    delay(2500, 2500);                                
}

void drawBox(int x1, int x2, int y1, int y2, int z, int color){
    while(y1<y2){
        while(x1<x2){
            interrupt(0x10, 0x0C00|color, 0, x1, y1);
            x1++;
        }
        x1=z;
        y1++;
    }
}

void delay(int a, int b){
    int i=0;
    int j=0;
    while(i<a){
        j=0;
        while(j<b){
            j++;
        }
        i++;
    }
}