/*
 * OpenGL demonstration program for ICS Computer Graphics courses
 * $Id: inputModule.h,v 1.1 2002/04/03 00:00:41 mshafae Exp $
 *
 */

#ifndef __INPUTMODULE_H
#define __INPUTMODULE_H

extern bool stopAnimation ;
extern bool activateTrans ;

extern GLfloat lightPositionx ;
extern GLfloat lightPositionz ;
extern GLfloat lightPositiony ;
extern GLfloat theta;
extern GLfloat theta;
extern GLfloat phi;
extern GLfloat eyex;
extern GLfloat eyey;
extern GLfloat eyez;
extern GLfloat eyer ;
extern GLfloat gazex;
extern GLfloat gazey;
extern GLfloat gazez ;
extern GLfloat angle2;

extern int pointx ;
extern int pointy;

extern GLfloat zdistance;

void readKeyboard( unsigned char key, int x, int y );
void readSpecialKeys( int key, int x, int y );
void mouseButtHandler( int button, int state, int x, int y );
void mouseMoveHandler( int x, int y );

#endif
