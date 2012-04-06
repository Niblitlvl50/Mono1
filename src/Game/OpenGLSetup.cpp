//
//  OpenGLSetup.cpp
//  Monolith1
//
//  Created by Niblit on 2012-04-03.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "OpenGLSetup.h"
#include <GLUT/glut.h>

using namespace game;

void OpenGLSetup::OnCreated()
{
    const float ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    const float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    const float position[] = { 0.0, 1.0, 0.0, 0.0 };
    const float mat_shininess[] = { 90.0 };
    const float mat_specular[] = { 0.8, 0.8, 0.8, 1.0 };
    const float mat_diffuse[] = { 0.46, 0.66, 0.795, 1.0 };
    const float mat_ambient[] = { 0.3, 0.4, 0.5, 1.0 };
    const float lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    const float lmodel_localviewer[] = { 0.0 };
    
    const float fog_color[] = { 0.0, 0.5, 0.9, 1.0 };
    
    glFrontFace(GL_CCW);
    
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_localviewer);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.0000025);
    glFogfv(GL_FOG_COLOR, fog_color);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.0, 0.5, 0.9, 1.0);
}

void OpenGLSetup::OnChanged(int width, int height)
{
    const float floatWidth = static_cast<float>(width);
    const float floatHeight = static_cast<float>(height);
    const float aspect = floatWidth / floatHeight;
    
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, aspect, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


