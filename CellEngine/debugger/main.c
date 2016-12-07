#ifdef _WIN32
#include <windows.h>
#endif
#include "GL/freeglut.h"
#include "CellEngine.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Do not include this in your program, only for debug utilities */
#include "private/CellEngine_p.h"
#include "private/CESpace_p.h"

/* Defines. */
#define MOUSE_CAMERA_MOVE_MODE 1
#define MOUSE_BODIES_MOVE_MODE 2
#define MOUSE_SELECTION_RECTANGLE_MODE 4

#define DEBUG_DRAW_SPATIAL_TABLE 1
#define DEBUG_DRAW_OCCUPIED_BOXES 2
#define DEBUG_DRAW_LIMITS 4
#define DEBUG_DRAW_CONJUGATION 8
#define DEBUG_DRAW_SEGMENT_POINTS 16
#define DEBUG_DRAW_BODIES_PER_BOX 32
#define DEBUG_DRAW_RINGS 64
#define DEBUG_DRAW_ROTATIONS 128
#define DEBUG_DRAW_PNEUMATIC_EFFECT 256

#define DEBUG_WRITE_NEIGHBORHOOD 1
#define DEBUG_WRITE_PNEUMATIC_EFFECT 2

#define BODY_VERTEX_INCREMENT 1.0f

/* Window and mouse. */
int wWindow = 800;
int hWindow = 600;
int rMouseButton = 0;
int lMouseButton = 0;
int rMouseMode = 0;
int lMouseMode = 0;
int clickPositionMouseX = 0;
int clickPositionMouseY = 0;
int lastPositionMouseX = 0;
int lastPositionMouseY = 0;

/* Simulator variables. */
double computationTimeStep = 0.0;
double computationTime = 0.0;
double timeLastFrame = 0.0;
unsigned int frame = 0;
uint32_t debugUtilities = DEBUG_DRAW_SPATIAL_TABLE | DEBUG_DRAW_OCCUPIED_BOXES;
uint32_t writeData = 0;
ceSpace* space;
float growthRate = 0.1f;
int interfaceYPosition;
ceVector2 cameraPosition;
float cameraZoom = 1.0f;
float bodyVertex = CE_PI / (int)(4 + 1.0f * BODY_VERTEX_INCREMENT);
int frameDivision = 0;
int pause = 0;
int step = 0;

float stadisticalLength = 0.0f;
float averageNeighborhood = 0.0f;
float pneumaticEffect = 0.0f;
unsigned int occupiedBoxes = 0;
float maxBodyDistance = 0;

/* Extra data for adding to the bodies. */
typedef struct bodyInfo
{
    int selected;
    int conjugation;
    float pneumaticValue;
} bodyInfo;

/* Seletion. */
ceBody** selected;
unsigned int selectedSize = 0;
int wSelectionSquare = 0;
int hSelectionSquare = 0;

void reset();
void freeResources();

int milliseconds = 0;

FILE* neighborhoodFile = NULL;
FILE* pneumaticEffectFile = NULL;
FILE* populationFile = NULL;
char* inputFileName = NULL;

//*************************************************************
//                         UTILITIES
//*************************************************************

void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text);
}

bodyInfo* createBodyInfo()
{
    bodyInfo* info = (bodyInfo*) malloc(sizeof(bodyInfo));
    info->selected = 0;
    info->conjugation = 0;
    info->pneumaticValue = 1.0f;
    return info;
}

void freeBodyInfo(bodyInfo* info)
{
    free(info);
}

ceBody* getBodyFromScreen(int x, int y)
{
    ceVector2 position;
    position.x = ((x - wWindow / 2) / cameraZoom) + cameraPosition.x;
    position.y = -((y - hWindow / 2) / cameraZoom) + cameraPosition.y;
    return ceGetBody(space, position);
}

ceBody** getBodiesFromScreen(int x, int y, int width, int height, unsigned int* size)
{
    ceVector2 position;
    position.x = ((x - wWindow / 2) / cameraZoom) + cameraPosition.x;
    position.y = -((y - hWindow / 2) / cameraZoom) + cameraPosition.y;
    ceVector2 dimension;
    dimension.x = width / cameraZoom;
    dimension.y = -height / cameraZoom;
    return ceGetBodies(space, size, position, dimension);
}

