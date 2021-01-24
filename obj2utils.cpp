#include <glew.h>
#include <gl.h>
#include <glu.h>
#include <math.h>
#include <sdl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include "obj2utils.h"
#include "../sdlglutils/sdlglutils.h"

using namespace std;

bool OBJ2_LoadOBJ(string path,OBJ2_VBO *vbo,const float X,const float Y,const float Z)
{
    FILE *fichierOBJ=fopen(path.c_str(),"rb");

    if(fichierOBJ==NULL)
        return false;

    string ligneLue;
    bool retour;
    vector<OBJ2_Vertex> coordVertices;
    vector<OBJ2_Vertex> coordTex;
    vector<OBJ2_Vertex> normals;
    OBJ2_Vertex v,ct,n;
    int nbFaces=0;

    while(1)
    {
        retour=lectureLigneFichier(fichierOBJ,ligneLue);
        if(!retour)
            break;

        //if(strstr(ligneLue.c_str(),"v ")==ligneLue.c_str())
        if(ligneLue.find("v ")==0)
        {
            sscanf(ligneLue.c_str(),"v %f %f %f\n",&v.X,&v.Y,&v.Z);

            v.X+=X;
            v.Y+=Y;
            v.Z+=Z;

            coordVertices.push_back(v);
        }
    }

    rewind(fichierOBJ);

    while(1)
    {
        retour=lectureLigneFichier(fichierOBJ,ligneLue);
        if(!retour)
            break;

        //if(strstr(ligneLue.c_str(),"vt ")==ligneLue.c_str())
        if(ligneLue.find("vt ")==0)
        {
            sscanf(ligneLue.c_str(),"vt %f %f\n",&ct.X,&ct.Y);

            coordTex.push_back(ct);
        }
    }

    rewind(fichierOBJ);

    while(1)
    {
        retour=lectureLigneFichier(fichierOBJ,ligneLue);
        if(!retour)
            break;

        //if(strstr(ligneLue.c_str(),"vn ")==ligneLue.c_str())
        if(ligneLue.find("vn ")==0)
        {
            sscanf(ligneLue.c_str(),"vn %f %f %f\n",&n.X,&n.Y,&n.Z);

            normals.push_back(n);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////

    /*char pathMTL[path.length()];

    strcpy(pathMTL,path.c_str());*/
    string pathMTL=path;

   /* char *adressePoint=strrchr(pathMTL,'.');
    *(adressePoint+1)='\0';

    strcat(pathMTL,"mtl");*/

    pathMTL.erase(pathMTL.rfind(".obj"));
    pathMTL+=".mtl";

    FILE *fichierMTL=fopen(pathMTL.c_str(),"rb");

    GLuint tex;
    OBJ2_Color couleur;
    //OBJ2_VBO vbo2;

    rewind(fichierOBJ);

    while(1)
    {
        lectureLigneFichier(fichierOBJ,ligneLue);
        //if(strstr(ligneLue.c_str(),"o ")==ligneLue.c_str())
        if(ligneLue.find("o ")==0)
            break;
    }

    /*char path2[path.length()];

    strcpy(path2,path.c_str());

    char *adresseSlash=strrchr(path2,'/');

    *(adresseSlash+1)='\0';*/
    string path2=path;

    path2.erase(path2.rfind("/")+1);

    float scaleX,scaleY;
    int numtex=-1;

    while(1)
    {
        retour=lectureLigneFichier(fichierOBJ,ligneLue);
        if(!retour)
        {
            vbo->nbVertices=vbo->coordVertices.size()/3;

            glGenVertexArrays(1,&vbo->VAO);
            glGenBuffers(1,&vbo->bufferVRAM);

            glBindVertexArray(vbo->VAO);
            glBindBuffer(GL_ARRAY_BUFFER,vbo->bufferVRAM);      //Bindage vers espace mémoire VRAM

            glBufferData(GL_ARRAY_BUFFER,vbo->nbVertices*3*sizeof(float)+vbo->coordTex.size()*sizeof(float)+vbo->normals.size()*sizeof(float)+vbo->colors.size()*sizeof(float)+vbo->attribNumtex.size()*sizeof(float),NULL,GL_STREAM_DRAW);

            glBufferSubData(GL_ARRAY_BUFFER,0,vbo->nbVertices*3*sizeof(float),&vbo->coordVertices[0]);      //Mise en mémoire coordonnées vertices

            if(vbo->coordTex.size()!=0)
                glBufferSubData(GL_ARRAY_BUFFER,vbo->nbVertices*3*sizeof(float),vbo->coordTex.size()*sizeof(float),&vbo->coordTex[0]);      //Mise en mémoire coordonnées texture

            if(vbo->normals.size()!=0)
                glBufferSubData(GL_ARRAY_BUFFER,vbo->nbVertices*3*sizeof(float)+vbo->coordTex.size()*sizeof(float),vbo->normals.size()*sizeof(float),&vbo->normals[0]);      //Mise en mémoire normales

            if(vbo->colors.size()!=0)
                glBufferSubData(GL_ARRAY_BUFFER,vbo->nbVertices*3*sizeof(float)+vbo->coordTex.size()*sizeof(float)+vbo->normals.size()*sizeof(float),vbo->colors.size()*sizeof(float),&vbo->colors[0]);      //Mise en mémoire couleurs

            if(vbo->attribNumtex.size()!=0)
                glBufferSubData(GL_ARRAY_BUFFER,vbo->nbVertices*3*sizeof(float)+vbo->coordTex.size()*sizeof(float)+vbo->normals.size()*sizeof(float)+vbo->colors.size()*sizeof(float),vbo->attribNumtex.size()*sizeof(float),&vbo->attribNumtex[0]);      //Mise en mémoire attributs

            //vbo->list_faces_change_tex.push_back(nbFaces);

            //vbos.push_back(vbo2);

            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)(vbo->coordVertices.size()*sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)(vbo->coordVertices.size()*sizeof(float)+vbo->coordTex.size()*sizeof(float)));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)(vbo->coordVertices.size()*sizeof(float)+vbo->coordTex.size()*sizeof(float)+vbo->normals.size()*sizeof(float)));
            glEnableVertexAttribArray(3);

            glVertexAttribPointer(4,1,GL_FLOAT,GL_FALSE,sizeof(float),(void*)(vbo->coordVertices.size()*sizeof(float)+vbo->coordTex.size()*sizeof(float)+vbo->normals.size()*sizeof(float)+vbo->colors.size()*sizeof(float)));
            glEnableVertexAttribArray(4);

            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindVertexArray(0);

            /////////////////////////////////////////////

            break;
        }

        //if(strstr(ligneLue.c_str(),"usemtl ")==ligneLue.c_str())
        if(ligneLue.find("usemtl ")==0)
        {
            char materiel[ligneLue.length()];

            sscanf(ligneLue.c_str(),"usemtl %s\n",materiel);

            OBJ2_LoadMTL(fichierMTL,string(materiel),&couleur,&tex,path2,&scaleX,&scaleY);

            /*vbo->colors.push_back(couleur.R);
            vbo->colors.push_back(couleur.G);
            vbo->colors.push_back(couleur.B);*/

            vbo->textures.push_back(tex);

            numtex++;

            //vbo->list_faces_change_tex.push_back(nbFaces);
        }

        else
        //if(strstr(ligneLue.c_str(),"f ")==ligneLue.c_str())
        if(ligneLue.find("f ")==0)
        {
            int numCoordVertex[3],numCoordTex[3],numNormale[3];

            if(nbOccurrences(ligneLue,'/')==0)
            {
                sscanf(ligneLue.c_str(),"f %d %d %d\n",&numCoordVertex[0],&numCoordVertex[1],&numCoordVertex[2]);

                numCoordVertex[0]--;
                numCoordVertex[1]--;
                numCoordVertex[2]--;

                vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].X);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Y);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Z);

                vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].X);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Y);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Z);

                vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].X);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Y);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Z);

                vbo->coordTex.push_back(0.0);
                vbo->coordTex.push_back(0.0);

                vbo->coordTex.push_back(0.0);
                vbo->coordTex.push_back(0.0);

                vbo->coordTex.push_back(0.0);
                vbo->coordTex.push_back(0.0);

                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);

                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);

                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);

                vbo->colors.push_back(couleur.R);
                vbo->colors.push_back(couleur.G);
                vbo->colors.push_back(couleur.B);

                vbo->colors.push_back(couleur.R);
                vbo->colors.push_back(couleur.G);
                vbo->colors.push_back(couleur.B);

                vbo->colors.push_back(couleur.R);
                vbo->colors.push_back(couleur.G);
                vbo->colors.push_back(couleur.B);

                if(tex!=0)
                {
                    vbo->attribNumtex.push_back(numtex);
                    vbo->attribNumtex.push_back(numtex);
                    vbo->attribNumtex.push_back(numtex);
                }

                else
                {
                    vbo->attribNumtex.push_back(-1);
                    vbo->attribNumtex.push_back(-1);
                    vbo->attribNumtex.push_back(-1);
                }
            }

            else
            if(nbOccurrences(ligneLue,'/')==3)
            {
                sscanf(ligneLue.c_str(),"f %d/%d %d/%d %d/%d\n",&numCoordVertex[0],&numCoordTex[0],&numCoordVertex[1],&numCoordTex[1],&numCoordVertex[2],&numCoordTex[2]);

                numCoordVertex[0]--;
                numCoordVertex[1]--;
                numCoordVertex[2]--;

                numCoordTex[0]--;
                numCoordTex[1]--;
                numCoordTex[2]--;

                vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].X);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Y);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Z);

                vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].X);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Y);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Z);

                vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].X);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Y);
                vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Z);

                vbo->coordTex.push_back(coordTex[numCoordTex[0]].X*scaleX);
                vbo->coordTex.push_back(coordTex[numCoordTex[0]].Y*scaleY);

                vbo->coordTex.push_back(coordTex[numCoordTex[1]].X*scaleX);
                vbo->coordTex.push_back(coordTex[numCoordTex[1]].Y*scaleY);

                vbo->coordTex.push_back(coordTex[numCoordTex[2]].X*scaleX);
                vbo->coordTex.push_back(coordTex[numCoordTex[2]].Y*scaleY);

                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);

                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);

                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);
                vbo->normals.push_back(0.0);

                vbo->colors.push_back(couleur.R);
                vbo->colors.push_back(couleur.G);
                vbo->colors.push_back(couleur.B);

                vbo->colors.push_back(couleur.R);
                vbo->colors.push_back(couleur.G);
                vbo->colors.push_back(couleur.B);

                vbo->colors.push_back(couleur.R);
                vbo->colors.push_back(couleur.G);
                vbo->colors.push_back(couleur.B);

                if(tex!=0)
                {
                    vbo->attribNumtex.push_back(numtex);
                    vbo->attribNumtex.push_back(numtex);
                    vbo->attribNumtex.push_back(numtex);
                }

                else
                {
                    vbo->attribNumtex.push_back(-1);
                    vbo->attribNumtex.push_back(-1);
                    vbo->attribNumtex.push_back(-1);
                }
            }

            else
            if(nbOccurrences(ligneLue,'/')==6)
            {
                if(ligneLue.find("//")!=-1)
                {
                    sscanf(ligneLue.c_str(),"f %d//%d %d//%d %d//%d\n",&numCoordVertex[0],&numNormale[0],&numCoordVertex[1],&numNormale[1],&numCoordVertex[2],&numNormale[2]);

                    numCoordVertex[0]--;
                    numCoordVertex[1]--;
                    numCoordVertex[2]--;

                    numNormale[0]--;
                    numNormale[1]--;
                    numNormale[2]--;

                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].X);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Y);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Z);

                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].X);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Y);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Z);

                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].X);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Y);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Z);

                    vbo->coordTex.push_back(0.0);
                    vbo->coordTex.push_back(0.0);

                    vbo->coordTex.push_back(0.0);
                    vbo->coordTex.push_back(0.0);

                    vbo->coordTex.push_back(0.0);
                    vbo->coordTex.push_back(0.0);

                    vbo->normals.push_back(normals[numNormale[0]].X);
                    vbo->normals.push_back(normals[numNormale[0]].Y);
                    vbo->normals.push_back(normals[numNormale[0]].Z);

                    vbo->normals.push_back(normals[numNormale[1]].X);
                    vbo->normals.push_back(normals[numNormale[1]].Y);
                    vbo->normals.push_back(normals[numNormale[1]].Z);

                    vbo->normals.push_back(normals[numNormale[2]].X);
                    vbo->normals.push_back(normals[numNormale[2]].Y);
                    vbo->normals.push_back(normals[numNormale[2]].Z);

                    vbo->colors.push_back(couleur.R);
                    vbo->colors.push_back(couleur.G);
                    vbo->colors.push_back(couleur.B);

                    vbo->colors.push_back(couleur.R);
                    vbo->colors.push_back(couleur.G);
                    vbo->colors.push_back(couleur.B);

                    vbo->colors.push_back(couleur.R);
                    vbo->colors.push_back(couleur.G);
                    vbo->colors.push_back(couleur.B);

                    if(tex!=0)
                    {
                        vbo->attribNumtex.push_back(numtex);
                        vbo->attribNumtex.push_back(numtex);
                        vbo->attribNumtex.push_back(numtex);
                    }

                    else
                    {
                        vbo->attribNumtex.push_back(-1);
                        vbo->attribNumtex.push_back(-1);
                        vbo->attribNumtex.push_back(-1);
                    }
                }

                else
                {
                    sscanf(ligneLue.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d\n",&numCoordVertex[0],&numCoordTex[0],&numNormale[0],&numCoordVertex[1],&numCoordTex[1],&numNormale[1],&numCoordVertex[2],&numCoordTex[2],&numNormale[2]);

                    numCoordVertex[0]--;
                    numCoordVertex[1]--;
                    numCoordVertex[2]--;

                    numCoordTex[0]--;
                    numCoordTex[1]--;
                    numCoordTex[2]--;

                    numNormale[0]--;
                    numNormale[1]--;
                    numNormale[2]--;

                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].X);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Y);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[0]].Z);

                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].X);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Y);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[1]].Z);

                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].X);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Y);
                    vbo->coordVertices.push_back(coordVertices[numCoordVertex[2]].Z);

                    vbo->coordTex.push_back(coordTex[numCoordTex[0]].X*scaleX);
                    vbo->coordTex.push_back(coordTex[numCoordTex[0]].Y*scaleY);

                    vbo->coordTex.push_back(coordTex[numCoordTex[1]].X*scaleX);
                    vbo->coordTex.push_back(coordTex[numCoordTex[1]].Y*scaleY);

                    vbo->coordTex.push_back(coordTex[numCoordTex[2]].X*scaleX);
                    vbo->coordTex.push_back(coordTex[numCoordTex[2]].Y*scaleY);

                    vbo->normals.push_back(normals[numNormale[0]].X);
                    vbo->normals.push_back(normals[numNormale[0]].Y);
                    vbo->normals.push_back(normals[numNormale[0]].Z);

                    vbo->normals.push_back(normals[numNormale[1]].X);
                    vbo->normals.push_back(normals[numNormale[1]].Y);
                    vbo->normals.push_back(normals[numNormale[1]].Z);

                    vbo->normals.push_back(normals[numNormale[2]].X);
                    vbo->normals.push_back(normals[numNormale[2]].Y);
                    vbo->normals.push_back(normals[numNormale[2]].Z);

                    vbo->colors.push_back(couleur.R);
                    vbo->colors.push_back(couleur.G);
                    vbo->colors.push_back(couleur.B);

                    vbo->colors.push_back(couleur.R);
                    vbo->colors.push_back(couleur.G);
                    vbo->colors.push_back(couleur.B);

                    vbo->colors.push_back(couleur.R);
                    vbo->colors.push_back(couleur.G);
                    vbo->colors.push_back(couleur.B);

                    if(tex!=0)
                    {
                        vbo->attribNumtex.push_back(numtex);
                        vbo->attribNumtex.push_back(numtex);
                        vbo->attribNumtex.push_back(numtex);
                    }

                    else
                    {
                        vbo->attribNumtex.push_back(-1);
                        vbo->attribNumtex.push_back(-1);
                        vbo->attribNumtex.push_back(-1);
                    }
                }
            }

            nbFaces++;
        }
    }

    fclose(fichierOBJ);
    fclose(fichierMTL);

    cout << path << " loaded" << endl << endl;

    return true;
}

