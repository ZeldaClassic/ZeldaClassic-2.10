/*                 __                  __
 *                /_/\  __  __  __    /_/\  ______
 *               _\_\/ / /\/ /\/ /\  _\_\/ / ____ \ 
 *              / /\  / / / / / / / / /\  / /\_ / /\ 
 *         __  / / / / /_/ /_/ / / / / / / / / / / /
 *        / /_/ / / /_________/ / /_/ / /_/ / /_/ /
 *        \____/ /  \_________\/  \_\/  \_\/  \_\/
 *         \___\/
 *
 *
 *
 *     jwin.c
 *
 *     Windows(R) style GUI for Allegro.
 *     by Jeremy Craner
 *
 *     Most routines are adaptations of Allegro code.
 *     Allegro is by Shawn Hargreaves, et al.
 *
 *     Version: 3/22/00
 *     Allegro version: 3.1x  (don't know if it works with WIP)
 *
 */

/* This code is not fully tested */

#define ALLEGRO_NO_COMPATIBILITY
#include <allegro.h>
#include "tab_ctl.h"

int vc2(int x)
{
  switch (x)
  {
    case 0:                                                 //black
      return makecol(0, 0, 0);
      break;
    case 1:                                                 //blue
      return makecol(0, 0, 170);
      break;
    case 2:                                                 //green
      return makecol(0, 170, 0);
      break;
    case 3:                                                 //cyan
      return makecol(0, 170, 170);
      break;
    case 4:                                                 //red
      return makecol(170, 0, 0);
      break;
    case 5:                                                 //magenta
      return makecol(170, 0, 170);
      break;
    case 6:                                                 //brown
      return makecol(170, 85, 170);
      break;
    case 7:                                                 //light grey
      return makecol(170, 170, 170);
      break;
    case 8:                                                 //dark grey
      return makecol(85, 85, 85);
      break;
    case 9:                                                 //light blue
      return makecol(85, 85, 255);
      break;
    case 10:                                                //light green
      return makecol(85, 255, 85);
      break;
    case 11:                                                //light cyan
      return makecol(85, 255, 255);
      break;
    case 12:                                                //light red (pink)
      return makecol(255, 85, 85);
      break;
    case 13:                                                //light magenta (lavender)
      return makecol(255, 85, 255);
      break;
    case 14:                                                //light brown (yellow)
      return makecol(255, 255, 85);
      break;
    case 15:                                                //white
      return makecol(255, 255, 255);
      break;
  }
  return -1;
}


inline int is_in_rect(int x,int y,int rx1,int ry1,int rx2,int ry2)
{
  return x>=rx1 && x<=rx2 && y>=ry1 && y<=ry2;
}

int tabs_width(TABPANEL *panel)
{
  int i=0;
  int w=0;
  for (i=0; panel[i].text; i++)
  {
    w+=text_length(font, (char *)panel[i].text)+15;
  }
  return w+1;
}

int discern_tab(TABPANEL *panel, int x)
{
  int i=0;
  int w=0;
  for (i=0; panel[i].text; i++)
  {
    w+=text_length(font, (char *)panel[i].text)+15;
    if (w>x)
    {
      return i;
    }
  }
  return -1;
}

