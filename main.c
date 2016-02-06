#include <allegro.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#define l   	97
#define r  	    100
#define u     	119
#define d   	115
#define esc     27
#define enter   13

int portal1[2];
int portal2[2];
int guardi;
int guardj;
int mode;
char dir; // the commands
char save; // the saved commands
int length; // length of snake 
int t; // delay time
int score; // score 
int sp; // speed
double record;
int scenery_number;
char look;

void body( int x, int y)
{
     BITMAP *b = load_bitmap("Image/Body.bmp",NULL);
     blit(b,screen,0,0,x-5,y-5,10,10);
}

void head( int x, int y)
{
     char direc[] = "Image/Headx.bmp";
     direc[10] = look;
     BITMAP *h = load_bitmap(direc,NULL);
     blit(h,screen,0,0,x-5,y-5,10,10);
}

void body_erase( int x, int y)
{
     BITMAP *b = load_bitmap("Image/Eraseblank.bmp",NULL);
     blit(b,screen,0,0,x-5,y-5,10,10);
}


void fruit( int x, int y)
{
     BITMAP *food = load_bitmap("Image/Food.bmp",NULL);
     blit(food,screen,0,0,x-5,y-5,10,10);
}

void block( int x, int y, BITMAP *bm) // display a little block on the screen
{
     BITMAP *b = load_bitmap("Image/Block.bmp",NULL);// load the block
     blit(b,bm,0,0,x-5,y-5,10,10);
}

void miniblock( int x, int y)
{
     BITMAP *mb = load_bitmap("Image/Miniblock.bmp",NULL);// load the miniblock
     blit(mb,screen,0,0,x-2,y-2,5,5);
}

void erase( int matrix[][50], int v[][2])
{
     int k; // counter to the vector
     for(k=0;k<length;k++){matrix[v[k][0]][v[k][1]]=0;} // erasing the vector on the matrix for every position in the vector
}


void zeroing( int matrix[][50]) // zeroing the matriz
{
     int i,j; // counters for line and column
     for(i=0;i<25;i++){for(j=0;j<50;j++){matrix[i][j]=0;}}
}

int charint( char c)
{
     if(c=='0') {return 0;}
     else if(c=='1') {return 1;}
     else if(c=='2') {return 2;}
     else if(c=='3') {return 3;}
     else if(c=='4') {return 4;}
     else if(c=='5') {return 5;}
     else if(c=='6') {return 6;}
     else if(c=='7') {return 7;}
     else if(c=='8') {return 8;}
     else if(c=='9') {return 9;}

}    

char intchar(int number) // convert an integer in a character
{
     if(number==0) {return '0';}
     else if(number==1) {return '1';}
     else if(number==2) {return '2';}
     else if(number==3) {return '3';}
     else if(number==4) {return '4';}
     else if(number==5) {return '5';}
     else if(number==6) {return '6';}
     else if(number==7) {return '7';}
     else if(number==8) {return '8';}
     else if(number==9) {return '9';}
     else return ' ';
}

void display_portal( int x, int y, int n, BITMAP *bm)
{
     char filename[] = "Image/PortalX.bmp";
     filename[12] = intchar(n);
     BITMAP *port = load_bitmap(filename,NULL);
     blit(port,bm,0,0,x-5,y-5,10,10);
}

void outnumber( int x, int y, int number) // prints a number by coordinates
{
     int k;
     char str_number[7];
     str_number[5] = str_number[6] = ' ';  
     for(k=0;k<5;k++) {str_number[4-k] = intchar(((int)(number/pow(10,k)))%10);}
     textout_ex(screen,font,str_number,x,y,0x040428,0xFFFFFF);
}


void display2() // display the dynamic numbers 
{
     outnumber(523,71,score); // display the score 
     outnumber(523,118,length); // display the length of snake
     outnumber(523,163,sp); // display the speed
     outnumber(523,208,record);
}

void display( int matrix[][50], int v[][2], int f[], int initial) // display dynamic things, using graphics (snake, food)
{
     int x,y,i,j; // counters
     x = 15; y = 15; // initial position to the matrix
     if(initial==1){for(i=0;i<length;i++){body(15+10*v[i][1],15+10*v[i][0]);}}
     else {
         body(15+10*v[1][1],15+10*v[1][0]);
         if((v[0][1]+v[0][0])%2==0) {body(15+10*v[0][1],15+10*v[0][0]);}
         else {head(15+10*v[0][1],15+10*v[0][0]);}
         body_erase(15+10*guardj,15+10*guardi);
         fruit(15+10*f[1],15+10*f[0]);// now display the food
     }

}