void OBJ2_LoadMTL(FILE *fichier,string nameMaterial,OBJ2_Color *color,GLuint *tex,string path,float *ScaleX,float *ScaleY)
{
    string ligneLue;

    //cout << path << endl;

    rewind(fichier);

    *tex=0;

    *ScaleX=1;
    *ScaleY=1;

    while(1)
    {
        lectureLigneFichier(fichier,ligneLue);

        //if(strstr(ligneLue.c_str(),"newmtl ")==ligneLue.c_str())
        if(ligneLue.find("newmtl ")==0)
        {
            char nameMaterial2[ligneLue.length()];

            sscanf(ligneLue.c_str(),"newmtl %s\n",nameMaterial2);

            //if(strcmp(nameMaterial2,nameMaterial.c_str())==0)
            if(string(nameMaterial2)==nameMaterial)
                break;
        }
    }

    //cout << nameMaterial2 << endl;

    while(1)
    {
        bool retour=lectureLigneFichier(fichier,ligneLue);
        if(!retour)
            break;

        //if(strstr(ligneLue.c_str(),"Kd ")==ligneLue.c_str())
        if(ligneLue.find("Kd ")==0)
            sscanf(ligneLue.c_str(),"Kd %f %f %f\n",&color->R,&color->G,&color->B);

        else
        //if(strstr(ligneLue.c_str(),"map_Kd ")==ligneLue.c_str())
        if(ligneLue.find("map_Kd ")==0)
        {
            //if(strstr(ligneLue.c_str(),"-s ")==NULL)
            if(ligneLue.find("-s ")==-1)
            {
                char pathTex[ligneLue.length()];

                sscanf(ligneLue.c_str(),"map_Kd %s\n",pathTex);

                //if(strchr(ligneLue.c_str(),':')!=NULL)
                if(ligneLue.find(":")!=-1)
                {
                    *tex=loadTexture(pathTex,0);

                    if(*tex!=0)
                        cout << "Loaded " << pathTex << " ok" << endl;
                    else
                        cout << "Loaded " << pathTex << " fail" << endl;
                }
                else
                {
                    *tex=loadTexture((path+string(pathTex)).c_str(),0);

                    if(*tex!=0)
                        cout << "Loaded " << path+pathTex << " ok" << endl;
                    else
                        cout << "Loaded " << path+pathTex << " fail" << endl;
                }
            }

            else
            {
                char dataTex[ligneLue.length()];

                strcpy(dataTex,strstr(ligneLue.c_str(),"-s ")+3);

                char pathTex[ligneLue.length()];

                float ScaleZ;

                sscanf(dataTex,"%f %f %f %s\n",ScaleX,ScaleY,&ScaleZ,pathTex);

                //if(strchr(ligneLue.c_str(),':')!=NULL)
                if(ligneLue.find(":")!=-1)
                {
                    *tex=loadTexture(pathTex,0);

                    if(*tex!=0)
                        cout << "Loaded " << pathTex << " ok" << endl;
                    else
                        cout << "Loaded " << pathTex << " fail" << endl;
                }
                else
                {
                    *tex=loadTexture((path+string(pathTex)).c_str(),0);

                    if(*tex!=0)
                        cout << "Loaded " << path+pathTex << " ok" << endl;
                    else
                        cout << "Loaded " << path+pathTex << " fail" << endl;
                }
            }

            //cout << pathTex << " => " << *tex << endl;
        }

        else
        //if(strstr(ligneLue.c_str(),"newmtl ")==ligneLue.c_str())
        if(ligneLue.find("newmtl ")==0)
            break;
    }
}

