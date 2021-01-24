#include <gl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(a) ((char*)NULL + (a))

using namespace std;

typedef struct OBJ2_Vertex
{
    float X;
    float Y;
    float Z;
}OBJ2_Vertex;

typedef struct OBJ2_Color
{
    float R;
    float G;
    float B;
}OBJ2_Color;

typedef struct OBJ2_VBO
{
    vector<float> coordVertices;
    vector<float> coordTex;
    vector<float> normals;
    vector<float> colors;
    vector<GLuint> textures;
    vector<float> attribNumtex;
    vector<float> posInstances;
    vector<float> inclinInstances;
    //vector<int> list_faces_change_tex;
    GLuint bufferVRAM;
    GLuint VAO;
    //GLuint indexAttributs;
    int nbVertices;
}OBJ2_VBO;

typedef struct OBJ2_Camera
{
    float pos[3];
    float target[3];
    float upVector[3];
    float ratio;
    double fovy;
    float near;
    float far;
    double angleH;
    double angleV;
}OBJ2_Camera;

bool OBJ2_LoadOBJ(string path,OBJ2_VBO *vbo,const float X=0,const float Y=0,const float Z=0);
void OBJ2_LoadMTL(FILE *fichier,string nameMaterial,OBJ2_Color *color,GLuint *tex,string path,float *ScaleX,float *ScaleY);
void OBJ2_DrawVBO(OBJ2_VBO *vbo,const GLuint shader=0);
void OBJ2_InitInstance(vector<OBJ2_Vertex> &positions,const GLuint shader);
void OBJ2_DrawVBO_Instanced(OBJ2_VBO *vbo,vector<OBJ2_Vertex> &positions,const GLuint shader=0);
//void OBJ2_MergeVBO(OBJ2_VBO *parent,OBJ2_VBO *child,const bool mergeTex,const float X=0,const float Y=0,const float Z=0);
