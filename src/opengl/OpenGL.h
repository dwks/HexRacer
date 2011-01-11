/** Includes whatever header files are necessary to get OpenGL working.
*/

#ifndef PROJECT_OPENGL__OPENGL_H
#define PROJECT_OPENGL__OPENGL_H

#ifdef WIN32
    #include <windows.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#endif