int init( int matrix[][50]) // this function will initiate the scenery and info
{
     int i,j; // generic counters
     BITMAP *background = load_bitmap("Image/Init.bmp",NULL);
     BITMAP *background2 = load_bitmap("Image/Init2.bmp",NULL);
     BITMAP *buffer2 = create_bitmap(600,272);
     // design
     clear_to_color(screen,0xFFFFFF);
     blit(background,buffer2,0,0,520,0,80,272);
     blit(background2,buffer2,0,0,0,0,520,272);
     rect(buffer2,521,3,596,46,0x000000); // score boundary
     rect(buffer2,521,48,596,91,0x000000); // status boundary
     rect(buffer2,521,95,596,138,0x000000); // length boundary
     rect(buffer2,521,142,596,185,0x000000); // speed boundary
     rect(buffer2,521,187,596,228,0x000000); // record boundary
     textout_ex(buffer2,font,"Score: ",523,51,0xFFFFFF,0x00000);
     textout_ex(buffer2,font,"Mode: ",523,5,0xFFFFFF,0x000000);
     textout_ex(buffer2,font,"Length: ",523,97,0xFFFFFF,0x000000);
     textout_ex(buffer2,font,"Speed: ",523,144,0xFFFFFF,0x000000);
     textout_ex(buffer2,font,"Record: ",523,189,0xFFFFFF,0x000000);
     
     if(mode==1){textout_ex(buffer2,font,"Wall ON",523,24,0x040428,0xFFFFFF);}
     else if(mode==-1) {textout_ex(buffer2,font,"Wall OFF",523,24,0x040428,0xFFFFFF);}
     // now, the static scenery
     for(i=0;i<25;i++)
     {
          for(j=0;j<50;j++)
          {
                 if(matrix[i][j]==8){block((15+10*j),(15+10*i),buffer2);}
                 else if(matrix[i][j]==1) {display_portal((15+10*j),(15+10*i),1,buffer2);}
                 else if(matrix[i][j]==2) {display_portal((15+10*j),(15+10*i),2,buffer2);}
          }
     }
     blit(buffer2,screen,0,0,0,0,600,272);
     
}

void opening()
{
     BITMAP *snake = load_bitmap("Image/Snake.bmp",NULL);
     if(snake!=NULL) blit(snake,screen,0,0,65,0,4740,243);
     rest(1500);
}

void build_scenery( int matrix[][50], char filename[]) // this function will open the scenery.txt file and convert into a matrix
{
     record = 0;
     int i,j; // generic counters
     char rec[5]; // the vector with the characters of record
     FILE *scenery;
     scenery = fopen(filename,"r");
     if(scenery==NULL) {zeroing(matrix);}
     else 
     {
          for(i=0;i<25;i++)
          {
                for(j=0;j<=50;j++)
                {
                       if(j==50) {rec[0] = getc(scenery);}
                       else 
                       {
                            matrix[i][j] = charint(getc(scenery));
                            if(matrix[i][j]==1){portal1[0] = i; portal1[1] = j;}
                            else if(matrix[i][j]==2){portal2[0] = i; portal2[1] = j;}
                       }
                }
          }
     }
     for(i=0;i<5;i++) {rec[i] = getc(scenery);} // at this point, the function knows all characters of record 
     for(i=0;i<5;i++) {record += (charint((int)rec[i]))*pow(10,4-i);} // now, the function will convert the string into a  integer
     fclose(scenery);
}

