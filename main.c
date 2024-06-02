#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<math.h>
#define length 30
#define width 20
void draw_grid(int,int);
int check (int,int);
void move_particle(void);
int about_to_collide(void);
void change_velocity(void);
int particle_x_position;
int particle_y_position;
int particle_x_velocity;
int particle_y_velocity;
int main(void)
{
    int x,y;
    printf("Ënter the initial position of the particle (x,y) : ");
    scanf("%d%d",&particle_x_position,&particle_y_position);
    printf("Ënter the velocity of the particle (x,y) : ");
    scanf("%d%d",&particle_x_velocity,&particle_y_velocity);
    x=particle_x_position;
    y=particle_y_position;
    while(1)
    {
    
        system("cls");
        if(about_to_collide()==1)
        {
            change_velocity();
        }
        else
        {
            move_particle();
        }
         draw_grid(width,length);
         if((particle_x_position==1||particle_x_position==length-2)&&(particle_y_position==1||particle_y_position==width-2))
    {
        printf("game over");
        exit(1);
    }
    else if(x==particle_x_position&&y==particle_y_position){
        printf("Repeated path");
        exit(1);
    }
    
    }
}
void draw_grid(int row,int col)
{
    int i,j;
    for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            if(check(j,i)==1)
            {
                printf("# ");
            }
            else if((i==0||i==row-1)||(j==0||j==col-1))
            {
                printf("$ ");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}
int check (int x ,int y)
{
    if (x==particle_x_position&&y==particle_y_position)
    {
        return 1;
    }
   
    return 0;
}
void move_particle(void)
{
    particle_x_position=particle_x_position+particle_x_velocity;
    particle_y_position=particle_y_position+particle_y_velocity;
}
int about_to_collide (void)
{

   if(particle_x_position+particle_x_velocity<1||particle_x_position+particle_x_velocity>length-2)
   {
        return 1;
   }
   
   if(particle_y_position+particle_y_velocity<1||particle_y_position+particle_y_velocity>width-2)
   {
        return 1;
   }
    return 0;
}
void change_velocity(void)
{
    if(particle_x_position+particle_x_velocity<1)
   {
       
        particle_x_position=2+fabs(particle_x_velocity)-particle_x_position;
        particle_x_velocity=(-1)*particle_x_velocity;
   }
   
    else if(particle_x_position+particle_x_velocity>length-2)
   {
        
        particle_x_position=2*(length-2)-particle_x_position-fabs(particle_x_velocity);
        particle_x_velocity=(-1)*particle_x_velocity;
   }
   else{
    particle_x_position=particle_x_position+particle_x_velocity;
   }
    if(particle_y_position+particle_y_velocity<1)
   {
        
        particle_y_position=2+fabs(particle_y_velocity)-particle_y_position;
        particle_y_velocity=(-1)*particle_y_velocity;
   }
    else if(particle_y_position+particle_y_velocity>width-2)
   {
        
        particle_y_position=2*(width-2)-particle_y_position-fabs(particle_y_velocity);
        particle_y_velocity=(-1)*particle_y_velocity;
   }
   else{
    particle_y_position=particle_y_position+particle_y_velocity;
   }
}
