/*
  File for 'molec' task implementation.
*/

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "devices/timer.h"

void init(void);

int k=0;
struct water_str{
int free;
int in_free;
int in_free_o;
int in_free_h;
struct condition not_full;
struct condition not_empty;
struct lock lock;

};

struct water_str water;

void init(void)
{
 water.free=3;
water.in_free=0;
water.in_free_o=0;
water.in_free_h=0;
cond_init(&water.not_empty);
cond_init(&water.not_full);
lock_init(&water.lock);
}

static void oxygen(void* arg UNUSED)
{
 
    // Not implemented.
lock_acquire(&water.lock);
water.in_free++;
water.in_free_o++;
 msg("O created.");
cond_wait(&water.not_full,&water.lock);
while(water.in_free<water.free && water.in_free_h <2 && water.in_free_o<1)
cond_wait(&water.not_empty,&water.lock);

cond_broadcast(&water.not_full, &water.lock);
lock_release(&water.lock);

}

static void hydrogen(void* arg UNUSED)
{
lock_acquire(&water.lock);
water.in_free++;
water.in_free_h++;
msg("H created.");
if (1) msg("In water now %d: o-%d h-%d",water.in_free, water.in_free_o, water.in_free_h);
while(water.in_free<water.free && water.in_free_h <2 && water.in_free_o<1)
cond_wait(&water.not_full,&water.lock);

if(water.in_free>=water.free && water.in_free_h >=2 && water.in_free_o>=1){
water.in_free-=3;
water.in_free_h-=2;
water.in_free_o--;
k++;
msg("Water_%d create for 10 ticks",k);
timer_sleep(10);
}

cond_broadcast(&water.not_empty,&water.lock);
lock_release(&water.lock);
    // Not implemented.

}

void test_molec(unsigned int num_oxygen, unsigned int num_hydrogen, int interval)
{
  int last_hyd = 1;
  unsigned int oxy = 0, hyd = 0;
  init();

  while ( (oxy < num_oxygen) || (hyd < num_hydrogen) )
  {
    timer_sleep(interval);

    if (oxy < num_oxygen && (last_hyd || (num_hydrogen == hyd)) )
    {
      oxy++;
      thread_create("oxygen", PRI_DEFAULT, &oxygen, 0);
      last_hyd = 0;
    }
    else if (hyd < num_hydrogen)
    {
      hyd++;
      thread_create("hydrogen", PRI_DEFAULT, &hydrogen, 0);
      last_hyd = 1;
    }
  }

  timer_msleep(5000);
  pass();
}
