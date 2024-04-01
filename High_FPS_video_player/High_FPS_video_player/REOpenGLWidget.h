#pragma once

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QtOpenGL/QOpenGLFunctions_3_3_Core>

class REOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
public:

    explicit REOpenGLWidget(QWidget* parent = nullptr);

    ~REOpenGLWidget();

protected:
    // 虚函数需要重写
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