void saverec( int matrix[][50], int v[][2], int f[]) // this function will write the new record on the file 
{
     int i,j,k=0; // counters
     char filename[] = "sceneryx.txt";
     if(scenery_number==-1) {filename[7] = 'x';}
     else if(scenery_number==0) {filename[7] = '_';}
     else {filename[7] = intchar(scenery_number);}
     // before, the function will erase the snake on the matrix and the food
     erase(matrix,v); matrix[f[0]][f[1]] = 0;
     FILE *scenery; // the pointer of file
     scenery = fopen(filename,"w"); // opening the file to write
     
     /* Writing the matrix */
     for(i=0;i<25;i++) // passing through the lines
     {
          for(j=0;j<50;j++){  putc(intchar(matrix[i][j]),scenery); } // passing through the column
          putc('\n',scenery); // at the final of the line, put a newline
     }
     /*Writing the record*/
     if(score > record) 
     {   
          record = score;
          for(i=4;i>=0;i--) {putc(intchar(((int)(record/pow(10,i)))%10),scenery);}
          textout_ex(screen,font,"NEW RECORD!",220,130,0xFFFFFF,0x000000);
     }
     else {for(i=4;i>=0;i--) {putc(intchar(((int)(record/pow(10,i)))%10),scenery);} textout_ex(screen,font,"GAME OVER!",220,130,0xFFFFFF,0x000000);}
     fclose(scenery);
     
}

void position( int matrix[][50], int v[][2] ) // position a 2d vector (of positions) in a matrix
{
     int k; // counter for vector v
     for(k=0;k<length;k++){matrix[v[k][0]][v[k][1]]=7;}
}

void food(int matrix[][50],int f[]) // places the food 
{
     display2();
     srand(time(NULL));
     f[0] = rand()%23 + 1;
     f[1] = rand()%48 + 1;
     while(matrix[f[0]][f[1]]!=0)
     {
            srand(time(NULL));
            f[0] = rand()%24;
            f[1] = rand()%49;
     }
     matrix[f[0]][f[1]]=6;
     fruit(15+10*f[1],15+10*f[0]);
     if(t>44) {t -= 1;}
     else if(t<44) {t += 1;}
     
}

void move( int matrix[][50], int v[][2], int f[]) // move the vector v in a direction 
// OBS: building
// portal1 = 1
// portal2 = 2
{
     int k; // counter to the vector v
     int save[2]; 
     save[0] = v[length-1][0]; save[1] = v[length-1][1]; // saving the position of last element of snake
     for(k=(length-1);k>=1;k-=1){v[k][0] = v [k-1][0]; v[k][1] = v[k-1][1];} // positions will "delay"
     
     if (dir==u) look = 'u';
     else if (dir==d) look = 'd';
     else if (dir==l) look = 'l';
     else if (dir==r) look = 'r';
     // first, check of portal1
     if((dir==u)&&(v[0][0]-1==portal1[0])&&(v[0][1]==portal1[1])){v[0][0] = portal2[0]-1; v[0][1] = portal2[1];if(t>20) {t -= 1;sp++;}}
     else if((dir==d)&&(v[0][0]+1==portal1[0])&&(v[0][1]==portal1[1])){v[0][0] = portal2[0]+1; v[0][1] = portal2[1];if(t>20) {t -= 1;sp++;}}
     else if((dir==l)&&(v[0][1]-1==portal1[1])&&(v[0][0]==portal1[0])){v[0][0] = portal2[0]; v[0][1] = portal2[1]-1;if(t>20) {t -= 1;sp++;}}
     else if((dir==r)&&(v[0][1]+1==portal1[1])&&(v[0][0]==portal1[0])){v[0][0] = portal2[0]; v[0][1] = portal2[1]+1;if(t>20) {t -= 1;sp++;}}
     
     // now, check of portal2
     else if((dir==u)&&(v[0][0]-1==portal2[0])&&(v[0][1]==portal2[1])){v[0][0] = portal1[0]-1; v[0][1] = portal1[1];if(t>20) {t -= 1;}}
     else if((dir==d)&&(v[0][0]+1==portal2[0])&&(v[0][1]==portal2[1])){v[0][0] = portal1[0]+1; v[0][1] = portal1[1];if(t>20) {t -= 1;}}
     else if((dir==l)&&(v[0][1]-1==portal2[1])&&(v[0][0]==portal2[0])){v[0][0] = portal1[0]; v[0][1] = portal1[1]-1;if(t>20) {t -= 1;}}
     else if((dir==r)&&(v[0][1]+1==portal2[1])&&(v[0][0]==portal2[0])){v[0][0] = portal1[0]; v[0][1] = portal1[1]+1;if(t>20) {t -= 1;}}
     
     else if ((dir==u)&&(v[0][0]-1 == v[1][0])) {v[0][0] +=1;}
     else if((dir==u)&&(v[0][0]==0)){v[0][0]=24;}
     else if(dir==u){v[0][0]-=1;}
     
     else if ((dir==d)&&(v[0][0]+1 == v[1][0])) {v[0][0] -=1;}
     else if((dir==d)&&(v[0][0]==24)){v[0][0]=0;}
     else if(dir==d){v[0][0]+=1;}
     
     else if ((dir==r)&&(v[0][1]+1 == v[1][1])) {v[0][1] -=1;}
     else if((dir==r)&&(v[0][1]==49)){v[0][1]=0;}
     else if(dir==r){v[0][1]+=1;}
     
     else if ((dir==d)&&(v[0][0]-1 == v[1][0])) {v[0][0] +=1;}
     else if((dir==l)&&(v[0][1]==0)){v[0][1]=49;}
     else if(dir==l){v[0][1]-=1;}
     
     if((v[0][0]==f[0])&&(v[0][1]==f[1])) // if the snake eat the food
     { 
          score = score + 10;
          length = length + 1; 
          sp = sp + 1; 
          v[length-1][0] = save[0]; 
          v[length-1][1] = save[1];
          food(matrix,f);
     }
     
}