void moveBodyFromScreen(ceBody* body, int x, int y)
{
    ceVector2 displacement;
    displacement.x = x / cameraZoom;
    displacement.y = y / cameraZoom;
    ceMoveBody(body, displacement);;
}

void rotateSelectedBodies(float radians)
{
    for(unsigned int i = 0; i < selectedSize; i++)
    {
        ceRotateBody(selected[i], radians);
    }
}

void deselectBodies()
{
    if(selectedSize > 0)
    {
        for(unsigned int i = 0; i < selectedSize; i++)
        {
            bodyInfo* info = (bodyInfo*) selected[i]->data;
            info->selected = 0;
        }
        free(selected);
        selectedSize = 0;
    }
}

//*************************************************************
//                          EVENTS
//*************************************************************

void keyboardSpecialKeysEvent(int key, int x, int y)
{
    switch(key)
    {

    }
}

void keyboardNormalKeysEvent(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: //ESC
            freeResources();
            exit(0);
            break;

        case 'u':
            cePhysicsIterationsPerCombinedRing += 1;
            break;

        case 'y':
            cePhysicsIterationsPerCombinedRing -= 1;
            if(cePhysicsIterationsPerCombinedRing < 0)
                cePhysicsIterationsPerCombinedRing = 0;
            break;

        case 'm':
            growthRate += 0.01f;
            break;

        case 'n':
            growthRate -= 0.01f;
            if(growthRate < 0.0f)
                growthRate = 0.0f;
            break;

        case 'j':
            ceThrustRelation -= 0.01;
            if(ceThrustRelation < 0.0f)
                ceThrustRelation = 0.0f;
            break;

        case 'k':
            ceThrustRelation += 0.01;
            if(ceThrustRelation > 1.0f)
                ceThrustRelation = 1.0f;
            break;

        case 'z':
            cameraPosition.x = 0.0f;
            cameraPosition.y = 0.0f;
            cameraZoom = 1.0f;
            break;

        case 'p':
            pause = !pause;
            break;

        case ' ':
            pause = 0;
            step = 1;
            break;

        case 'x':
            reset();
            break;

        case 'a':
            rotateSelectedBodies(0.1f);
            break;

        case 's':
            rotateSelectedBodies(-0.1f);
            break;

        case 't':
            debugUtilities ^= DEBUG_DRAW_SPATIAL_TABLE;
            break;

        case 'b':
            debugUtilities ^= DEBUG_DRAW_OCCUPIED_BOXES;
            break;

        case 'l':
            debugUtilities ^= DEBUG_DRAW_LIMITS;
            break;

        case 'c':
            debugUtilities ^= DEBUG_DRAW_CONJUGATION;
            break;

        case 'd':
            debugUtilities ^= DEBUG_DRAW_SEGMENT_POINTS;
            break;

        case 'g':
            debugUtilities ^= DEBUG_DRAW_BODIES_PER_BOX;
            break;

        case 'r':
            debugUtilities ^= DEBUG_DRAW_RINGS;
            break;

        case 'h':
            debugUtilities ^= DEBUG_DRAW_ROTATIONS;
            break;

        case 'f':
            debugUtilities ^= DEBUG_DRAW_PNEUMATIC_EFFECT;
            break;

        case '0':
            if(selectedSize > 0)
            {
                ceDestroyBody(selected[0]);
                deselectBodies();
            }

        case '1':
            writeData = DEBUG_WRITE_NEIGHBORHOOD;
            neighborhoodFile = fopen("neighborhood.txt", "w");
            break;

        case '2':
            writeData = DEBUG_WRITE_PNEUMATIC_EFFECT;
            pneumaticEffectFile = fopen("pneumaticEffect.txt", "w");
            break;
    }
}

