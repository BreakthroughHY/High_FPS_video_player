#include "REOpenGLWidget.h"

REOpenGLWidget::REOpenGLWidget(QWidget* parent)
	: QOpenGLWidget( parent )
{

}

REOpenGLWidget::~REOpenGLWidget()
{

}

// 虚函数需要重写
void REOpenGLWidget::initializeGL()
{

}

void REOpenGLWidget::resizeGL(int w, int h)
{

}

void REOpenGLWidget::paintGL()
{
	/*glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);*/
}