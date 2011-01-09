#ifdef CMAKE_BUILD
    #include "MainDisplay.moc"
#endif

#include <QTimer>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include "MainDisplay.h"
#include "log/Logger.h"

#include "MathWrapper.h"
#include "GeometryDrawing.h"

namespace Project {
namespace OpenGL {

const double MainDisplay::FIELD_OF_VIEW = 60.0;

MainDisplay::MainDisplay() {
    scale = 1.0;
    angle = 0.0;
    mouseIsDown = false;
    trackball = new Trackball();
    
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

MainDisplay::~MainDisplay() {
    delete trackball;
}

void MainDisplay::initializeGL() {
    LOG2(OPENGL, INIT, "Initializing OpenGL . . .");
    
    // Depth buffer stuff
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    // Lighting stuff
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Texture stuff
    glEnable(GL_TEXTURE_2D);
    
    // Allow default materials to be used so that colours show up
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Enable line anti-aliasing, so that when glEnable(GL_LINE_SMOOTH) is
    // called, lines will be anti-aliased.
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_LINE_SMOOTH);  // use antialiased lines by default
    
    // Default to smooth shading
    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    
    // Blending settings, for translucency
    glDepthFunc(GL_LEQUAL);
    
    LOG2(OPENGL, INIT, "OpenGL initialization successful");
    
    setViewMatrix();

	//Instantiate quadrics
	test_cylinder = gluNewQuadric();
	gluQuadricDrawStyle(test_cylinder, GLU_FILL);
	test_ring = gluNewQuadric();
	gluQuadricDrawStyle(test_ring, GLU_LINE);
}

void MainDisplay::resizeGL(int width, int height) {
    LOG2(OPENGL, INIT, "Resizing OpenGL portal to " << width << "x" << height);
    
    OpenGLPortal::resizeGL(width, height);
    
    setViewMatrix();
}

void MainDisplay::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslated(0.0, 0.0, -2.0);
    
    trackball->applyTransform();
    
    /*glTranslated(
        -translation.getX(),
        -translation.getY(),
        -translation.getZ());*/
    
    //glRotated(angle, 0.0, 1.0, 0.0);
    //glScaled(scale, scale, scale);
    
    // add a light
    {
        static GLfloat pos[] = {0.0, 0.0, 2.0, 1.0};
        
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
    }

	//Draw a white-edged cylinder, because why not?
	glEnable(GL_LIGHTING);
	glRotatef(35.0f, 0.3f, 0.4f, 0.25f);
	
	glColor3f(1.0f, 0.0f, 0.0f); //Red

	gluCylinder(test_cylinder, 0.25f, 0.35f, 1.0f, 60, 4);

	glLineWidth(3.0f); //Thicker lines

	glDisable(GL_LIGHTING); //No lighting
	glColor3f(1.0f, 1.0f, 1.0f); //White

	gluDisk(test_ring, 0.25f, 0.25f, 60, 1);
	glTranslatef(0.0f, 0.0f, 1.0f);
	gluDisk(test_ring, 0.35f, 0.35f, 60, 1);

	glLineWidth(1.0f); //Reset line width
    
	/*
    glBegin(GL_TRIANGLES);
    MathWrapper::glVertex(Math::Point(0.0, 0.1, 0.0));
    MathWrapper::glVertex(Math::Point(-0.1, 0.0, 0.0));
    MathWrapper::glVertex(Math::Point(+0.1, 0.0, 0.0));
    glEnd();
	*/

	Math::BoundingBox3D& box3D = Math::BoundingBox3D(0.4f, 0.2f, 0.2f);
	GeometryDrawing::drawObject(box3D, true);
	//delete(&box3D);

	Math::BoundingBox2D& box2D = Math::BoundingBox2D(0.15f, 0.2f, Math::X_AXIS);
	GeometryDrawing::drawObject(box2D, true);
	//delete(&box2D);

	box2D = Math::BoundingBox2D(0.05f, 0.125f, Math::Z_AXIS);
	box2D.translate2D(Math::Point2D(0.025f, 0.4f, Math::Z_AXIS));
	GeometryDrawing::drawObject(box2D, true);
	//delete(&box2D);
    
    repaintManager.scheduleNextRepaint(this);
    glFlush();
}

void MainDisplay::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Shift) {
        //LOG(OPENGL, OPENGL, "Shift pressed");
    }
}

void MainDisplay::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Shift) {
        //LOG(OPENGL, OPENGL, "Shift released");
    }
}

void MainDisplay::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        mouseIsDown = true;
        
        Math::Point position = screenToGL(event->pos());
        trackball->setMouseStartAt(position);
    }
}

void MainDisplay::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        mouseIsDown = false;
    }
}

void MainDisplay::mouseMoveEvent(QMouseEvent *event) {
    if(mouseIsDown) {
        Math::Point position = screenToGL(event->pos());
        trackball->setMouseCurrentAt(position);
        
        //updateGL();  // not necessary, continuous update
    }
}

void MainDisplay::wheelEvent(QWheelEvent *event) {
    // mouse wheel delta is defined in multiples of 120
    double move = event->delta() / 15.0 / 8.0;
    
    if(event->modifiers() & Qt::SHIFT) {
        Math::Point axis = Math::Point(0.0, 0.0, 1.0);
        translation += -move * trackball->multiplyByInverseMatrix(axis);
    }
    else {
        if(move > 0.0) {
            scale *= 1.1;
        }
        else {
            scale /= 1.1;
        }
    }
}

void MainDisplay::setViewMatrix() {
    //return;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    double aspect_ratio
        = getCurrentDimensions().getX() / getCurrentDimensions().getY();
    
    gluPerspective(FIELD_OF_VIEW, aspect_ratio, 0.01, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
}

}  // namespace OpenGL
}  // namespace Project
