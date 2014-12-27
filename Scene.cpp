#include <QMatrix4x4>
#include "Scene.h"

Scene::Scene( QWidget *parent ) :
    QOpenGLWidget( parent ),
    m_angle( 0 )
{
    connect( &m_timer, SIGNAL( timeout() ),
             this, SLOT( slotRotate() ) );

    m_timer.start( 10 );
}

void Scene::slotRotate()
{
    ++m_angle;

    if ( m_angle >= 360 )
        m_angle = 0;

    update();
}

Scene::~Scene()
{
    makeCurrent();
    delete m_triangle;
    doneCurrent();
}

void Scene::initializeGL()
{
    glClearColor( 0.1f, 0.1f, 0.2f, 1.0f );

    QOpenGLShader vShader( QOpenGLShader::Vertex );
    vShader.compileSourceFile( ":/Shaders/vShader.glsl" );

    QOpenGLShader fShader( QOpenGLShader::Fragment );
    fShader.compileSourceFile( ":/Shaders/fShader.glsl" );

    m_program.addShader( &vShader );
    m_program.addShader( &fShader );
    if ( !m_program.link() )
    {
        qWarning( "Error: unable to link a shader program." );
        return;
    }

    m_vertexAttr = m_program.attributeLocation( "vertexAttr" );
    m_textureAttr = m_program.attributeLocation( "textureAttr" );
    m_matrixUniform = m_program.uniformLocation( "matrix" );
    m_textureUniform = m_program.uniformLocation( "textureUniform" );

    m_triangle = new Triangle( &m_program, m_vertexAttr, m_textureAttr, m_textureUniform );
}

void Scene::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );

    if ( !m_program.bind() )
        return;

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    QMatrix4x4 matrix;
    matrix.perspective( 60.0f, 4.0f / 3.0f, 0.1f, 400.0f );
    matrix.translate( 0.0f, 0.0f, -2.0f );
    matrix.rotate( m_angle, 0.0f, 1.0f, 0.0f );
    m_program.setUniformValue( m_matrixUniform, matrix );

    m_triangle->draw();

    m_program.release();
}

void Scene::resizeGL( int w, int h )
{
    glViewport( 0, 0, w, h );
}
