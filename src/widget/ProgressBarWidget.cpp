#include "ProgressBarWidget.h"
#include "opengl/Color.h"
#include "WidgetRenderer.h"
#include "AbsoluteLayout.h"

namespace Project {
namespace Widget {

	ProgressBarWidget::ProgressBarWidget(const std::string &name, const WidgetRect &rect)
		: AbstractWidget(name) {

		setLayout(new AbsoluteLayout(rect));
		updateLayout(rect);

		totalSteps = 10;
		currentStep = 5;
	}

	void ProgressBarWidget::setCurrentStep(int current_step) {
		currentStep = current_step;

		/*
		Widget::WidgetRenderer::
	    
		renderer.begin();
		this->accept(renderer);
		renderer.end();
		*/
		render();

		glFlush();
	}

	void ProgressBarWidget::render() {

		double progress = 0.0;
		if (currentStep >= 0 && currentStep < totalSteps)
			progress = (double) currentStep / (double) totalSteps;
		else
			progress = 1.0;

		WidgetPoint corner = getBoundingRect().getCorner();
		WidgetPoint dimensions = getBoundingRect().getDimensions();
		double right = corner.getX() + progress * dimensions.getX();
		double done_right = corner.getX() + dimensions.getX();
		double bottom = corner.getY()+dimensions.getY();

		OpenGL::Color::glColor(OpenGL::Color::GREY);
		glBegin(GL_QUADS);
		WidgetRenderer::glVertex(corner);
		WidgetRenderer::glVertex(WidgetPoint(done_right, corner.getY()));
		WidgetRenderer::glVertex(WidgetPoint(done_right, bottom));
		WidgetRenderer::glVertex(WidgetPoint(corner.getX(), bottom));
		glEnd();

		OpenGL::Color::glColor(OpenGL::Color::WHITE);
		glBegin(GL_QUADS);
		WidgetRenderer::glVertex(corner);
		WidgetRenderer::glVertex(WidgetPoint(right, corner.getY()));
		WidgetRenderer::glVertex(WidgetPoint(right, bottom));
		WidgetRenderer::glVertex(WidgetPoint(corner.getX(), bottom));
		glEnd();
	}

}  // namespace Widget
}  // namespace Project
