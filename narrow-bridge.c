
/* File for 'narrow_bridge' task implementation.  
   SPbSTU, IBKS, 2017 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "narrow-bridge.h"




int cur_dir;
int all_em;
int all_nor;
struct semaphore norL, norR, emR, emL;
int f = 0;

struct all_bridge{
int k_on; // on bridge

int k_norL;//norm left
int k_norR;//norm right
int k_emL;// emer left
int k_emR;//emer right

int all_em,all_nor;

};


struct all_bridge bridge;



void narrow_bridge_init(unsigned int num_vehicles_left,unsigned int num_vehicles_right, unsigned int num_emergency_left,unsigned int num_emergency_right)
{
bridge.k_norL=num_vehicles_left;
bridge.k_norR=num_vehicles_right;
bridge.k_emL=num_emergency_left;
bridge.k_emR=num_emergency_right;
cur_dir=0;
all_em=num_emergency_left+num_emergency_right;
//printf("all_em - %d",all_em);
all_nor=num_vehicles_left+num_vehicles_right;

bridge.k_on=0;

sema_init(&emL,0);
sema_init(&emR, 0);
sema_init(&norL, 0);
sema_init(&norR,0);

}


void arrive_bridge(enum car_priority prio, enum car_direction dir)
{ 



if(bridge.k_emL>bridge.k_emR && f ==0)
{
cur_dir=0;
f=1;
}
if(bridge.k_emR>bridge.k_emL && f ==0)
{
cur_dir=1;
f=1;
}


if(bridge.k_norL>bridge.k_norR && bridge.k_on==0 && bridge.k_emL==0 && bridge.k_emR==0 )
{
cur_dir=0;
}
if(bridge.k_norR>bridge.k_norL && bridge.k_on==0 && bridge.k_emL==0 && bridge.k_emR==0)
{
cur_dir=1;
}






if (bridge.k_on==0 || bridge.k_on==1)
{
   if(bridge.k_norR==0 && bridge.k_emR==0)
      cur_dir=0;
   else{
      if(bridge.k_norL==0 && bridge.k_emL==0){
        cur_dir=1;
        }
       }
}


int ch=0;

//printf(" cur_prio=  %d   cur_dir=  %d  ",prio,dir);

if(prio == 1){
   if(dir==0){
     if(bridge.k_on==2 || cur_dir==1)
{ //printf(" hi2");
     ch=1;
     sema_down(&emL);
     }
       else
     {
  //printf(" hi2");
     bridge.k_emL--;

   bridge.k_on++;
     f=1;
     }
   }
   else{
     if(bridge.k_on==2 || cur_dir==0){
//printf(" hi2");
     ch=1;
     sema_down(&emR);
     }
       else
     {
//printf(" hi2");
     bridge.k_emR--;
     f=1;
   bridge.k_on++;
     }
   }
}

if(prio == 0){
   if(dir==0){
     if(bridge.k_on==2 || cur_dir==1 || bridge.k_emL){






//printf(" 1 cur_k_on= %d ",bridge.k_on);
//printf(" 1 cur_dir2= %d ",cur_dir);
//printf(" 1 bridge.k_emL= %d ",bridge.k_emL);
     ch=1;
     sema_down(&norL);
     }
     else
     {
//printf(" 2 cur_k_on= %d ",bridge.k_on);
//printf(" 2 cur_dir2= %d ",cur_dir);
//printf(" 2 bridge.k_emL= %d ",bridge.k_emL);
//printf(" hi2");


    bridge.k_on++;
     bridge.k_norL--;
     f=1;
     }
   }
   else{
     if(bridge.k_on==2 || cur_dir==0 || bridge.k_emR ){
//printf(" hi2");
//printf(" 3 cur_k_on= %d ",bridge.k_on);
//printf(" 3 cur_dir2= %d ",cur_dir);
//printf(" 3 bridge.k_emL= %d ",bridge.k_emL);
     ch=1;
     sema_down(&norR);
     }
       else
     {


//printf(" 4 cur_k_on= %d ",bridge.k_on);
//printf(" 4 cur_dir2= %d ",cur_dir);
//printf(" 4 bridge.k_emL= %d ",bridge.k_emL);
     //printf(" hi2");
     bridge.k_on++;
     bridge.k_norR--;
    f=1;
     }

   }
}

//printf(" cur_dir=  %d  ",cur_dir);
}

void exit_bridge(enum car_priority prio, enum car_direction dir)
{
bridge.k_on--;
//printf(" hi1");
if(bridge.k_on ==0)
{
        if(cur_dir==0  && (!(bridge.k_emR==0 && bridge.k_emL) || (bridge.k_emR || bridge.k_norR)))
{   
         cur_dir=1;
         }
 else if(cur_dir==1  && (!(bridge.k_emL==0 && bridge.k_emR) || (bridge.k_emL || bridge.k_norL)))
{   //R
         //ch_dir=1;
         cur_dir=0;
         }

//printf(" ! cur_k_on= %d ",bridge.k_on);
    while(bridge.k_on<2)
    {
     int ch=0;
    if(cur_dir==0 && bridge.k_emL  )
      {
//printf(" 5 cur_k_on= %d ",bridge.k_on);
//printf(" 5 cur_dir2= %d ",cur_dir);
//printf(" 5 bridge.k_emL= %d ",bridge.k_emL);
      sema_up(&emL);
      bridge.k_emL--;
      bridge.k_on++;
      ch=1;
      }
else if(cur_dir==0 && bridge.k_norL && bridge.k_emL==0 )
           {
//printf(" 6 cur_k_on= %d ",bridge.k_on);
//printf(" 6 cur_dir2= %d ",cur_dir);
//printf(" 6 bridge.k_emL= %d ",bridge.k_emL);
            sema_up(&norL);
      bridge.k_norL--;
      bridge.k_on++;
      ch=1;
           }
else if(cur_dir==0 && bridge.k_norL==0 && bridge.k_emL==0 && bridge.k_norR )
           {
//printf(" 6 cur_k_on= %d ",bridge.k_on);
//printf(" 6 cur_dir2= %d ",cur_dir);
//printf(" 6 bridge.k_emL= %d ",bridge.k_emL);
            sema_up(&norR);
      bridge.k_norR--;
      bridge.k_on++;
      ch=1;
           }
    else  if (cur_dir==1 && bridge.k_emR )
          {
//printf(" 7 cur_k_on= %d ",bridge.k_on);
//printf(" 7 cur_dir2= %d ",cur_dir);
//printf(" 7 bridge.k_emL= %d ",bridge.k_emL);
          sema_up(&emR);
      bridge.k_emR--;
      bridge.k_on++;
      ch=1;
          }
     
   else if(cur_dir==1 && bridge.k_norR && bridge.k_emR==0 )

           {
//printf(" hi1");
//printf(" 8 cur_k_on= %d ",bridge.k_on);
//printf(" 8 cur_dir2= %d ",cur_dir);
//printf(" 8 bridge.k_emL= %d ",bridge.k_emL);
            sema_up(&norR);
      bridge.k_norR--;
      bridge.k_on++;
      ch=1;
           }
else if(cur_dir==1 && bridge.k_norR==0 && bridge.k_emR==0 && bridge.k_norL )
           {
//printf(" 6 cur_k_on= %d ",bridge.k_on);
//printf(" 6 cur_dir2= %d ",cur_dir);
//printf(" 6 bridge.k_emL= %d ",bridge.k_emL);
            sema_up(&norL);
      bridge.k_norL--;
      bridge.k_on++;
      ch=1;
           }
//printf(" !1 cur_k_on= %d ",bridge.k_on);
if(ch==0)
break;
    }
}

//printf(" cur_dir=  %d  ",cur_dir);
}
