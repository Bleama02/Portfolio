
//File Name: Blea_FinalProject.cpp
//Description: Make a 2x2x2 rubiks cube that the user can manipulate 
//Author: Marcus Blea
//Date: 12/15/2022
/*
Key Binds:   F1 = top Rotate Right
             F3 = Rotate Right side up
             F4 = Rotate Right Side Down
             F5 = Rotate front face right
             F6 = Rotate front face Left
             F7 = Rotate Left side up
             F8 = Rotate Left side Down 
             F9 = Rotate top Left 
*/
#include "GLUtilities.h"
#include "Camera.h"


// Function protoypes
void enableLights();
void display(void);
void resetScene();
void reshape(GLsizei width, GLsizei height);
void keyboardClick(unsigned char key, GLsizei x, GLsizei y);
void bottomTurn(int button, int x, int y);
void mouseClick(int button, int state, GLsizei x, GLsizei y);
void mouseMotion(GLsizei x, GLsizei y);
void timer(int millisec);




//Global Varibles
bool blLightsAreOn = true;
GLfloat rotationX, rotationY;   // Current rotation of the center object
GLfloat rotationXBottom = 0.0f, rotationXTop = 0.0f, rotationYSideLeft = 0.0f, rotationYSideRight = 0.0f, rotationZFront = 0.0f, rotationZBack = 0.0f;
GLfloat locationX, locationY;   // Current location of the center object
int orbitRotation;              // Rotation in degrees 
GLsizei prevMouseX, prevMouseY; // Current mouse location in the window
GLsizei windowWidth = 640, windowHeight = 480;
bool blMouseLeftDown;           // Current state of the windoww
bool blMouseMiddleDown;         // Current state of the windoww
bool blMouseRightDown;          // Current state of the windoww
const int BOTTOM_CUBES = 4;

float cube_positions1[BOTTOM_CUBES][3] = {
  {1.0f, -1.0f, 1.0f},  // Cube 1
  {0.0f, -1.0f, 1.0f},  // Cube 2
  {0.0f, -1.0f, 0.0f},  // Cube 3
  {0.0f, -1.0f, 0.0f},  // Cube 4
};

GLfloat x1 = 1.0f, t1 = -1.0f, z1 = 1.0f;
GLfloat x2 = 0.0f, y2 = -1.0f, z2 = 1.0f;
GLfloat x3 = 0.0f, y3 = -1.0f, z3 = 0.0f;
GLfloat x4 = 1.0f, y4 = -1.0f, z4 = 0.0f;


Material ground;
Material cubeMaterials[6];
Material teapot;
Material sky;
GLUquadric *q;


Camera avatarPOV;

int main(int argc, char** argv)
{
    
    //glutInit is used to initialize the GLUT library. 
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize(windowWidth, windowHeight); 
    glutInitWindowPosition(50, 50); // Postion's the window's top-left corner 
    glutCreateWindow("Textures Everywhere!");

    // Callback Functions
    glutDisplayFunc(&display);        // Callback to display
    glutReshapeFunc(&reshape);       
    glutKeyboardFunc(&keyboardClick); 
    glutSpecialFunc(&bottomTurn);
    glutMouseFunc(&mouseClick);       
    glutMotionFunc(&mouseMotion);    
    glutTimerFunc(20, &timer, 20);    

    //load the materials...
    ground.load("Textures/Tiles1.png");
    cubeMaterials[0].load("Textures/redSquare3.png");
    cubeMaterials[1].load("Textures/blueSquare.png");
    cubeMaterials[2].load("Textures/whiteSquare3.png");
    cubeMaterials[3].load("Textures/greenSquare.png");
    cubeMaterials[4].load("Textures/orangeSquare.png");
    cubeMaterials[5].load("Textures/yelloweSquare.png");
    teapot.load("Textures/Bricks2.png");
    sky.load("Textures/Sky1.png");

    q = gluNewQuadric();    //Allocate the memory for a new quadric
    gluQuadricNormals(q, GL_SMOOTH);  //Generate smooth normals for the quadric
    gluQuadricTexture(q, GL_TRUE);  //Enable texture coordinates for the quadric
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);  
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);  

    resetScene();
    glutMainLoop();
    return EXIT_SUCCESS;
}

