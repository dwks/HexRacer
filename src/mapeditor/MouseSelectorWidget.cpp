#ifdef CMAKE_BUILD
    #include "MouseSelectorWidget.moc"
#endif

#include "MouseSelectorWidget.h"
#include "opengl/GeometryDrawing.h"
#include <QtGui>
using namespace Project;
using namespace OpenGL;
using namespace Math;

MouseSelectorWidget::MouseSelectorWidget(QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
	: QGLWidget(parent, shareWidget, f)
{
	for (int i = 0; i < 3; i++) {
		mouse_pressed[i] = false;
		mouse_dragged[i] = false;
	}
}

MouseSelectorWidget::~MouseSelectorWidget()
{

}

void MouseSelectorWidget::drawSelectionArrow(Qt::MouseButton button, float z, float arrowhead_size) {

	int mouse_index = mouseButtonToIndex(button);
	if (mouse_index != -1 && mouse_dragged[mouse_index]) {
	}
}

void MouseSelectorWidget::drawSelectionBox(Qt::MouseButton button, Color color, float z, bool overlay) {

	int mouse_index = mouseButtonToIndex(button);

	if (mouse_index != -1 && mouse_dragged[mouse_index]) {

		float min_x = min(mouse_click_pos[mouse_index].getU(), mouse_current_pos.getU());
		float max_x = max(mouse_click_pos[mouse_index].getU(), mouse_current_pos.getU());
		float min_y = min(mouse_click_pos[mouse_index].getV(), mouse_current_pos.getV());
		float max_y = max(mouse_click_pos[mouse_index].getV(), mouse_current_pos.getV());

		Color::glColor(color);

		glBegin(GL_LINE_LOOP);

		glVertex3f(min_x, min_y, z);
		glVertex3f(max_x, min_y, z);
		glVertex3f(max_x, max_y, z);
		glVertex3f(min_x, max_y, z);

		glEnd();

		if (overlay) {
			glDepthMask(GL_FALSE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

			Color::glColor(color, 0.10f);

			glBegin(GL_QUADS);

			glVertex3f(min_x, min_y, z);
			glVertex3f(max_x, min_y, z);
			glVertex3f(max_x, max_y, z);
			glVertex3f(min_x, max_y, z);

			glEnd();

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

	}
}

int MouseSelectorWidget::mouseButtonToIndex(Qt::MouseButton button) {
	switch (button) {
		case Qt::LeftButton: return 0;
		case Qt::MidButton: return 1;
		case Qt::RightButton: return 2;
        default: break;
	}
	return -1;
}

Qt::MouseButton MouseSelectorWidget::mouseIndexToButton(int index) {
	switch (index) {
		case 0: return Qt::LeftButton;
		case 1: return Qt::MidButton;
		case 2: return Qt::RightButton;
        default: break;
	}
	return Qt::NoButton;
}

void MouseSelectorWidget::mousePressEvent(QMouseEvent *event) {

	int mouse_index = mouseButtonToIndex(event->button());

	if (mouse_index != -1) {

		mouse_click_screen_pos[mouse_index] = Point(event->x(), event->y());
		Point worldPos = screenToWorld(mouse_click_screen_pos[mouse_index]);

		mouse_pressed[mouse_index] = true;
		mouse_dragged[mouse_index] = false;
		mouse_click_pos[mouse_index] = worldPos;
		mouse_previous_pos[mouse_index] = worldPos;

		mouse_previous_screen_pos[mouse_index] = mouse_click_screen_pos[mouse_index];
		
		this->mouseClicked(event->button(), worldPos, event->modifiers());
		this->mouseClickedScreen(event->button(), mouse_click_screen_pos[mouse_index], event->modifiers());
	}

	active_modifiers = event->modifiers();
}

void MouseSelectorWidget::mouseMoveEvent(QMouseEvent *event) {

	Point prev_screen_pos = mouse_current_screen_pos;
	Point prev_pos = mouse_current_pos;

	mouse_current_screen_pos = Point(event->x(), event->y());
	mouse_current_pos = screenToWorld(mouse_current_screen_pos);

	this->mouseDragged(Qt::NoButton, mouse_current_pos, Point(), prev_pos, event->modifiers());
	this->mouseDraggedScreen(Qt::NoButton, mouse_current_screen_pos, Point(), prev_screen_pos, event->modifiers());

	for (int i = 0; i < 3; i++) {
		if (mouse_pressed[i]) {
			mouse_dragged[i] = true;
			this->mouseDragged(mouseIndexToButton(i), mouse_current_pos, mouse_click_pos[i], mouse_previous_pos[i], event->modifiers());
			this->mouseDraggedScreen(mouseIndexToButton(i), mouse_current_screen_pos, mouse_click_screen_pos[i], mouse_previous_screen_pos[i], event->modifiers());
			mouse_previous_screen_pos[i] = mouse_current_screen_pos;
			mouse_previous_pos[i] = mouse_current_pos;
		}
	}

	active_modifiers = event->modifiers();
}

void MouseSelectorWidget::mouseReleaseEvent(QMouseEvent *event) {

	Point screenPos = Point(event->x(), event->y());
	Point worldPos = screenToWorld(screenPos);
	int mouse_index = mouseButtonToIndex(event->button());

	if (mouse_index != -1) {
		mouse_pressed[mouse_index] = false;
		mouse_dragged[mouse_index] = false;
		this->mouseReleased(event->button(), worldPos, mouse_click_pos[mouse_index], event->modifiers());
		this->mouseReleasedScreen(event->button(), screenPos, mouse_click_screen_pos[mouse_index], event->modifiers());
	}

	active_modifiers = event->modifiers();
}

void MouseSelectorWidget::wheelEvent(QWheelEvent *event) {

	if (event->delta() > 0)
		this->wheelMoved(true, event->modifiers());
	else if (event->delta() < 0)
		this->wheelMoved(false, event->modifiers());

	active_modifiers = event->modifiers();
}