int analysis( int matrix[][50], int v[][2], int f[]) // if wall mode is on mode = 1 // returns 1 if game over is true
{
     /*((dir==u)&&(v[0][0]-1==v[length-1][0])) {erase(matrix,v); move(matrix,v,dir,f); position(matrix,v); return 0;}       
     else if((dir==d)&&(v[0][0]+1==v[length-1][0])) {erase(matrix,v); move(matrix,v,dir,f); position(matrix,v); return 0;}    
     else if((dir==r)&&(v[0][1]+1==v[length-1][1])) {erase(matrix,v); move(matrix,v,dir,f); position(matrix,v); return 0;}        
     else if((dir==l)&&(v[0][1]-1==v[length-1][1])) {erase(matrix,v); move(matrix,v,dir,f); position(matrix,v); return 0;}*/
     
     if((dir==u)&&(save==d)){dir = d;}
     else if((dir==d)&&(save==u)){dir = u;}
     else if((dir==r)&&(save==l)){dir = l;}
     else if((dir==l)&&(save==r)){dir = r;}
     
     // first, check of portal1
     if((dir==u)&&(v[0][0]-1==portal1[0])&&(v[0][1]==portal1[1])&&(matrix[portal2[0]-1][portal2[1]]==8)){return 1;}
     else if((dir==d)&&(v[0][0]+1==portal1[0])&&(v[0][1]==portal1[1])&&(matrix[portal2[0]+1][portal2[1]]==8)){return 1;}
     else if((dir==l)&&(v[0][1]-1==portal1[1])&&(v[0][0]==portal1[0])&&(matrix[portal2[0]][portal2[1]-1]==8)){return 1;}
     else if((dir==r)&&(v[0][1]+1==portal1[1])&&(v[0][0]==portal1[0])&&(matrix[portal2[0]][portal2[1]+1]==8)){return 1;}
     
     // now, check of portal2
     else if((dir==u)&&(v[0][0]-1==portal2[0])&&(v[0][1]==portal2[1])&&(matrix[portal1[0]-1][portal1[1]]==8)){return 1;}
     else if((dir==d)&&(v[0][0]+1==portal2[0])&&(v[0][1]==portal2[1])&&(matrix[portal1[0]+1][portal1[1]]==8)){return 1;}
     else if((dir==l)&&(v[0][1]-1==portal2[1])&&(v[0][0]==portal2[0])&&(matrix[portal1[0]][portal1[1]-1]==8)){return 1;}
     else if((dir==r)&&(v[0][1]+1==portal2[1])&&(v[0][0]==portal2[0])&&(matrix[portal1[0]][portal1[1]+1]==8)){return 1;}
     
     else if((mode==1)&&(dir==u)&&(v[0][0]==0)) { return 1;} // up boundary
     else if((mode==1)&&(dir==d)&&(v[0][0]==24)) { return 1;} // down boundary
     else if((mode==1)&&(dir==l)&&(v[0][1]==0)) { return 1;} // left boundary
     else if((mode==1)&&(dir==r)&&(v[0][1]==49)) { return 1;} // right boundary
     
     else if((mode==-1)&&(dir==u)&&(v[0][0]==0)&&(matrix[24][v[0][1]]==0)) {erase(matrix,v); move(matrix,v,f); position(matrix,v); return 0;} // upper tail
     else if((mode==-1)&&(dir==d)&&(v[0][0]==24)&&(matrix[0][v[0][1]]==0)) {erase(matrix,v); move(matrix,v,f); position(matrix,v); return 0;}
     else if((mode==-1)&&(dir==l)&&(v[0][1]==0)&&(matrix[v[0][0]][49]==0)) {erase(matrix,v); move(matrix,v,f); position(matrix,v); return 0;} // left tail
     else if((mode==-1)&&(dir==r)&&(v[0][1]==49)&&(matrix[v[0][0]][0]==0)) {erase(matrix,v); move(matrix,v,f); position(matrix,v); return 0;} // right scenery
     
     else if((mode==-1)&&(dir==u)&&(v[0][0]==0)&&(matrix[24][v[0][1]]==7)) { return 1;} // upper tail
     else if((mode==-1)&&(dir==u)&&(v[0][0]==0)&&(matrix[24][v[0][1]]==8)) { return 1;} // upper scenery
     
     else if((mode==-1)&&(dir==d)&&(v[0][0]==24)&&(matrix[0][v[0][1]]==7)) { return 1;} // bottom tail
     else if((mode==-1)&&(dir==d)&&(v[0][0]==24)&&(matrix[0][v[0][1]]==8)) { return 1;} // bottom scenery
     
     else if((mode==-1)&&(dir==l)&&(v[0][1]==0)&&(matrix[v[0][0]][49]==7)) { return 1;} // left tail
     else if((mode==-1)&&(dir==l)&&(v[0][1]==0)&&(matrix[v[0][0]][49]==8)) { return 1;} // left scenery
     
     else if((mode==-1)&&(dir==r)&&(v[0][1]==49)&&(matrix[v[0][0]][0]==7)) { return 1;} // right tail
     else if((mode==-1)&&(dir==r)&&(v[0][1]==49)&&(matrix[v[0][0]][0]==8)) { return 1;} // right scenery
     
     else if((dir==u)&&(matrix[v[0][0]-1][v[0][1]]==7)) { return 1;} // upper tail
     else if((dir==u)&&(matrix[v[0][0]-1][v[0][1]]==8)) { return 1;} // upper scenery

     else if((dir==d)&&(matrix[v[0][0]+1][v[0][1]]==7)) { return 1;} // bottom tail
     else if((dir==d)&&(matrix[v[0][0]+1][v[0][1]]==8)) { return 1;} // bottom scenery
     
     else if((dir==l)&&(matrix[v[0][0]][v[0][1]-1]==7)) { return 1;} // left tail
     else if((dir==l)&&(matrix[v[0][0]][v[0][1]-1]==8)) {return 1;} // left scenery
     
     else if((dir==r)&&(matrix[v[0][0]][v[0][1]+1]==7)) { return 1;} // right tail
     else if((dir==r)&&(matrix[v[0][0]][v[0][1]+1]==8)) { return 1;} // right scenery
     
     else { erase(matrix,v); move(matrix,v,f); position(matrix,v); return 0;} 
}

