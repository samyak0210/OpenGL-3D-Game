#include "main.h"
#include "timer.h"
#include "boat.h"
#include "block.h"
#include "water.h"
#include "cube.h"
#include "cannon.h"
#include "coin.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
char player[1000];
float wave_speed = -0.05;
Coin coins[100];
Boat boat;
int boss_kill =0,kill=0,boss_tick=0;
float boss_x=1000,boss_z=1000;
Cube enemy[100],fireball,boss,barrels[100];
Cannon cannon;
Cube boss_cube;
int v=1,boat_x,boat_z,flag=0,tick=0,score=0,lives=500,coin=0;
Water sea;
float eyex,eyey,eyez,fireball_speed=3.0;
Block block[100];
float screen_zoom = 0.5, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0,angle=0;
int tick_flag,wind_flag=0,wind_angle,fire=0;
float fireball_angle=0;
Timer t60(1.0 / 60);
glm::vec3 eye;
glm::vec3 target;
glm::vec3 up;
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    if(tick==0)
        tick_flag=500+rand()%400;
    tick++;

    if(tick_flag == tick){
        wind_angle = rand()%360;
        tick=0;
        wind_flag=100;
    }
    if(wind_flag){
        boat.position.x+=0.4*sin(wind_angle* M_PI/180.0);
        boat.position.z+=0.4*cos(wind_angle* M_PI/180.0);

        cannon.position.x+=0.4*sin(wind_angle* M_PI/180.0);
        cannon.position.z+=0.4*cos(wind_angle* M_PI/180.0);
        wind_flag--;
    }
    glUseProgram (programID);

    if(v==1){
      // Boat Follow
       eye[0]=boat.position.x - 75*sin(boat.rotation * M_PI/180.0);
       eye[1]=30;
       eye[2]=boat.position.z - 75*cos(boat.rotation * M_PI/180.0);

       up[0]=0;
       up[1]=1;
       up[2]=0;

       target[0]=boat.position.x;
       target[1]=20;
       target[2]=boat.position.z;
    }
    else if(v==2){
      // Up view
       eye[0]=boat.position.x;
       eye[1]=100;
       eye[2]=boat.position.z;

       up[0]=0;
       up[1]=0;
       up[2]=1;

       target[0]=boat.position.x;
       target[1]=boat.position.y;
       target[2]=boat.position.z;
    }
    else if(v==3){
      // Boat front view
      eye[0]=boat.position.x+18*sin(boat.rotation * M_PI/180.0);//-50*sin(boat.rotation * M_PI/180.0);
      eye[1]=10+boat.position.y;
      eye[2]=boat.position.z+18*cos(boat.rotation * M_PI/180.0);//-50*cos(boat.rotation * M_PI/180.0);

      up[0]=0;
      up[1]=1;
      up[2]=0;

      target[0]=boat.position.x+50*sin(boat.rotation * M_PI/180.0);
      target[1]=12+boat.position.y;
      target[2]=boat.position.z+50*cos(boat.rotation * M_PI/180.0);
    }
    else if(v==4){
      // Tower view
      eye[0]=boat_x;
      eye[1]=100;
      eye[2]=boat_z-50;

      up[0]=0;
      up[1]=1;
      up[2]=0;

      target[0]=boat_x+3;
      target[1]=10;
      target[2]=boat_z;

    }
    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 1*cos(camera_rotation_angle*M_PI/180.0f), 1*sin(camera_rotation_angle*M_PI/180.0f),0  );

    // glm::vec3 eye (boat.position.x - 75*sin(boat.rotation * M_PI/180.0),100,boat.position.z-75*cos(boat.rotation * M_PI/180.0));
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, -1, 0);


    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    cannon.draw(VP);
    sea.draw(VP);
    boat.draw(VP);
    fireball.draw(VP);
    for(int i=0;i<100;i++){
      block[i].draw(VP);
      coins[i].draw(VP);
      barrels[i].draw(VP);
    }
    for(int i=0;i<75;i++){
      if(detect_collision(enemy[i].bounding_box(),fireball.bounding_box())==0 or fire==0)
        enemy[i].draw(VP);
      else{
        enemy[i].position.x+=100+rand()%400;
        enemy[i].position.z+=100+rand()%400;
        score+=enemy[i].score;
        boss_kill++;
      }
    }
    if(boss_kill==3){
        boss_x = boat.position.x + 50 + rand()%100;
        boss_z = boat.position.z + 50 + rand()%100;
        boss_kill=0;
    }
    boss_cube.draw(VP);
    boss.draw(VP);
}

