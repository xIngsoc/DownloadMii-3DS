#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <iostream>
#include <sstream>
#include <3ds.h>
#include "utils.h"
#include "application.h"
#include "input.h"
#include "gui.h"
#include "main.h"

using namespace std;

navBar_s navbar;
u8* screenTopLeft = 0;
u8* screenTopRight = 0;
u8* screenBottom = 0;
extern char superStr[];
/* SCENE */
unsigned int scene = 0;
unsigned int maxScene = 2;
char* sceneTitle = "";


/*
TopScreen: w400 h240
BottomScreen: w320 h240
*/

/* Used for the so called "scrolling" */
int VSPX = 0; 
int VSPY = 0;
int VSTX = 0; 
int VSTY = 0;

void initGUI(){
	//ToDo
}

//Todo: add scrolling to the whole app!!!

void renderGUI(){
	Application_s app2 = {1, 4, "Test3DS", "filfat", "Utils", "http://downloadmii.filfatstudios.com/"};
	screenTopLeft = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL); 
	screenTopRight = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
	screenBottom = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL); 
	/* Clear Screen */
	clearScreen(screenBottom, GFX_BOTTOM);
	clearScreen(screenTopLeft, GFX_TOP); 
	clearScreen(screenTopRight, GFX_TOP);

	/* Background */
	background();
	
	/* UI: TOP */
	navbar.Title = "Applications"; //ToDo
	drawTopBar();
	
	/* UI: BOTTOM */
	
	/* DEBUG */
	renderDebug();
	
	/* Screen related UI(Changes based on scene) */
	switch(scene){
		case 0:
			renderOverview();
			drawAppEntry(app2, 1);
			drawAppEntry(app2, 2);
			drawAppEntry(app2, 3);
			drawAppEntry(app2, 4);
			drawAppEntry(app2, 5);
			drawAppEntry(app2, 6);
			drawAppEntry(app2, 7);
			print("VSTY: %d", VSTY);
			print(", VSPY: %d\n", VSPY);
			break;
		case 1:
			
			break;
		case 2:
			
			break;
		default:
			
			break;
	}
	
	/* Buffers */
	gfxFlushBuffers(); 
	gfxSwapBuffers();
}




/* Scenes */

void renderOverview(){ //Renders a nice overview of the top apps for the user

}
int getOnScreenY(int vsy)
{
    return (vsy-VSPY);
}

/* UIs */
void drawAppEntry(Application_s app, int place){
    //Should be removed and recoded by someone smart unlike me as it doesnt work //filfat
    int y = 0;
    VSTY = place == 1 ? APPLICATION_ENTRY_H : VSTY +MARGIN+ APPLICATION_ENTRY_H; //If this is the first app in the list clear the value set the valut to APPLICATION_ENTRY_H else add APPLICATION_ENTRY_H to VSPY
    y = (MARGIN * (place)) + (APPLICATION_ENTRY_H * (place - 1));

    if((getOnScreenY(y)>=240 || getOnScreenY(y)+APPLICATION_ENTRY_H <= 0) || VSPY >= VSTY){
        return; //Outside screen dont draw
    }
    else if(getOnScreenY(y)+APPLICATION_ENTRY_H >= 240)/*The entry is partly offscreen*/
    {
        drawFillRect( 0,getOnScreenY(y), 320,239, 255,y/(float)VSTY*255,255, screenBottom);
    }
    else if(getOnScreenY(y)<0)
    {
        drawLine(0,0,320,0,0,255,0,screenBottom);
        drawFillRect( 0,0, 320,getOnScreenY(y)+APPLICATION_ENTRY_H, 255,y/(float)VSTY*255,255, screenBottom);
        drawLine( 0, getOnScreenY(y) + APPLICATION_ENTRY_H , 320, getOnScreenY(y) + APPLICATION_ENTRY_H, 204,204,204, screenBottom);
    }
    else{
        drawFillRect( 0,getOnScreenY(y), 320,getOnScreenY(y)+APPLICATION_ENTRY_H, 255,y/(float)VSTY*255,255, screenBottom);
        drawLine( 0, getOnScreenY(y) + APPLICATION_ENTRY_H , 320, getOnScreenY(y) + APPLICATION_ENTRY_H, 204,204,204, screenBottom);
    }
    stringstream s;
    s << app.name << " (y: " << y << ", VSTY:" << VSTY << ")";
    app.name = (char*)s.str().c_str();
    /*For christ sakes, use the same coordinate system on all gfx/draw functions*/
    gfxDrawText(GFX_BOTTOM, GFX_LEFT, &fontBlack, app.name,239-getOnScreenY( APPTITLE_MARGIN + y ), 5);
}


void renderDebug(){
	int i = countLines(superStr); 
 	while(i>200/fontDefault.height-3){cutLine(superStr);i--;} 
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontBlack, superStr, (240-fontDefault.height*5)+6, 6); 
	gfxDrawText(GFX_TOP, GFX_RIGHT, &fontBlack, superStr, (240-fontDefault.height*5)+6, 6); 
}
void background(){
	drawFillRect( 0, 0, 320, 240, 227,242,253, screenBottom);
	drawFillRect( 0, 0, 400, 240, 227,242,253, screenTopLeft);
	drawFillRect( 0, 0, 400, 240, 227,242,253, screenTopRight);
}
void drawTopBar(){
	char buffer[100];
	sprintf(buffer, navbar.Title.c_str());
	drawFillRect(0,0,400,NAVBAR_H, 0,126,216, screenTopLeft);
	drawFillRect(0,0,400,NAVBAR_H, 0,126,216, screenTopRight);
	drawFillRect(0,12,400,SECONDARY_NAVBAR_H + 12, 0,148,255, screenTopRight);
	drawFillRect(0,12,400,SECONDARY_NAVBAR_H + 12, 0,148,255, screenTopRight);
	
	drawString(APPLICATION_NAME, (400-strlen(APPLICATION_NAME)*8)/2,2, 255,255,255, screenTopLeft,GFX_TOP);
	drawString(APPLICATION_NAME, (400-strlen(APPLICATION_NAME)*8)/2,2, 255,255,255, screenTopRight,GFX_TOP);
	
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontWhiteHeader, buffer, 237 - (((SECONDARY_NAVBAR_H/2) + fontWhiteHeader.height)), 13); 
	gfxDrawText(GFX_TOP, GFX_RIGHT, &fontWhiteHeader, buffer, 237 - (((SECONDARY_NAVBAR_H/2) + fontWhiteHeader.height)), 13); 
	
	u64 timeInSeconds = osGetTime() / 1000; 
	u64 dayTime = timeInSeconds % SECONDS_IN_DAY; 
	sprintf(buffer, "%llu:%llu:%llu",dayTime / SECONDS_IN_HOUR,(dayTime % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE,dayTime % SECONDS_IN_MINUTE);
	drawString(buffer, 2,2, 255,255,255, screenTopLeft,GFX_TOP);
	drawString(buffer, 2,2, 255,255,255, screenTopRight,GFX_TOP);
}
int countLines(char* str)
{
	if(!str)return 0;
	int cnt; for(cnt=1;*str=='\n'?++cnt:*str;str++);
	return cnt;
}
void cutLine(char* str)
{
	if(!str || !*str)return;
	char* str2=str;	for(;*str2&&*(str2+1)&&*str2!='\n';str2++);	str2++;
	memmove(str,str2,strlen(str2)+1);
}