void mouseButtonEvent(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            lMouseButton = 1;
            clickPositionMouseX = x;
            clickPositionMouseY = y;
            lastPositionMouseX = x;
            lastPositionMouseY = y;
            lMouseMode = MOUSE_CAMERA_MOVE_MODE;
        }
        else
        {
            lMouseButton = 0;
        }
    }
    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            rMouseButton = 1;
            clickPositionMouseX = x;
            clickPositionMouseY = y;
            lastPositionMouseX = x;
            lastPositionMouseY = y;
            if(selectedSize)
            {
                ceBody* body = getBodyFromScreen(x, y);
                if(body)
                {
                    //bodyInfo* info = (bodyInfo*) body->data;
                    //if(info->selected)
                        //rMouseMode = MOUSE_BODIES_MOVE_MODE;
                    //else
                        rMouseMode = MOUSE_SELECTION_RECTANGLE_MODE;
                }
                else
                    rMouseMode = MOUSE_SELECTION_RECTANGLE_MODE;
            }
            else
                rMouseMode = MOUSE_SELECTION_RECTANGLE_MODE;
        }
        else
        {
            rMouseButton = 0;
            if(rMouseMode == MOUSE_SELECTION_RECTANGLE_MODE)
            {
                //deselectBodies();
                if(abs(wSelectionSquare) <= 3 && abs(hSelectionSquare) <= 3)
                {
                    ceBody* body = getBodyFromScreen(x, y);
                    if(body)
                    {
                        bodyInfo* info = (bodyInfo*) body->data;
                        if(info->selected == 1)
                        {
                            info->selected = 2;

                        }
                        else
                            info->selected = 1;
                        selected = (ceBody**) malloc(sizeof(ceBody*));
                        *selected = body;
                        selectedSize = 1;
                    }
                }
                else
                {
                    selected = getBodiesFromScreen(clickPositionMouseX, clickPositionMouseY, wSelectionSquare, hSelectionSquare, &selectedSize);
                    for(unsigned int i = 0; i < selectedSize; i++)
                    {
                        bodyInfo* info = (bodyInfo*) selected[i]->data;
                        info->selected = 1;
                    }
                }
                wSelectionSquare = 0;
                hSelectionSquare = 0;
            }
            rMouseMode = 0;
        }
    }
    else if(button == 3) //WHEEL
    {
        cameraZoom *= 1.1;
        bodyVertex = CE_PI / (int)(4 + cameraZoom * BODY_VERTEX_INCREMENT);
    }
    else if(button == 4) //WHEEL
    {
        cameraZoom /= 1.1;
        bodyVertex = CE_PI / (int)(4 + cameraZoom * BODY_VERTEX_INCREMENT);
    }

}

void mouseMotionEvent(int x, int y)
{
    int xDiff = x - lastPositionMouseX;
    int yDiff = y - lastPositionMouseY;
    lastPositionMouseX = x;
    lastPositionMouseY = y;

    if(lMouseButton)
    {
        if(lMouseMode == MOUSE_CAMERA_MOVE_MODE)
        {
            cameraPosition.x -= xDiff / cameraZoom;
            cameraPosition.y += yDiff / cameraZoom;
        }
    }
    else if(rMouseButton)
    {
        if(rMouseMode == MOUSE_BODIES_MOVE_MODE)
        {
            for(unsigned int i = 0; i < selectedSize; i++)
            {
                moveBodyFromScreen(selected[i], xDiff, -yDiff);
            }
        }
        else if(rMouseMode == MOUSE_SELECTION_RECTANGLE_MODE)
        {
            wSelectionSquare += xDiff;
            hSelectionSquare += yDiff;
        }
    }
}

void mousePassiveMotionEvent(int x, int y)
{

}

//*************************************************************
//                         INTERFACE
//*************************************************************