void jump(int flag1){
    if(flag1==0)
        return;
    boat.position.y+=boat.speed_y;
    boat.speed_y -= 0.1;
    if(boat.position.y<0.0){
        boat.position.y=0.0;
        boat.speed_y = 2.0;
        flag=0;
    }
}

void view5(float x, float z) {
        if (v == 5){
            eye[0] = boat.position.x;
            eye[1] = 100;
            eye[2] = boat.position.z-100;

            target[0] = boat.position.x+ (x-300)/300 * 50;
            target[1] = boat.position.y;
            target[2] = boat.position.z+ (z-300)/300 * 50;

            up[0] = 0;
            up[1] = 1;
            up[2] = 0;
        }
        else{
          if(x>=200 and x<=400){
            cannon.rotation = boat.rotation-((3*x/5) - 180);
            fireball.rotation = boat.rotation-((3*x/5) - 180);
          }
        }
}


void tick_input(GLFWwindow *window) {
    jump(flag);
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int f = glfwGetKey(window, GLFW_KEY_F);
    int view1 = glfwGetKey(window, GLFW_KEY_1);
    int view2 = glfwGetKey(window, GLFW_KEY_2);
    int view3 = glfwGetKey(window, GLFW_KEY_3);
    int view4 = glfwGetKey(window, GLFW_KEY_4);
    int view5 = glfwGetKey(window, GLFW_KEY_5);
    if (left) {
        boat.rotation+=1;
        if(down)
          boat.rotation-=2;
        cannon.rotation+=1;
    }
    if (right){
        boat.rotation-=1;
        if(down)
          boat.rotation+=2;
        cannon.rotation-=1;
    }
    if(up){
        angle = ((int)boat.rotation)%360;
        boat.position.z+=boat.speed_z;
        boat.position.x+=boat.speed_x;

        // cannon.position.z+=1.6*cos(angle * M_PI / 180.0f);
        // cannon.position.x+=1.0*sin(angle * M_PI / 180.0f);

    }
    if(down){
        angle = ((int)boat.rotation)%360;
        boat.position.z-=boat.speed_z;
        boat.position.x-=boat.speed_x;

        cannon.position.z-=boat.speed_z;
        cannon.position.x-=boat.speed_x;
    }
    if(space and flag==0){
      flag=1;
      jump(flag);
    }
    if(f and fire==0){
      fire = 1;
      fireball_angle = cannon.rotation;
    }
    if(view1){
        v=1;
    }
    if(view2){
        v=2;
    }
    if(view3){
        v=3;
    }
    if(view4){
        v=4;
        boat_x = boat.position.x;
        boat_z = boat.position.z;
    }
    if(view5){
        v=5;
    }
}

