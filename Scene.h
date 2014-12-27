#ifndef SCENE_H
#define SCENE_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include "Triangle.h"

class Scene : public QOpenGLWidget
{
    Q_OBJECT

public:
    Scene( QWidget *parent = 0 );
    ~Scene();

private slots:
    void slotRotate();

private:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );

    Triangle *m_triangle;
    QOpenGLShaderProgram m_program;

    int m_vertexAttr;
    int m_textureAttr;
    int m_matrixUniform;
    int m_textureUniform;

    int m_angle;
    QTimer m_timer;
};

#endif // SCENE_H