void showcomputationTime()
{
    static char stringToShow[64];
    if(!pause)
        computationTime += timeLastFrame;
    sprintf(stringToShow, "Computing time: %.2fsec", computationTime);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showFrame()
{
    static char stringToShow[64];
    if(!pause)
        frame++;
    sprintf(stringToShow, "Frame: %u", frame);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showFPS()
{
    static char fpsToShow[32] = "FPS: 0";
    static int framesCounter = 0;
    static double frameTimeAccumulator = 0;
    framesCounter++;
    frameTimeAccumulator += timeLastFrame;
    if(frameTimeAccumulator >= 1.0)
    {
        sprintf(fpsToShow, "FPS: %u", framesCounter);
        framesCounter = 0;
        frameTimeAccumulator -= 1.0;
    }
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, fpsToShow);
}

void showFrameTime()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Frame time: %.3fsec", timeLastFrame);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showCameraPosition()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Camera position: %.2f, %.2f", cameraPosition.x, cameraPosition.y);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showCameraZoom()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Camera zoom: %.3f", cameraZoom);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showSelected()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Selected: %u", selectedSize);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showPopulation()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Population: %u", space->populationSize);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showGrowthRate()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Growth rate: %.2f", growthRate);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showStadisticalBodyLength()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Stadistical body length: %.2fpx", stadisticalLength);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showDensity()
{
    static char stringToShow[64];
    float stadisticalBodyArea = CE_PI * ceBodySquareWidth * 0.25 + (stadisticalLength - ceBodyWidth) * ceBodyWidth;
    float areaRelation = (stadisticalBodyArea * space->populationSize) / (ceBoxLength * ceBoxLength * occupiedBoxes);
    sprintf(stringToShow, "Density: %.3f", areaRelation);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showRadialDensity()
{
    static char stringToShow[64];
    float radius = maxBodyDistance * 0.8;
    float bodyAreaAccumulator = 0;
    ceBody* body = space->firstBody;
    while(body != NULL)
    {
        float distance = ceGetLengthVector2(body->center);
        if(distance < radius)
        {
            bodyAreaAccumulator += CE_PI * ceBodySquareWidth * 0.25 + (body->length - ceBodyWidth) * ceBodyWidth;
        }
        body = body->next;
    }
    float circularArea = (radius * radius) * CE_PI;
    float areaRelation = (radius > 0) ? bodyAreaAccumulator / circularArea : 0.0f;
    sprintf(stringToShow, "Radial density: %.3f", areaRelation);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showAverageNeighborhood()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Avarage neighborhood: %.2f", averageNeighborhood);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showPneumaticEffect()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Pneumatic effect: %.2f", pneumaticEffect);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showBorderDistance()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Border distance: %.0fpx", maxBodyDistance);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showMaxBodiesPerBox()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Max bodies per box: %u", ceMaxBodiesPerBox);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showCombinedRings()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Combined rings: %u", ceCombinedRing);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showIterationsPerRing()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Iterations per ring: %u", cePhysicsIterationsPerCombinedRing);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showThrustRelation()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Thrust relation: %.2f", ceThrustRelation);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showRingsNumber()
{
    static char stringToShow[64];
    sprintf(stringToShow, "Rings number: %u", space->ringsNumber);
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showPress1()
{
    static char stringToShow[64] = "Press 1 -> neighborhood.txt";
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void showPress2()
{
    static char stringToShow[64] = "Press 2 -> pneumaticEffect.txt";;
    interfaceYPosition += 20;
    drawText(10, interfaceYPosition, stringToShow);
}

void drawSelectionRectangle()
{
    glColor4f(0.0f, 0.4f, 1.0f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(clickPositionMouseX, clickPositionMouseY);
        glVertex2f(clickPositionMouseX, clickPositionMouseY + hSelectionSquare);
        glVertex2f(clickPositionMouseX + wSelectionSquare, clickPositionMouseY + hSelectionSquare);
        glVertex2f(clickPositionMouseX + wSelectionSquare, clickPositionMouseY);
    glEnd();

    glLineWidth(1.0f);
    glColor3f(0.0f, 0.4f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(clickPositionMouseX, clickPositionMouseY);
        glVertex2f(clickPositionMouseX, clickPositionMouseY + hSelectionSquare);
        glVertex2f(clickPositionMouseX + wSelectionSquare, clickPositionMouseY + hSelectionSquare);
        glVertex2f(clickPositionMouseX + wSelectionSquare, clickPositionMouseY);
    glEnd();
}

//*************************************************************
//                        MAIN PROGRAM
//*************************************************************

void debugDrawSpatialTable()
{
    if(debugUtilities & DEBUG_DRAW_SPATIAL_TABLE)
    {
        int lineWidth = (cameraZoom > 1.0f) ? 1.0f : cameraZoom;
        int middleTableLength = space->middleTableLength;
        glLineWidth(lineWidth);
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_LINES);
            for(float x = -middleTableLength; x <= middleTableLength; x += ceBoxLength)
            {
                glVertex2f(x, -middleTableLength);
                glVertex2f(x, middleTableLength);
            }

            for(float y = -middleTableLength; y <= middleTableLength; y += ceBoxLength)
            {
                glVertex2f(-middleTableLength, y);
                glVertex2f(middleTableLength, y);
            }
        glEnd();
    }
}

void debugDrawBodiesPerBox()
{
    if(debugUtilities & DEBUG_DRAW_BODIES_PER_BOX)
    {
        int middleTableLength = space->middleTableLength;
        for(int i = 0; i < space->tableBoxLength; i++)
            for(int j = 0; j < space->tableBoxLength; j++)
            {
                int size = space->spatialTable[i * space->tableBoxLength + j].size;
                if(size)
                {
                    if(ceMaxBodiesPerBox - size <= 3)
                        glColor3f(1.0f, 0.0f, 0.0f);
                    else if((size <= ceMaxBodiesPerBox / 4) / _CE_ACCURACY_EDGE)
                        glColor3f(1.0f, 0.0f, 1.0f);
                    else
                        glColor3f(0.0f, 0.0f, 1.0f);
                    float x = j * ceBoxLength - middleTableLength;
                    float y = i * ceBoxLength - middleTableLength;
                    static char number[6];
                    sprintf(number, "%u", size);
                    drawText(x + ceBoxLength / 4, y + ceBoxLength / 4, number);
                }
            }
    }
}

void debugDrawLimits()
{
    if(debugUtilities & DEBUG_DRAW_LIMITS)
    {
        int middleTableLength = space->middleTableLength;
        glColor4f(0.8f, 0.3f, 1.0f, 0.5f);
        unsigned int size = 0;
        int* limits = _ceDebugFindLimits(space, &size);
        glBegin(GL_QUADS);
        for(int i = 0; i < size; i += 2)
        {
            float x = limits[i] * ceBoxLength - middleTableLength;
            float y = limits[i + 1] * ceBoxLength - middleTableLength;
            glVertex2f(x, y);
            glVertex2f(x, y + ceBoxLength);
            glVertex2f(x + ceBoxLength, y + ceBoxLength);
            glVertex2f(x + ceBoxLength, y);
        }
        glEnd();
        free(limits);
    }
}

void debugDrawOccupiedBoxes()
{
    if(debugUtilities & DEBUG_DRAW_OCCUPIED_BOXES)
    {
        occupiedBoxes = 0;
        int middleTableLength = space->middleTableLength;
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_QUADS);
        for(int i = 0; i < space->tableBoxLength; i++)
            for(int j = 0; j < space->tableBoxLength; j++)
            {
                if(space->spatialTable[i * space->tableBoxLength + j].size)
                {
                    float x = j * ceBoxLength - middleTableLength;
                    float y = i * ceBoxLength - middleTableLength;
                    glVertex2f(x, y);
                    glVertex2f(x, y + ceBoxLength);
                    glVertex2f(x + ceBoxLength, y + ceBoxLength);
                    glVertex2f(x + ceBoxLength, y);
                    if(ceMaxBodiesPerBox - space->spatialTable[i * space->tableBoxLength + j].size > 3)
                        occupiedBoxes++;
                }
            }
        glEnd();
    }
}

void debugDrawSegmentPoints()
{
    if(debugUtilities & DEBUG_DRAW_SEGMENT_POINTS)
    {
        glPointSize(3.0f + (cameraZoom / 5));
        glColor3f(1.0f, 0.0f, 0.0f);
        ceBody* body = space->firstBody;
        glBegin(GL_POINTS);
        while(body != NULL)
        {
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(body->p1.x, body->p1.y);
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex2f(body->p2.x, body->p2.y);

            body = body->next;
        }
        glEnd();
    }
}

void chooseBodyColor(ceBody* body)
{
    bodyInfo* info = (bodyInfo*) body->data;
    if(info->selected == 1)
        glColor4f(0.0f, 1.0f, 1.0f, 0.7f);
    else if(info->selected > 1)
        glColor4f(-0.2f + (((float)info->selected) / 10), 0.0f, 1.2f - (((float)info->selected) / 10), 1.0f);
    else if(debugUtilities & DEBUG_DRAW_CONJUGATION && info->conjugation)
        glColor4f(1.0f, 0.5f, 1.0f, 0.7f);
    else if(debugUtilities & DEBUG_DRAW_ROTATIONS)
        glColor4f(0.0f, 0.1f + 0.9f * fabs(sin(body->rotation)), 0.1f + 0.9f * fabs(cos(body->rotation)), 1.0f);
    else if(debugUtilities & DEBUG_DRAW_PNEUMATIC_EFFECT)
    {
        if(info->pneumaticValue == -1)
            glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        else
            glColor4f(1.0f - 1.0f * info->pneumaticValue, 1.0f * info->pneumaticValue, 0.0f, 1.0f);
    }
    else if(debugUtilities & DEBUG_DRAW_RINGS)
    {
        if(body->pressure == _CE_PRESSURE_CLEAR)
            glColor4f(0.15f, 0.15f, 0.15f, 0.7f);
        else if(body->pressure == 0)
            glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
        else if((body->pressure % 5) == 1)
            glColor4f(1.0f, 1.0f, 0.5f, 1.0f);
        else if((body->pressure % 5) == 2)
            glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        else if((body->pressure % 5) == 3)
            glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        else if((body->pressure % 5) == 4)
            glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
        else
            glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    }
    else
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
}

void renderBody(ceBody* body)
{
    glPushMatrix();

    glLineWidth(1.0f);
    glTranslatef(body->center.x, body->center.y, 0.0f);
    glRotatef(body->rotation * CE_RADIAN, 0.0f, 0.0f, 1.0f);
    float mWidth = ceBodyWidth * 0.5;
    float mRectLength = body->length * 0.5f - mWidth;

    chooseBodyColor(body);
    /* Shape */
    glBegin(GL_POLYGON);
        glVertex2f(-mRectLength, mWidth);
        for(float a = 0.0; a < CE_PI; a += bodyVertex)
            glVertex2f(-(mRectLength + sin(a) * mWidth), mWidth * cos(a));
        glVertex2f(-mRectLength, -mWidth);
        glVertex2f(mRectLength, -mWidth);
        for(float a = 0.0; a < CE_PI; a += bodyVertex)
            glVertex2f(mRectLength + sin(a) * mWidth, -mWidth * cos(a));
        glVertex2f(mRectLength, mWidth);
    glEnd();
    glColor3f(0.0f, 0.3f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-mRectLength, mWidth);
        for(float a = 0.0; a < CE_PI; a += bodyVertex)
            glVertex2f(-(mRectLength + sin(a) * mWidth), mWidth * cos(a));
        glVertex2f(-mRectLength, -mWidth);
        glVertex2f(mRectLength, -mWidth);
        for(float a = 0.0; a < CE_PI; a += bodyVertex)
            glVertex2f(mRectLength + sin(a) * mWidth, -mWidth * cos(a));
        glVertex2f(mRectLength, mWidth);
    glEnd();

    glPopMatrix();
}

void render()
{
    /***** Clear canvas. *****/
    glClear(GL_COLOR_BUFFER_BIT);

    /***** World projection. *****/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-wWindow / 2, wWindow / 2, -hWindow / 2, hWindow / 2, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(cameraZoom, cameraZoom, 0.0f);
    glTranslatef(-cameraPosition.x, -cameraPosition.y, 0.0f);

    /* Draw debug utilities */
    debugDrawOccupiedBoxes();
    debugDrawLimits();
    debugDrawSpatialTable();

    /* Draw cells. */
    ceBody* aux = space->firstBody;
    while(aux != NULL)
    {
        /* Draw body */
        renderBody(aux);

        /* Next body */
        aux = aux->next;
    }

    /* Draw debug utilities */
    debugDrawSegmentPoints();
    debugDrawBodiesPerBox();

    /***** Interface projection. *****/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, wWindow, hWindow, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* items. */
    interfaceYPosition = 0;
    glColor3f(0.0f, 0.0f, 0.0f);

    showcomputationTime();
    showFrame();
    showFPS();
    showFrameTime();
    interfaceYPosition += 20;

    showCameraPosition();
    showCameraZoom();
    showSelected();
    interfaceYPosition += 20;

    showPopulation();
    showGrowthRate();
    showStadisticalBodyLength();
    showDensity();
    showRadialDensity();
    showAverageNeighborhood();
    showBorderDistance();
    showPneumaticEffect();

    interfaceYPosition += 20;

    showMaxBodiesPerBox();
    showCombinedRings();
    showIterationsPerRing();
    showThrustRelation();
    showRingsNumber();

    interfaceYPosition += 100;

    showPress1();
    showPress2();

    drawSelectionRectangle();

    /***** Render to screen. *****/
    glutSwapBuffers();
}

void load()
{
    /* Init CellEngine system. */
    ceInit(4, 1, 40.0f, 20.0f, 10.0f);

    /* Create a space. */
    space = ceCreateSpace();

    populationFile = fopen("population.txt", "w");

    if(inputFileName != NULL)
    {
        FILE* inputFile = fopen(inputFileName, "r");
        char lineBuffer[256];
        while(fgets(lineBuffer, 256, inputFile))
        {
            int x;
            int y;
            float radians;
            int length;
            sscanf(lineBuffer,"%i,%i,%f,%i", &x, &y, &radians, &length);
            printf("%i,%i,%.6f,%i\n", x, y, radians, length);
            ceBody* body = ceCreateBody(space, length, ceGetVector2(x, y), radians);
            ceSetData(body, createBodyInfo());
        }
        fclose(inputFile);
    }
    else
    {
        ceBody* body1 = ceCreateBody(space, 20.0f, ceGetVector2(0.0f, 0.0f), 0.0f);
        //ceBody* body2 = ceCreateBody(space, 20.0f, ceGetVector2(10.0f, 0.0f), 90.0f);
        //ceBody* body3 = ceCreateBody(space, 20.0f, ceGetVector2(0.0f, 10.0f), 92.0f);
        ceSetData(body1, createBodyInfo());
        //ceSetData(body2, createBodyInfo());
        //ceSetData(body3, createBodyInfo());
    }

    /* Create a groud of cells in the space. */
    /*int size = 200;
    int side = 300;
    for(int i = 0; i < size; i++)
    {
        float randx = -(side / 2) + (((float)rand()) / RAND_MAX) * side;
        float randy = -(side / 2) + (((float)rand()) / RAND_MAX) * side;
        float randr = (((float)rand()) / RAND_MAX) * 3.14;
        ceBody* body1 = ceCreateBody(space, 20.0f, ceGetVector2(randx, randy), randr);
        ceSetData(body1, createBodyInfo());
    }*/
}

void update()
{
    if(!pause)
    {
        /* Get time. */
        milliseconds = glutGet(GLUT_ELAPSED_TIME);

        stadisticalLength = 0.0f;
        averageNeighborhood = 0.0f;
        pneumaticEffect = 0.0f;
        int pneumaticEffectInvalidBodies = 0;

        /* Clear previous conjugations */
        ceBody* body = space->firstBody;
        while(body != NULL)
        {
            bodyInfo* info = (bodyInfo*) body->data;
            info->conjugation = 0;

            body = body->next;
        }

        /* Iterate the cells. */
        unsigned int populationRemaining = space->populationSize;
        body = space->firstBody;
        while(body != NULL && populationRemaining != 0)
        {
            /* Conjugation. */
            bodyInfo* info = (bodyInfo*) body->data;
            float pneumatic = 0.0f;
            info->pneumaticValue = 0.0f;
            unsigned int size;
            ceBody** list = ceGetNearBodies(body, 5.0f, &size);
            if(size >= 2)
            {
                for(int i = 0; i < size; i++)
                {
                    ceBody* nearBody = list[i];
                    pneumatic += fabs(ceGetDotProductVector2(body->unitary, nearBody->unitary));
                    if(debugUtilities & DEBUG_DRAW_CONJUGATION && info->selected)
                    {
                        bodyInfo* infoAux = (bodyInfo*) nearBody->data;
                        infoAux->conjugation = 1;
                    }
                }
                if(size != 0)
                pneumatic /= size;
                info->pneumaticValue = pneumatic;
            }
            else
            {
                pneumaticEffectInvalidBodies++;
                info->pneumaticValue = -1;
            }
            free(list);


            static char value[32];
            averageNeighborhood += size;
            if(writeData == DEBUG_WRITE_NEIGHBORHOOD)
            {
                sprintf(value, "%u\n", size);
                fputs(value, neighborhoodFile);
            }
            pneumaticEffect += pneumatic;
            if(writeData == DEBUG_WRITE_PNEUMATIC_EFFECT)
            {
                sprintf(value, "%.5f\n", pneumatic);
                fputs(value, pneumaticEffectFile);
            }

            if(((int)(computationTime)) / 10.0 >= computationTimeStep)
            {
                sprintf(value, "%u\n", space->populationSize);
                fputs(value, populationFile);
                computationTimeStep++;
            }

            /* Cell body growth. */
            ceGrowBody(body, growthRate);

            /* Cell body division. */
            if(body->length >= 40.0f)
            {
                float alpha = (CE_PI / 180) * (((float)rand()) / RAND_MAX) - (CE_PI / 360);
                float lenghtVariation = 0.4f + 0.2f * (((float)rand()) / RAND_MAX);
                ceBody* child = ceDivideBody(body, alpha, lenghtVariation);

                ceSetData(child, createBodyInfo());
                bodyInfo* infoParent = (bodyInfo*) body->data;
                bodyInfo* infoChild = (bodyInfo*) child->data;
                if(infoParent->selected < 2)
                    infoChild->selected = infoParent->selected;
                else
                    infoChild->selected = infoParent->selected + 1;
                infoChild->pneumaticValue = infoParent->pneumaticValue;
            }
            stadisticalLength += body->length;

            /* Next body. */
            body = body->next;
            populationRemaining--;
        }

        stadisticalLength /= space->populationSize;
        averageNeighborhood /= space->populationSize;
        pneumaticEffect /= space->populationSize - pneumaticEffectInvalidBodies;

        maxBodyDistance = 0;
        body = space->firstBody;
        while(body != NULL)
        {
            float distance = ceGetLengthVector2(body->center);
            if(distance > maxBodyDistance)
                maxBodyDistance = distance;
            body = body->next;
        }

        /* Physics. */
        ceStep(space);
    }

    /* Draw step */
    render();

    if(!pause)
    {
        /* Frame time. */
        timeLastFrame = ((double)(glutGet(GLUT_ELAPSED_TIME) - milliseconds)) / 1000;

        /* Frame control */
        if(step)
        {
            pause = 1;
            step = 0;
        }

        if(computationTime > 1001.0)
        {
            fclose(populationFile);
            computationTime = 0;
        }

        if(writeData != 0)
        {
            if(writeData == DEBUG_WRITE_NEIGHBORHOOD)
                fclose(neighborhoodFile);
            else if(writeData == DEBUG_WRITE_PNEUMATIC_EFFECT)
                fclose(pneumaticEffectFile);
            writeData = 0;
        }
    }
}

void initGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void reshape(int width, int height)
{
    wWindow = width;
    hWindow = height;
    glViewport(0, 0, wWindow, hWindow);
}

void display()
{
}

void reset()
{
    freeResources();
    computationTime = 0.0;
    frame = 0;
    srand(glutGet(GLUT_ELAPSED_TIME));
    load();
}

void freeResources()
{
    /* Deselect bodies. */
    deselectBodies();

    /* Free data associated to bodies. */
    ceBody* body = space->firstBody;
    while(body != NULL)
    {
        freeBodyInfo((bodyInfo*) body->data);
        body = body->next;
    }

    /* Free space. Also, It frees the bodies associated. */
    ceDestroySpace(space);
}

int main(int argc, char** argv)
{
    /* Init GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(wWindow, hWindow);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - wWindow) / 2,  (glutGet(GLUT_SCREEN_HEIGHT) - hWindow) / 2);
    glutCreateWindow("CellEngine 0.1.7.1");
    glutIdleFunc(update);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboardSpecialKeysEvent);
    glutKeyboardFunc(keyboardNormalKeysEvent);
    glutMouseFunc(mouseButtonEvent);
    glutMotionFunc(mouseMotionEvent);
    glutPassiveMotionFunc(mousePassiveMotionEvent);

    if(argc > 1)
        inputFileName = argv[1];

    /* Init debugger*/
    initGL();
    cameraPosition = ceGetVector2(0, 0);
    srand(glutGet(GLUT_ELAPSED_TIME));
    load();

    /* Loop */
    glutMainLoop();

    /* Delete resources */
    freeResources();
    return 0;
}