#ifndef MOUSESELECTORWIDGET_H
#define MOUSESELECTORWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include "math/Point.h"
#include "opengl/Color.h"
using namespace std;
using namespace Project;
using namespace Math;
using namespace OpenGL;
 
class MouseSelectorWidget : public QGLWidget
{
	Q_OBJECT

public:
	MouseSelectorWidget(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
	~MouseSelectorWidget();

	static int mouseButtonToIndex(Qt::MouseButton button);
	static Qt::MouseButton mouseIndexToButton(int index);
	static const int mouseLeftIndex = 0;
	static const int mouseMidIndex = 1;
	static const int mouseRightIndex = 2;

protected:

	int wheel_state;
	bool mouse_pressed[3];
	bool mouse_dragged[3];
	Point mouse_click_pos[3];
	Point mouse_click_screen_pos[3];
	Point mouse_previous_pos[3];
	Point mouse_previous_screen_pos[3];
	Point mouse_current_pos;
	Point mouse_current_screen_pos;
	Qt::KeyboardModifiers active_modifiers;

	void drawSelectionArrow(Qt::MouseButton button, float z = 0.0f, float arrowhead_size = 0.25f);

	void drawSelectionBox(Qt::MouseButton button, Project::OpenGL::Color color, float z = 0.0f, bool overlay = false);

	/*Abstract Methods*/

	/* Translates a point from screen coordinates to world coordinates
	screenPos: The point in screen coordinates
	return: The given point translated to world coordinates
	*/
	virtual Point screenToWorld(Point screenPos) {return screenPos;}

	/* Handles the event of a mouse click with all positions in world coordinates
	button: The mouse button that was pressed
	click_position: The position of the mouse when the button was pressed
	modifiers: The keyboard modifiers active when the event occured
	*/
	virtual void mouseClicked(Qt::MouseButton button, Point click_position, Qt::KeyboardModifiers modifiers) {}

	/* Handles the event of a mouse click with all positions in screen coordinates
	button: The mouse button that was pressed
	click_position: The position of the mouse when the button was pressed
	modifiers: The keyboard modifiers active when the event occured
	*/
	virtual void mouseClickedScreen(Qt::MouseButton button, Point click_position, Qt::KeyboardModifiers modifiers) {}

	/* Handles the event of a mouse button release with all positions in world coordinates
	button: The mouse button that was released
	release_position: The position of the mouse when the button was released
	click_position: The position of the mouse when the button was pressed
	modifiers: The keyboard modifiers active when the event occured
	*/
	virtual void mouseReleased(Qt::MouseButton button, Point release_position, Point click_position, Qt::KeyboardModifiers modifiers) {}
	
	/* Handles the event of a mouse button release with all positions in screen coordinates
	button: The mouse button that was released
	release_position: The position of the mouse when the button was released
	click_position: The position of the mouse when the button was pressed
	modifiers: The keyboard modifiers active when the event occured
	*/
	virtual void mouseReleasedScreen(Qt::MouseButton button, Point release_position, Point click_position, Qt::KeyboardModifiers modifiers) {}
	
	/* Handles the event of a mouse being moved with all positions in world coordinates
	button: The mouse button of the drag event (Qt::NoButton for general move events)
	current_position: The current position of the mouse
	click_position: The position of the mouse when the button was pressed
	previous_position: The last position of the mouse
	modifiers: The keyboard modifiers active when the event occured
	*/
	virtual void mouseDragged(Qt::MouseButton button, Point current_position, Point click_position, Point previous_position, Qt::KeyboardModifiers modifiers) {}
	
	/* Handles the event of a mouse being moved with all positions in screen coordinates
	button: The mouse button of the drag event (Qt::NoButton for general move events)
	current_position: The current position of the mouse
	click_position: The position of the mouse when the button was pressed
	previous_position: The last position of the mouse
	modifiers: The keyboard modifiers active when the event occured
	*/
	virtual void mouseDraggedScreen(Qt::MouseButton button, Point current_position, Point click_position, Point previous_position, Qt::KeyboardModifiers modifiers) {}
	
	/* Handles the event of a mouse wheel movement
	up: True if the mouse wheel was moved up, False if the mouse wheel was moved down
	modifiers: The keyboard modifiers active when the event occured
	*/
	virtual void wheelMoved(bool up, Qt::KeyboardModifiers modifiers) {}

	/*Virtual Methods*/

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	
};

#endif // MOUSESELECTORWIDGET_H