void select_stage( int matrix[][50]) // if wall mode is off
{
     int k = 0, i,j,s=0;
     char button;
     char address[13] = "sceneryx.txt";  // the generic address to some scenery
     BITMAP *buffer2 = create_bitmap(600,272); 
     BITMAP *mb = load_bitmap("Image/Miniblock.bmp",NULL);// load the miniblock
     FILE *scenery; // pointer of file
     while(s != 1)
     {
          clear_to_color(buffer2,0x6F6F6F);   
          rect(buffer2,173,71,425,198,0x000000);// boot the rectangle
          rect(buffer2,172,70,426,199,0x000000);// boot the rectangle
          rectfill(buffer2,174,72,424,197,0xFFFFFF);// boot the rectangle
          address[7] = intchar(k);
          scenery = fopen(address,"r");
          if(scenery==NULL) {zeroing(matrix);}
          else 
          {
               for(i=0;i<25;i++)
               {
                     for(j=0;j<=50;j++)
                     {
                            if(j==50) {getc(scenery);}
                            else matrix[i][j] = charint(getc(scenery));
                     }
               }
               fclose(scenery);
          }  
          for(i=0;i<25;i++)
               {
                     for(j=0;j<50;j++)
                     {
                            if(matrix[i][j]==8) {blit(mb,buffer2,0,0,(177+j*5)-2,(75+i*5)-2,5,5);}
                     }
               }
          blit(buffer2,screen,0,0,0,0,600,272);
          outnumber(240,210,k);
          textout_ex(screen,font,"Field ",185,210,0xFFFFFF,0x6F6F6F); 
          button = readkey(); rest(20);
          if((button==enter)&&(scenery!=NULL)&&(k!=0)) { clear_to_color(screen,0xFFFFFF); build_scenery(matrix,address); s = 1; scenery_number = k;}
          else if((button==enter)&&(scenery==NULL)) {build_scenery(matrix,"scenery_.txt"); s = 1; scenery_number = 0;}
          else if((button==l)||(button==d)){if(k>0){k--;}} 
          else {if(k<9) {k++;}} 
     }
}