void tick_elements() {
  cannon.position.z=boat.position.z;
  cannon.position.x=boat.position.x;
    if(kill==0){
        boss = Cube(0,0,0,30,COLOR_RED);
    }
    else if(kill==1){
        boss = Cube(0,0,0,25,COLOR_YELLOW);
    }
    else if(kill==2){
        boss = Cube(0,0,0,20,COLOR_BLACK);
    }
    else if(kill==3){
        boss_x = 1000;
        boss_z = 1000;
        score+=100;
        boss_cube.position.x = boss.position.x;
        boss_cube.position.z = boss.position.z;
        kill=0;
    }
    boss.position.x = boss_x;
    boss.position.z = boss_z;
    if(detect_collision(boss_cube.bounding_box(),boat.bounding_box())) {
        boss_cube.position.x = 1000;
        boss_cube.position.z = 1000;
        if(rand()%2==0)
          boss_tick=300;
        else
          if(lives+10>500)
            lives=500;
          else
            lives+=10;
    }
    if(detect_collision(boss.bounding_box(),boat.bounding_box())){
        boat.position.z-=3;
        lives-=5;
    }
    if(boss_tick){
        boat.position.x+=1.5*sin(boat.rotation * M_PI/180.0);
        boat.position.z+=1.5*cos(boat.rotation * M_PI/180.0);
        boss_tick--;
    }
    if(detect_collision(boss.bounding_box(),fireball.bounding_box())){
        kill++;
        fire=0;
    }

    if(flag==0){
        sea.position.y += wave_speed;
        if(sea.position.y<=-2.0 or sea.position.y>=0.0)
          wave_speed=-wave_speed;
    }

    cannon.position.y=boat.position.y;
    if(lives<=0)
      quit(window);
    if(fireball.position.y<0.0){
        fire=0;
        fireball.position.y=0.0;
        fireball_speed=3.0;
    }

    if(fire==1){
        fireball.position.x+=2.0*sin(fireball_angle * M_PI / 180.0f);
        fireball.position.z+=2.0*cos(fireball_angle * M_PI / 180.0f);
        fireball.position.y+=fireball_speed;
        fireball_speed-=0.1;
    }
    angle = ((int)boat.rotation)%360;
    boat.speed_x = 1.0*sin(angle * M_PI / 180.0f);
    boat.speed_z = 1.0*cos(angle * M_PI / 180.0f);
    if(fire==0){
      fireball.position.x = boat.position.x;
      fireball.position.z = boat.position.z;
      fireball.position.y = boat.position.y;
    }
    for(int i=0;i<100;i++){
        if(detect_collision(boat.bounding_box(),block[i].bounding_box()) and boss_tick==0) {
            boat.position.z-=3;
            cannon.position.z-=3;
            lives-=1;
        }
        if(detect_collision(boat.bounding_box(),coins[i].bounding_box()) and boss_tick==0){
            coin++;
            coins[i].position.x+=50;
            coins[i].position.z+=50;
            barrels[i].position.x+=50;
            barrels[i].position.z+=50;
        }
        coins[i].rotation+=10;
    }
    for(int i=0;i<75;i++){
      // enemy[i].position.x+=0.6*cos(boat.rotation * M_PI/180.0);
      // enemy[i].position.z+=0.6*sin(boat.rotation * M_PI/180.0);
      if(detect_collision(boat.bounding_box(),enemy[i].bounding_box()) and boss_tick==0){
          boat.position.z-=1;
          cannon.position.z-=1;
          lives-=2;
      }
    }
    sea.position.x = boat.position.x;
    sea.position.z = boat.position.z;

    sprintf(player,"\t\t\t\t\tScore: %d | Lives: %d |Coins: %d",score,lives,coin);
    glfwSetWindowTitle(window,player);

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat = Boat(0, 0,0, COLOR_RED);

    for(int i=0;i<100;i++){
      int x=-300+rand()%1000;
      int z=-300+rand()%1000;
      block[i] = Block(x,0,z,COLOR_BLACK);
    }
    cannon = Cannon(0,0,0,COLOR_GREY);
    fireball = Cube(0,0,0,1,COLOR_ORANGE);
    for(int i=0;i<100;i++){
      int x=-300+rand()%800;
      int z=200+rand()%800;
      int index = rand()%4;
      color_t arr[] = {COLOR_GREEN,COLOR_BROWN,COLOR_GREY,COLOR_VIOLET};
      int sc[] = {5,10,15,20};
      enemy[i]=Cube(x,0,z,12,arr[index]);
      enemy[i].score = sc[index];
      enemy[i].angle = rand()%360;
    }
    for(int i=0;i<100;i++){
      int x=-300+rand()%900;
      int z=200+rand()%1000;
      coins[i]=Coin(x,20.0,z,COLOR_YELLOW);
      barrels[i]=Cube(x,0,z,6,COLOR_YELLOW);
    }
    boss = Cube(0,0,0,30,COLOR_RED);
    boss_cube = Cube(1000,0,1000,10,COLOR_ORANGE);
    sea = Water(0,0,0,COLOR_BLUE);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            tick_input(window);
            tick_elements();

        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x)  < (a.width + b.width)) &&
           (abs(a.z - b.z)  < (a.length + b.length)) && (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    // float top    = screen_center_y + 4 / screen_zoom;
    // float bottom = screen_center_y - 4 / screen_zoom;
    // float left   = screen_center_x - 4 / screen_zoom;
    // float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::perspective(45.0f,1.0f,10.0f,250.0f);
    // Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
