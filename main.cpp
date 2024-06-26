#include<SDL2/SDL.h>
#include<iostream>
#include<math.h>
#include<string.h>
#define length 1000
#define width 700
#define n 3
#define grav_const 100
void collision_detection();
void body_collision();
float offset_x,offset_y;
float COM_x,COM_y;
void calc_COM();
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

class bodies{
    public:
        float mass,ax=0,ay=0,x,y,radius=10,vx,vy,dt=0.001;

        void calc_vel()
        {
        	vx=vx+ax*dt;
        	vy=vy+ay*dt;
        }
        void move(void)
        {
            x=x+vx*dt;
            y=y+vy*dt;
        }
        void flush_acc()
        {
            ax=0;
            ay=0;
        }
};

bodies body[3];

void update_info()
{
    // Initialize positions
    body[0].x = 500;  // Body 1 at (700, 350)
    body[0].y = 200;
    
    body[1].x = 300;  // Body 2 at (300, 550)
    body[1].y = 350;
    
    body[2].x = 900;  // Body 3 at (300, 150)
    body[2].y = 350;

    // Initialize velocities
    float R = 200;
    float v_initial = 100*sqrt(grav_const * body[0].mass / R);  // Initial velocity magnitude
    
    body[0].vx = -0;
    body[0].vy = 0;
    
    body[1].vx = 0;
    body[1].vy = 167;
    
    body[2].vx = 0;
    body[2].vy = 176;

    // Set mass and timestep
    for (int i = 0; i < 3; ++i) {
        body[i].mass = 1000000;  // Set mass to 10000 for each body
        body[i].dt = 0.001;    // Set timestep dt to 0.001
    }
}


void calc_acc(float m1,float m2,float x1,float y1,float x2,float y2,float distance,int i)
{
	float dx=x2-x1;
	float dy=y2-y1;
	

	float nx=dx/distance;
	float ny=dy/distance;

	float acc=(grav_const*m2)/(distance*distance);

	float acc_nx=nx*acc;
	float acc_ny=ny*acc;

	body[i].ax=body[i].ax+acc_nx;
	body[i].ay=body[i].ay+acc_ny;


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
        float distance;

        

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(i!=j)
                {
                    distance=sqrt((body[j].x-body[i].x)*(body[j].x-body[i].x)+(body[j].y-body[i].y)*(body[j].y-body[i].y));
                    calc_acc(body[i].mass,body[j].mass,body[i].x,body[i].y,body[j].x,body[j].y,distance,i);
                }
            }
        }

        for(int i=0;i<3;i++)
        {
            body[i].calc_vel();
            body[i].move();
        }

        
        
        body[0].flush_acc();
        body[1].flush_acc();
        body[2].flush_acc();
        body_collision();
        //collision_detection();
    
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        calc_COM();
        float offset_x=length/2-COM_x;
        float offset_y=width/2-COM_y;

        DrawCircle(renderer,body[0].x+offset_x,body[0].y+offset_y,body[0].radius);
        DrawCircle(renderer,body[1].x+offset_x,body[1].y+offset_y,body[1].radius);
        DrawCircle(renderer,body[2].x+offset_x,body[2].y+offset_y,body[2].radius);

        SDL_RenderPresent(renderer);

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        
        
        SDL_RenderClear(renderer);


    }
    
}
int main(int argv,char** args )
{
    update_info();
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
void calc_COM()
{
    COM_x=((body[0].mass*body[0].x)+(body[1].mass*body[1].x)+(body[2].mass*body[2].x))/(body[0].mass+body[1].mass+body[2].mass);
    COM_y=((body[0].mass*body[0].y)+(body[1].mass*body[1].y)+(body[2].mass*body[2].y))/(body[0].mass+body[1].mass+body[2].mass);
}
void body_collision() {
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(i!=j)
            {
                float dx=body[j].x-body[i].x;
                float dy=body[j].y-body[i].y;

                float distance=sqrt(dx*dx+dy*dy);

                if(distance<=body[i].radius+body[j].radius)
                {

                    float nx=dx/distance;
                    float ny=dy/distance;

                    float v1n=body[i].vx*nx+body[i].vy*ny;
                    float v2n=body[j].vx*nx+body[j].vy*ny;

                    float v1nx=v1n*nx;
                    float v1ny=v1n*ny;
    
                    float v2nx=v2n*nx;
                    float v2ny=v2n*ny;

                    float v1tx=body[i].vx-v1nx;
                    float v1ty=body[i].vy-v1ny;
    
                    float v2tx=body[j].vx-v2nx;
                    float v2ty=body[j].vy-v2ny;

                    float new_v1nx=v2nx;
                    float new_v1ny=v2ny;
                    float new_v2nx=v1nx;
                    float new_v2ny=v1ny;

                    float k = (body[i].radius+body[j].radius-distance)/2;

    
                    body[i].vx=new_v1nx+v1tx;
                    body[i].vy=new_v1ny+v1ty;
        
                    body[j].vx=new_v2nx+v2tx;
                    body[j].vy=new_v2ny+v2ty;

                    body[i].x=body[i].x+(k/sqrt(pow(body[i].vx,2)+pow(body[i].vy,2)))*body[i].vx;
                    body[i].y=body[i].y+(k/sqrt(pow(body[i].vx,2)+pow(body[i].vy,2)))*body[i].vy;
                    body[j].x=body[j].x+(k/sqrt(pow(body[i].vx,2)+pow(body[i].vy,2)))*body[j].vx;
                    body[j].y=body[j].y+(k/sqrt(pow(body[j].vx,2)+pow(body[j].vy,2)))*body[j].vy;
                    body[i].move();
                    body[j].move();
                }
            }
        }
    }

}
void collision_detection() {
    for (int i = 0; i < 3; i++) {
        if (body[i].x - body[i].radius <= 0) {
            float m=body[i].vy/body[i].vx;
            body[i].y=m*(body[i].radius-body[i].x)+body[i].y;
            body[i].x=body[i].radius;
            body[i].vx=-body[i].vx;
        }
        if (body[i].x + body[i].radius >= length) {
            float m=body[i].vy/body[i].vx;
            body[i].y=m*(length-body[i].radius-body[i].x)+body[i].y;
            body[i].x=length-body[i].radius;
            body[i].vx=-body[i].vx;
        }
        if (body[i].y - body[i].radius <= 0) {
            float m=body[i].vy/body[i].vx;
            body[i].x=1/m*(body[i].radius-body[i].y)+body[i].x;
            body[i].y=body[i].radius;
            body[i].vy=-body[i].vy;
        }
        if (body[i].y + body[i].radius >= width) {
            float m=body[i].vy/body[i].vx;
            body[i].x=1/m*(width-body[i].radius-body[i].y)+body[i].x;
            body[i].y=width-body[i].radius;
            body[i].vy=-body[i].vy;
        }
    }
}