void display(void)
{  
    avatarPOV.runCamera();
    GLfloat repeatNum = 100.0f;
    reshape(windowWidth, windowHeight);
    
    enableLights();
    #if defined__APPLE__
    glClear(GL_COLOR_BUFFER_BIT);
    #else
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    #endif
    glEnable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);         // Swtich to model view mode (objects)
    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
   
    
    glPushMatrix();                                             //bottom left front
    glTranslatef(x4, y4, z4); 
    glRotatef(rotationXBottom, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationYSideLeft, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZFront, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();                       // restroes the current state of the matrix

    glPushMatrix();                        // bottom right front
    glTranslatef(x3, y3, z3); 
    glRotatef(rotationXBottom, 0.0f, 1.0f, 0.0f); 
    glRotatef(rotationYSideRight, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZFront, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();

    glPushMatrix();                      //top right front 
    glTranslatef(0.0f, 0.0f, 0.0f); 
    glRotatef(rotationXTop, 0.0f, 1.0f, 0.0f); 
    glRotatef(rotationYSideRight, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZFront, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();

    glPushMatrix();                    //top left front
    glTranslatef(1.0f, 0.0f, 0.0f); 
    glRotatef(rotationXTop, 0.0f, 1.0f, 0.0f); 
    glRotatef(rotationYSideLeft, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZFront, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();

    glPushMatrix();                        //Back left top
    glTranslatef(1.0f, 0.0f, 1.0f); 
    glRotatef(rotationXTop, 0.0f, 1.0f, 0.0f); 
    glRotatef(rotationYSideLeft, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZBack, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();

    glPushMatrix();                         //Back right top
    glTranslatef(0.0f, 0.0f, 1.0f); 
    glRotatef(rotationXTop, 0.0f, 1.0f, 0.0f); 
    glRotatef(rotationYSideRight, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZBack, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();

    glPushMatrix();                         //Back right bottom
    glTranslatef(x2, y2, z2); 
    glRotatef(rotationXBottom, 0.0f, 1.0f, 0.0f); 
    glRotatef(rotationYSideRight, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZBack, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();

    glPushMatrix();                        //Back left bottom
    glTranslatef(x1, t1, z1); 
    glRotatef(rotationXBottom, 0.0f, 1.0f, 0.0f); 
    glRotatef(rotationYSideLeft, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationZBack, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    //glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
    drawTextureCube(cubeMaterials, 2.0f);
    glPopMatrix();
    
    
    
    //Draw the orbiting object
    

    //draw the ground
    GLfloat groundSize = 1000.0f;
    GLfloat groundHeight = -15.0f;
    
    glPushMatrix();
    ground.paintMaterial();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, repeatNum);
    glVertex3f(-groundSize/2.0f, groundHeight , groundSize/2.0f);
    glTexCoord2f(repeatNum, repeatNum);
    glVertex3f(groundSize/2.0f, groundHeight , groundSize/2.0f);
    glTexCoord2f(repeatNum, 0.0f);
    glVertex3f(groundSize/2.0f, groundHeight , -groundSize/2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-groundSize/2.0f, groundHeight , -groundSize/2.0f);
    glEnd();
    ground.stopMaterial();
    glPopMatrix();           

    //draw sky
    

    //glTranslatef(0.0f, 0.0f, -10.0f);
    glutSwapBuffers(); // call once and only once
}

void enableLights()
{
    if(blLightsAreOn)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        GLfloat lightsKa[] = {0.4f, 0.4f, 0.4f,1.0f};   // ambient light
        GLfloat lightsKd[] = {0.7f, 0.7f, 0.7f, 1.0f};  // diffuse light
        GLfloat lightsKs[] = {1.0f, 1.0f, 1.0f,1.0f};  // specular light
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightsKa);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightsKd);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightsKs);

        GLfloat lightPos[] = {0.0f, 0.0f, -13.0f, 1.0f}; // location of the light
        glLightfv (GL_LIGHT0, GL_POSITION, lightPos);
        glEnable(GL_LIGHT0);


    }
    else
    {
        glDisable(GL_LIGHTING);

    }

}

void resetScene()
{
    avatarPOV.setLocation(0.0f, 0.0f, -7.0f);
    avatarPOV.setRotation(0.0f, 0.0f, 1.0f);
    avatarPOV.setProjectionType(CAM_PROJ_PERSPECTIVE);
    blLightsAreOn = true;
    rotationX = 0.0f;
    rotationY = 0.0f;
    locationX = 0;
    locationY = 0;
    prevMouseX = 0;
    prevMouseY = 0;
    //prevMouseX, prevMouseY;
    blMouseLeftDown = false;
    blMouseMiddleDown = false;
    blMouseRightDown = false;

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f); 
    glClearDepth(1.0f);                      
    glEnable(GL_DEPTH_TEST);                
    glDepthFunc(GL_LEQUAL);                 
    glShadeModel(GL_SMOOTH);                 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
    glFrontFace (GL_CCW);
    glutPostRedisplay();

}

void reshape(GLsizei width, GLsizei height)
{
 
    
    if (height <= 0)  height = 1;           // Sanity checks. Atlest keep it at one
    if (width <= 0)  width = 1;             // Sanity Checks. Atleast keep it at 1
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    windowWidth = width;
    windowHeight = height;

    avatarPOV.setupCamera(CAM_PROJ_PERSPECTIVE, width, height, 45.0f, 0.01f, 5000.0f);

    // Set the viewport to cover the entire window
    glViewport(0, 0, width, height);
}

void keyboardClick(unsigned char key, GLsizei x, GLsizei y)
{

    switch (key)
    {
        case 'q': case 'Q': case 27:
            exit(EXIT_SUCCESS);
            break;
        case 'L': 
            blLightsAreOn = !blLightsAreOn;
      break;
        case 'r': case 'R' :
            resetScene();
            break;
        case 'w':
            avatarPOV.moveForward(0.5f);
            break; 
        case 's':
            avatarPOV.moveBackward(0.1f);
            break; 
        case 'a':
            avatarPOV.strafeLeft(0.5f);
            break;   
        case 'd':
            avatarPOV.strafeRight(0.5f);
            break;
        case '\'':
            avatarPOV.moveUp(0.1f);
            break;   
        case '/':
            avatarPOV.moveDown(0.1f);
            break;

    }
    glutPostRedisplay();

}



void bottomTurn(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
    {
        rotationXBottom -= 90.0f;
       
         // Rotate bottom to the left
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        rotationXBottom += 90.0f; // Rotate bottom to the righ
      
    }
    else if (key == GLUT_KEY_F1)
    {
        rotationXTop += 90.0f; // Top Rotate right
    }
    else if (key == GLUT_KEY_F9)
    {
        rotationXTop -= 90.0f; // Top Rotate left
    }
    else if (key == GLUT_KEY_F8)     
    {
        rotationYSideLeft += 90.0f; 
    }
    else if (key == GLUT_KEY_F7)     
    {
        rotationYSideLeft -= 90.0f; //Rotate left Side down
    }
    else if (key == GLUT_KEY_F3)    
    {
        rotationYSideRight += 90.0f; //Rotate left Side down
    }
    else if (key == GLUT_KEY_F4)    
    {
        rotationYSideRight -= 90.0f; //Rotate left Side downw
    }
    
    else if (key == GLUT_KEY_F5)     
    {
        rotationZFront -= 90.0f; //Rotate face left
    }
    else if (key == GLUT_KEY_F6)     
    {
        rotationZFront += 90.0f; //Rotate face right
    }
    // Request that the display be updated
    glutPostRedisplay();
}



void mouseClick(int button, int state, GLsizei x, GLsizei y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN) blMouseLeftDown = true;
        else blMouseLeftDown = false;

    }
    if (button == GLUT_MIDDLE_BUTTON)
    {
        if (state == GLUT_DOWN) blMouseMiddleDown = true;
        else blMouseMiddleDown = false;

    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN) blMouseRightDown = true;
        else blMouseRightDown = false;

    }
    prevMouseX = x;
    prevMouseY = y;
    glutPostRedisplay();

  
}
void mouseMotion(GLsizei x, GLsizei y)
{
    if (blMouseLeftDown)
    {
        if(x < prevMouseX) avatarPOV.turnLeft(2.0f);
        if(x > prevMouseX) avatarPOV.turnRight(2.0f);
        if(y > prevMouseY) avatarPOV.lookDown(2.0f);
        if(y < prevMouseY) avatarPOV.lookUp(2.0f);

        

    }
    if (blMouseRightDown)
    {
        if(x < prevMouseX) avatarPOV.strafeLeft(0.3f);
        if(x > prevMouseX) avatarPOV.strafeRight(0.3f);
        if(y < prevMouseY) avatarPOV.moveForward(0.3f);
        if(y > prevMouseY) avatarPOV.moveBackward(0.3f);
        locationX += 10.0f * (x - prevMouseX) / windowWidth;
        locationY += 10.0f * (y - prevMouseY) / windowHeight;
        //locationY += y - prevMouseY;

    }
    prevMouseX = x;
    prevMouseY = y;
    glutPostRedisplay();

}
void timer(int millisec)
{
    orbitRotation++;
    if (orbitRotation >= 360) orbitRotation -= 360;
    glutTimerFunc(millisec, &timer, millisec);
    glutPostRedisplay();

}