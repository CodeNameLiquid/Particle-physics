#include<SDL2/SDL.h>
#include <iostream>
#include<math.h>
#define length 1000
#define width 700
#define n 250
void render(SDL_Renderer*);
void part_info(void);
void collision_detection(void);
void particle_collision(void);
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}
class particles{
    public:
        float x,y,radius=2,vx=200,vy=200,dt=0.01;
        void move(void)
        {
            x=x+vx*dt;
            y=y+vy*dt;
        }
};
particles particle[1000];
int main(int argv,char** args )
{
    part_info();
        SDL_Window* window =nullptr;
        SDL_Renderer* renderer=nullptr;

        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(length,width, 0, &window, &renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
        render(renderer);
        SDL_RenderClear(renderer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        
       
        
        return 0;

}
void render(SDL_Renderer* renderer)
{
    
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
       
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;  
            }
        }
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        particle_collision();
        collision_detection();
        
        for(int i=0;i<n;i++)
        {
        DrawCircle(renderer,particle[i].x,particle[i].y,particle[i].radius);
        }
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderPresent(renderer);
        for(int i=0;i<n;i++)
        {
        particle[i].move();
        }
        SDL_RenderClear(renderer);
    }
    
}
void part_info(void)
{
    for(int i=0;i<n;i++){
    particle[i].x=(i+1)*4;
    particle[i].y=(i+1)*4;
    }
    particle[249].x=221;
    particle[249].y=444;
}
void collision_detection() {
    for (int i = 0; i < n; i++) {
        if (particle[i].x - particle[i].radius <= 0) {
            float m=particle[i].vy/particle[i].vx;
            particle[i].y=m*(particle[i].radius-particle[i].x)+particle[i].y;
            particle[i].x=particle[i].radius;
            particle[i].vx=-particle[i].vx;
        }
        if (particle[i].x + particle[i].radius >= length) {
            float m=particle[i].vy/particle[i].vx;
            particle[i].y=m*(length-particle[i].radius-particle[i].x)+particle[i].y;
            particle[i].x=length-particle[i].radius;
            particle[i].vx=-particle[i].vx;
        }
        if (particle[i].y - particle[i].radius <= 0) {
            float m=particle[i].vy/particle[i].vx;
            particle[i].x=1/m*(particle[i].radius-particle[i].y)+particle[i].x;
            particle[i].y=particle[i].radius;
            particle[i].vy=-particle[i].vy;
        }
        if (particle[i].y + particle[i].radius >= width) {
            float m=particle[i].vy/particle[i].vx;
            particle[i].x=1/m*(width-particle[i].radius-particle[i].y)+particle[i].x;
            particle[i].y=width-particle[i].radius;
            particle[i].vy=-particle[i].vy;
        }
    }
}

void particle_collision() {
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
    float dx=particle[j].x-particle[i].x;
    float dy=particle[j].y-particle[i].y;

    float distance=sqrt(dx*dx+dy*dy);

    float nx=dx/distance;
    float ny=dy/distance;

    float v1n=particle[i].vx*nx+particle[i].vy*ny;
    float v2n=particle[j].vx*nx+particle[j].vy*ny;

    float v1nx=v1n*nx;
    float v1ny=v1n*ny;
    
    float v2nx=v2n*nx;
    float v2ny=v2n*ny;

    float v1tx=particle[i].vx-v1nx;
    float v1ty=particle[i].vy-v1ny;
    
    float v2tx=particle[j].vx-v2nx;
    float v2ty=particle[j].vy-v2ny;

    float new_v1nx=v2nx;
    float new_v1ny=v2ny;
    float new_v2nx=v1nx;
    float new_v2ny=v1ny;

    float k = (particle[i].radius+particle[j].radius-distance)/2;

    if(distance<=particle[i].radius+particle[j].radius)
    {
        particle[i].vx=new_v1nx+v1tx;
        particle[i].vy=new_v1ny+v1ty;
        
        particle[j].vx=new_v2nx+v2tx;
        particle[j].vy=new_v2ny+v2ty;

        particle[i].x=particle[i].x+(k/sqrt(pow(particle[i].vx,2)+pow(particle[i].vy,2)))*particle[i].vx;
        particle[i].y=particle[i].y+(k/sqrt(pow(particle[i].vx,2)+pow(particle[i].vy,2)))*particle[i].vy;
        particle[j].x=particle[j].x+(k/sqrt(pow(particle[i].vx,2)+pow(particle[i].vy,2)))*particle[j].vx;
        particle[j].y=particle[j].y+(k/sqrt(pow(particle[j].vx,2)+pow(particle[j].vy,2)))*particle[j].vy;
    }
        }}

}

