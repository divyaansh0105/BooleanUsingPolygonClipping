#include "stdafx.h"
#include "OpenGLWindow.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <iostream>
#include "../headers/Triangulation.h"
#include "../headers/STLWriter.h"
#include "../headers/Point3D.h"
#include "../headers/STLReader.h"
#include "../headers/Clipping.h"

OpenGLWindow::OpenGLWindow(const QColor& background, QMainWindow* parent) :
    mBackground(background)
{
    setParent(parent);
    setMinimumSize(300, 250);
}
OpenGLWindow::~OpenGLWindow()
{
    reset();
}

void OpenGLWindow::reset()
{
    // And now release all OpenGL resources.
    makeCurrent();
    delete mProgram;
    mProgram = nullptr;
    delete mVshader;
    mVshader = nullptr;
    delete mFshader;
    mFshader = nullptr;
    mVbo.destroy();
    doneCurrent();

    QObject::disconnect(mContextWatchConnection);
}

void OpenGLWindow::setData(const std::vector<Point3D>& vertices) {
    mVertices.clear(); // Clear the existing vertices

    // Convert the vertices to QVector3D and add them to mVertices
    for (const auto& vertex : vertices) {
        QVector3D qVertex(vertex.x(), vertex.y(), vertex.z());
        mVertices.append(qVertex);
    }
}

void OpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mProgram->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f * scaleFactor, 4.0f / 3.0f * scaleFactor, 0.1f, 100.0f);
    matrix.translate(0, 0, -15);
    matrix.rotate(rotationAngle);
   
    mProgram->setUniformValue(m_matrixUniform, matrix);

    std::vector<QVector3D> gradientColors;
    gradientColors.push_back(QVector3D(1.0f, 0.0f, 0.0f));  // Red
    gradientColors.push_back(QVector3D(0.0f, 1.0f, 0.0f));  // Green
    gradientColors.push_back(QVector3D(0.0f, 0.0f, 1.0f));  // Blue


    // Render the first STL file (answer1)
    
    mProgram->release();
}

void OpenGLWindow::initializeGL()
{
    static const char* vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

    static const char* fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";
    rotationAngle = QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, 0.0f);
    initializeOpenGLFunctions();


    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mProgram->link();
    m_posAttr = mProgram->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = mProgram->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = mProgram->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
}

void OpenGLWindow::showData1(std::string &filepath)
{
    Triangulation triangulation1;
    STLReader reader1;
    STLWriter writer1;

    reader1.readSTL(filepath, triangulation1);
    writer1.writevector(answer1, triangulation1);

    setData(answer1);
    update();
   
}
void OpenGLWindow::showData2(std::string& filepath)
{
    Triangulation triangulation2;
    STLReader reader2;
    STLWriter writer2;

    reader2.readSTL(filepath, triangulation2);
    writer2.writevector(answer2, triangulation2);
    setData(answer2);
    update();

}

void OpenGLWindow::showIntersectedPart(std::string& filepath1 , string& filepath2)
{
    Triangulation triangulation1;
    Triangulation triangulation2;

    Clipping intersectedPortion;

    commonArea=intersectedPortion.clip(triangulation1, triangulation2,filepath1,filepath2);

    setData(commonArea);
    update();
}

void OpenGLWindow::clearData1()
{
    // Clear rendering data for the first STL file
    answer1.clear();
    update(); // Trigger repaint
}

void OpenGLWindow::clearData2()
{
    // Clear rendering data for the second STL file
    answer2.clear();
    update(); // Trigger repaint
}
void OpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (event->buttons() & Qt::LeftButton)
    {
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.5f * dx);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.5f * dy);

        rotationAngle = rotX * rotY * rotationAngle;
        update();
    }
    lastPos = event->pos();
}

void OpenGLWindow::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        zoomOut();
    }
    else
    {
        zoomIn();
    }
}

void OpenGLWindow::zoomIn()
{
    scaleFactor *= 1.1f;
    update();
}

void OpenGLWindow::zoomOut()
{
    scaleFactor /= 1.1f;
    update();
}