int menu_mode( int matrix[][50]) // boot the menu
{
     char button;
     BITMAP *menu = load_bitmap("Image/Menu.bmp",NULL); // circle(250,98,5); circle(250,129,5);
     BITMAP *selector = load_bitmap("Image/Selector.bmp",NULL);
     BITMAP *erase = load_bitmap("Image/Erase.bmp",NULL);
     mode = 1;
     clear_to_color(screen,0x4D4D4D);
     if(menu!=NULL) {blit(menu,screen,0,0,150,50,300,100);}
     while(button!=enter)
     {
         if(mode>0) {blit(selector,screen,0,0,154,85,90,30); blit(erase,screen,0,0,154,115,90,30);}
         else {blit(selector,screen,0,0,154,115,90,30); blit(erase,screen,0,0,154,85,90,30);}
         button = readkey(); 
         if(button!= enter) {mode = mode*(-1);}
         rest(10);
     }    
     if(mode==-1){select_stage(matrix);}
     else {build_scenery(matrix,"sceneryx.txt"); scenery_number = -1;}
     return mode;
     
}


int main() 
{	
    int field[25][50];  int python[1000][2]; int fd[2];
    int k,i,j,n=1,counter=0, over,t2; 
    char directory[] = "Music/x.mid";
   	
    allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 600, 272, 0, 0);
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
	
	BITMAP *buffer = create_bitmap(600,272);
	MIDI *music;
	while(n>0)
	{
    	dir = r; length = 4; score = 0; t = 100; sp = 1; over = 0; look = 'r'; t2 = 1;
        python[0][0] = 15; python[0][1] = 10; python[1][0] = 15; python[1][1] = 9; python[2][0] = 15; python[2][1] = 8; python[3][0] = 15; python[3][1] = 7;
    	
    	srand(time(NULL)); directory[6] = intchar(rand()%9 + 1);
    	
    	music = load_midi(directory);
    	play_midi(music,0);
    	
    	clear_to_color(screen,0x000000);
    	opening();
    	menu_mode(field);
    	init(field);
    	
    	position(field,python);  // position the snake
        food(field,fd);
        guardi = python[length-1][0]; guardj = python[length-1][1];
        display(field,python,fd,t2);
        
         while(over!=1)
         {
               if(t2<10){t2++;}
               guardi = python[length-1][0]; guardj = python[length-1][1];
               save = dir;
               python[length][0] = i; python[length][1] = j;
               rest(10);
               if(keypressed()){dir = readkey();}  
               else if ((dir & 0xff) == 'w') dir =u;
               else if ((dir & 0xff) == 's') dir =d;
               else if ((dir & 0xff) == 'a') dir =l;
               else if ((dir & 0xff) == 'd') dir =r;
               
               if((dir==u)||(dir==d)||(dir==l)||(dir==r)) {over = analysis(field,python,fd);rest(t-10); }
               else if(dir==esc){over=1;n=0;}
               else {dir = save; over = analysis(field,python,fd);rest(t-10);};
               display(field,python,fd,t2);

               
               
         }
         portal1[0] = 50; portal1[1] = 50; portal2[0] = 50; portal2[1] = 50;
         clear_to_color(screen,0xFFFFFF);
         textout_ex(screen,font,"Score: ",225,160,0x02094A,0xF7FF0B); outnumber(265,160,score);
         saverec(field,python,fd);
         destroy_midi(music);
         rest(100);
         if(readkey()==esc){n=0;}
        
     }
	
	destroy_bitmap(buffer);
	return 0;
}
END_OF_MAIN();