void OBJ2_DrawVBO(OBJ2_VBO *vbo,const GLuint shader)
{
    int compteurVBOs=0,compteurListeFaces;
    int numFace1,numFace2;
    int numtex;
    //const int attributId=glGetAttribLocation(shader,"numtex");

    /*glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableVertexAttribArray(attributId);*/

    /*while(1)
    {*/
        //glBindBuffer(GL_ARRAY_BUFFER,vbo->bufferVRAM);

        /*glVertexPointer(3,GL_FLOAT,0,BUFFER_OFFSET(0));
        glTexCoordPointer(2,GL_FLOAT,0,BUFFER_OFFSET(vbo->nbVertices*3*sizeof(float)));
        glNormalPointer(GL_FLOAT,0,BUFFER_OFFSET(vbo->nbVertices*3*sizeof(float)+vbo->nbVertices*2*sizeof(float)));
        glColorPointer(3,GL_FLOAT,0,BUFFER_OFFSET(vbo->nbVertices*3*sizeof(float)+vbo->nbVertices*2*sizeof(float)+vbo->nbVertices*3*sizeof(float)));
        glVertexAttribPointer(attributId,1,GL_FLOAT,0,0,BUFFER_OFFSET(vbo->nbVertices*3*sizeof(float)+vbo->nbVertices*2*sizeof(float)+vbo->nbVertices*3*sizeof(float)+vbo->nbVertices*3*sizeof(float)));*/

        /*numtex=0;
        compteurListeFaces=0;
        while(1)
        {
            numFace1=vbo->list_faces_change_tex[compteurListeFaces];
            numFace2=vbo->list_faces_change_tex[compteurListeFaces+1];

            glBindTexture(GL_TEXTURE_2D,vbo->textures[numtex]);

            if(shader!=0)
            {
                if(vbo->textures[numtex]==0)
                    glUniform1i(glGetUniformLocation(shader,"isTextured"),false);
                else
                    glUniform1i(glGetUniformLocation(shader,"isTextured"),true);
            }

            glDrawArrays(GL_TRIANGLES,3*numFace1,3*(numFace2-numFace1));

            numtex++;

            compteurListeFaces++;
            if(compteurListeFaces>=vbo->list_faces_change_tex.size()-1)
                break;
        }*/

        glBindVertexArray(vbo->VAO);

        if(vbo->textures.size()!=0)
        {
            int texShader[vbo->textures.size()];

            int compteurTex=0;
            while(1)
            {
                glActiveTexture(GL_TEXTURE0+compteurTex);
                glBindTexture(GL_TEXTURE_2D,vbo->textures[compteurTex]);

                texShader[compteurTex]=compteurTex;

                //cout << vbo->textures[compteurTex] << endl;

                compteurTex++;
                if(compteurTex==vbo->textures.size())
                    break;
            }
            //cout << endl;

            glUniform1iv(glGetUniformLocation(shader,"tex"),vbo->textures.size(),texShader);
        }

        glDrawArrays(GL_TRIANGLES,0,vbo->nbVertices);

        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(0);

        /*compteurVBOs++;
        if(compteurVBOs==vbos.size())
            break;
    }*/

    /*glBindBuffer(GL_ARRAY_BUFFER,0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableVertexAttribArray(attributId);*/
}

void OBJ2_InitInstance(vector<OBJ2_Vertex> &positions,const GLuint shader)
{
    int compteurPos=0;

    while(1)
    {
        ostringstream os;
        os << compteurPos;

        string index="positions["+os.str()+"]";

        glUniform3f(glGetUniformLocation(shader,index.c_str()),positions[compteurPos].X,positions[compteurPos].Y,positions[compteurPos].Z);

        compteurPos++;
        if(compteurPos==positions.size())
            break;
    }
}