int d_tab_proc(int msg, DIALOG *d, int c)
{
  int fg;
  int i;
  int tx;
  int sd=2; //selected delta
  TABPANEL *panel=(TABPANEL *)d->dp;
  DIALOG   *panel_dialog=NULL, *current_object=NULL;
  int selected=0;
  int counter=0;
  ASSERT(d);
  switch (msg)
  {
    case MSG_START:
      {
        d->d1=-1;
        //for each tab...
        for (i=0; panel[i].text; i++)
        {
          panel[i].objects=0;
          //see how many controls (i) are handled by this tab
          while(panel[i].dialog[(panel[i].objects)++]!=-1);
          //because the -1 is counted, drop back one
          (panel[i].objects)--;
          //allocate space to store the x and y coordinates for them
          panel[i].xy=(int*)malloc(panel[i].objects*2*sizeof(int));
          //what dialog is this tab control in (programmer must set manually)
          panel_dialog=(DIALOG *)d->dp3;
          //for each object handled by this tab...
          for(counter=0; counter<panel[i].objects; counter++)
          {
            //assign current_object to one of the controls handled by the tab
            current_object=panel_dialog+(panel[i].dialog[counter]);
            //remember the x and y positions of the control
            panel[i].xy[counter*2]=current_object->x;
            panel[i].xy[counter*2+1]=current_object->y;
            //move the control offscreen
            current_object->x=SCREEN_W*3;
            current_object->y=SCREEN_H*3;
          }
        }
        d->d2=1;
      }
      break;
    case MSG_END:
    {
      for (i=0; panel[i].text; i++)
      {
        //what dialog is this tab control in (programmer must set manually)
        panel_dialog=(DIALOG *)d->dp3;
        //for each object handled by this tab...
        for(counter=0; counter<panel[i].objects; counter++)
        {
          //assign current_object to one of the controls handled by the tab
          current_object=panel_dialog+(panel[i].dialog[counter]);
          //put the controls back where they belong
          current_object->x=panel[i].xy[counter*2];
          current_object->y=panel[i].xy[counter*2+1];
        }
      }
	  }
	  break;
    case MSG_IDLE:
    {
      //if we are off-screen
      if (d->x>SCREEN_W||d->y>SCREEN_H)
      {
        if (d->d2==1)
        {
          //for each tab
          for (i=0; panel[i].text; i++)
          {
            //what dialog is this tab control in (programmer must set manually)
            panel_dialog=(DIALOG *)d->dp3;
            //for each object handled by this tab...
            for(counter=0; counter<panel[i].objects; counter++)
            {
              //assign current_object to one of the controls handled by the tab
              current_object=panel_dialog+(panel[i].dialog[counter]);
              //move the control offscreen
              current_object->x=SCREEN_W*3;
              current_object->y=SCREEN_H*3;
            }
          }
          d->d2=0;
        }
      }
      else
      {
        d->d2=1;
      }
    }
    break;
    case MSG_DCLICK:
    case MSG_LPRESS:
    case MSG_LRELEASE:
    case MSG_MPRESS:
    case MSG_MRELEASE:
    case MSG_RPRESS:
    case MSG_RRELEASE:
    case MSG_GOTMOUSE:
    case MSG_LOSTMOUSE:
      break;

  
    case MSG_DRAW:
    {
      //backup the default font
      FONT *oldfont = font;
      if (d->x<SCREEN_W&&d->y<SCREEN_H)
      {
        panel_dialog=(DIALOG *)d->dp3;
        rectfill(screen, d->x, d->y, d->x+d->w-1, d->y+8+text_height(font), d->bg); //tab area
        fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
        rectfill(screen, d->x+1, d->y+sd+text_height(font)+7, d->x+d->w-2, d->y+sd+d->h-2, d->bg); //panel
        //left, right, and bottom borders of the tab control
        vline(screen, d->x, d->y+sd+7+text_height(font), d->y+sd+d->h-1, fg);
        vline(screen, d->x+d->w-1, d->y+sd+7+text_height(font), d->y+sd+d->h-1, fg);
        hline(screen, d->x+1, d->y+sd+d->h-1, d->x+d->w-2, fg);
        //do the tabs have a custom font?
        if (d->dp2)
        {
          font = (FONT *)d->dp2;
        }
        //tx=tab control x position
        tx=d->x;
        //if the programmer gave us a tab panel set to use...
        if (d->dp) {
          //if the first tab is not selected
          if (!(panel[0].flags&D_SELECTED))
          {
            //draw the initial tab panel top border
            hline(screen, tx, d->y+sd+6+text_height(font), tx+1, fg); //initial bottom
          }
          tx+=2;
          //for each tab (that actually has text)
          for (i=0; panel[i].text; i++)
          {
            //if the tab is the selected one...
            if (panel[i].flags&D_SELECTED)
            {
              //remember it
              selected=i;
            }
            //sd is 2 if the panel is not selected, 0 if it is
            sd=(panel[i].flags&D_SELECTED)?0:2;
            //if this is the first tab or if the previous tab is not selected (doesn't cover the left side of this tab)
            if ((i==0) || (!(panel[i-1].flags&D_SELECTED)))
            {
              //draw the left side of the tab
              vline(screen, tx-(2-sd), d->y+sd+2, d->y+8+text_height(font), fg); //left side
              putpixel(screen, tx+1-(2-sd), d->y+sd+1, fg); //left angle
            }
            hline(screen, tx+2-(2-sd), d->y+sd, tx+12+(2-sd)+text_length(font, (char *)panel[i].text), fg); //top
            //if the tab is not selected...
            if (!(panel[i].flags&D_SELECTED))
            {
              //draw the top border of the tab panel under the tab itself
              hline(screen, tx+1, d->y+sd+6+text_height(font), tx+13+text_length(font, (char *)panel[i].text), fg); //bottom
            }
            tx+=4;
            //draw the text of the panel
            gui_textout_ex(screen, (char *)panel[i].text, tx+4, d->y+sd+4, fg, d->bg, FALSE);
            tx+=text_length(font, (char *)panel[i].text)+10;
            //if this is the last tab or the next tab is not selected...
            if (!(panel[i+1].text) || (!(panel[i+1].flags&D_SELECTED)))
            {
              //draw the right side of this tab
              putpixel(screen, tx-1+(2-sd), d->y+sd+1, fg); //right angle
              vline(screen, tx+(2-sd), d->y+sd+2, d->y+8+text_height(font), fg); //right side
            }
            tx++;
          }
        }
        //restore the default font
        font = oldfont;
        //draw the remaining top border of the tab panel control
        hline(screen, tx+(2-sd), d->y+8+text_height(font), d->x+d->w-1, fg); //ending bottom
        
        //what dialog is this tab control in (programmer must set manually)
        panel_dialog=(DIALOG *)d->dp3;
        //for each object handled by the currently selected tab...
        for(counter=0; counter<panel[selected].objects; counter++)
        {
          //assign current_object to one of the controls handled by the tab
          current_object=panel_dialog+(panel[selected].dialog[counter]);
          //remember the x and y positions of the control
          current_object->x=panel[selected].xy[counter*2];
          current_object->y=panel[selected].xy[counter*2+1];
          object_message(current_object, MSG_DRAW, 0);
        }
      }
      //if there was a previously selected tab...
      if (d->d1!=-1)
      {
        //for each object handled by the tab
        for(counter=0; counter<panel[d->d1].objects; counter++)
        {
          //assign current_object to one of the controls handled by the tab
          current_object=panel_dialog+(panel[d->d1].dialog[counter]);
          current_object->x=SCREEN_W*3;
          current_object->y=SCREEN_H*3;
        }
      }
      for (i=0; panel[i].text; i++)
      {
        //if the tab is the selected one...
        if (panel[i].flags&D_SELECTED)
        {
          //remember it
          d->d1=i;
        }
      }
    }
    break;

    case MSG_CLICK:
    {
      d->d1=-1;
      // is the mouse in the tab area?
      if (is_in_rect(gui_mouse_x(),gui_mouse_y(), d->x+2, d->y+2, d->x+tabs_width(panel), d->y+text_height(font)+9))
      {
        // for each tab...
        for (i=0; panel[i].text; i++)
        {
          // see if it's the selected tab and make note of it (in d->d1)
          if (panel[i].flags&D_SELECTED)
          {
            d->d1=i;
          }
        }
        // find out what the new tab (tb) will be (where the mouse is)
        selected=discern_tab(panel, gui_mouse_x()-d->x-2);
        if (selected!=-1&&selected!=d->d1)
        {
          for (i=0; panel[i].text; i++)
          {
            panel[i].flags &= ~D_SELECTED;
          }
          panel[selected].flags |= D_SELECTED;
          object_message(d, MSG_DRAW, 0);
        }
      }
    }
    break;
  }
  return D_O_K;
}

/***  The End  ***/
