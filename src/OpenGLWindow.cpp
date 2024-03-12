#include "stdafx.h"
#include "OpenGLWindow.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <iostream>
#include "Triangulation.h"
#include "STLWriter.h"
#include "Point3D.h"
#include "STLReader.h"
#include "BooleanOperations.h"

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
    if (!coordinatesOfFirstStl.empty()) {
        
       
        vector<GLfloat> verticesOfStl1;
        verticesOfStl1 = verticesOfFirstStl();

        vector<GLfloat>colorOfFirstStl1;
        colorOfFirstStl1 = colorOfFirstStl();

        // Set up blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, verticesOfStl1.data());
        glVertexAttribPointer(m_colAttr, 4, GL_FLOAT, GL_FALSE, 0, colorOfFirstStl1.data());

        glEnableVertexAttribArray(m_posAttr);
        glEnableVertexAttribArray(m_colAttr);

        glLineWidth(5.0);
        glDrawArrays(GL_TRIANGLES, 0, coordinatesOfFirstStl.size() ); // Multiply by 2 to render both front and back faces

        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_posAttr);

        verticesOfStl1.clear();
        colorOfFirstStl1.clear();
    }

    // Render the second STL file (answer2)
    if (!coordinatesOfSecondStl.empty()) {
       
      
        vector<GLfloat> verticesOfStl2;
        verticesOfStl2 = verticesOfSecondStl();

       vector<GLfloat> colorOfSecondStl2;
       colorOfSecondStl2 = colorOfSecondStl();
       
        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, verticesOfStl2.data());
        glVertexAttribPointer(m_colAttr, 4, GL_FLOAT, GL_FALSE, 0, colorOfSecondStl2.data());

        glEnableVertexAttribArray(m_posAttr);
        glEnableVertexAttribArray(m_colAttr);

        glLineWidth(5.0);
        glDrawArrays(GL_TRIANGLES, 0, coordinatesOfSecondStl.size());

        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_posAttr);

        verticesOfStl2.clear();
        colorOfSecondStl2.clear();
    }

    // Render the common area
    if (!coordinatesOfIntersectedArea.empty()) {
       
        
        vector<GLfloat> verticesOfIntersected;
        verticesOfIntersected = verticesOfIntersectedPart();

        vector<GLfloat> colorOfIntersected;
        colorOfIntersected = verticesOfIntersectedPart();
       
        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, verticesOfIntersected.data());
        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colorOfIntersected.data());

        glEnableVertexAttribArray(m_posAttr);
        glEnableVertexAttribArray(m_colAttr);

        // Disable blending for solid rendering
        glDisable(GL_BLEND);

        glLineWidth(5.0);
        glDrawArrays(GL_TRIANGLES, 0, coordinatesOfIntersectedArea.size());

        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_posAttr);

        verticesOfIntersected.clear();
        colorOfIntersected.clear();
    }

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
vector<GLfloat> OpenGLWindow::verticesOfFirstStl()
{
    std::vector<GLfloat> vertices1;

    if (!coordinatesOfFirstStl.empty()) {
        for (int i = 0; i < coordinatesOfFirstStl.size(); i++) {
            vertices1.push_back(coordinatesOfFirstStl[i].x());
            vertices1.push_back(coordinatesOfFirstStl[i].y());
            vertices1.push_back(coordinatesOfFirstStl[i].z());
        }
    }

    return vertices1;

}

vector<GLfloat> OpenGLWindow::verticesOfSecondStl()
{

    std::vector<GLfloat> vertices2;
    int k = 0;
    if (!coordinatesOfSecondStl.empty())
    {
        for (int i = 0; i < coordinatesOfSecondStl.size(); i++) {
            vertices2.push_back( coordinatesOfSecondStl[i].x());
            vertices2.push_back(coordinatesOfSecondStl[i].y());
            vertices2.push_back(coordinatesOfSecondStl[i].z());
        }
    }
    return vertices2;

}
vector<GLfloat> OpenGLWindow::verticesOfIntersectedPart()
{
    std::vector<GLfloat> vertices3;
   
    if (!coordinatesOfIntersectedArea.empty())
    {
    for (int i = 0; i < coordinatesOfIntersectedArea.size(); i++) {
        vertices3.push_back(coordinatesOfIntersectedArea[i].x());
        vertices3.push_back(coordinatesOfIntersectedArea[i].y());
        vertices3.push_back(coordinatesOfIntersectedArea[i].z());
    }
}
    return vertices3;

}
vector<GLfloat>OpenGLWindow::colorOfFirstStl()
{
    std::vector<GLfloat> colorOfStl1;

    for (int i = 0; i < coordinatesOfFirstStl.size() * 4; i += 4) {
        colorOfStl1.push_back(0.0f);   // Red component
        colorOfStl1.push_back(1.0f);   // Green component
        colorOfStl1.push_back(1.0f);   // Blue component
        colorOfStl1.push_back(0.3f);   // Alpha component
    }

    return colorOfStl1;

}

vector<GLfloat>OpenGLWindow::colorOfSecondStl()
{
    std::vector<GLfloat> colorOfStl2;

    for (int i = 0; i < coordinatesOfSecondStl.size() * 3; i += 3) {
        colorOfStl2.push_back(1.0f);   // Red component
        colorOfStl2.push_back(0.0f);   // Green component
        colorOfStl2.push_back(1.0f);   // Blue component
        colorOfStl2.push_back(0.3f);   // Alpha component
    }

    return colorOfStl2;

}


std::vector<GLfloat> OpenGLWindow::colorOfIntersectedArea()
{
    std::vector<GLfloat> colorOfIntersected;

    for (int i = 0; i < coordinatesOfIntersectedArea.size() * 4; i += 4) {
        colorOfIntersected.push_back(0.0f);   // Red component (50% darkness)
        colorOfIntersected.push_back(1.0f);   // Green component (no green)
        colorOfIntersected.push_back(0.0f);   // Blue component (no blue)
        colorOfIntersected.push_back(0.7f);   // Alpha component (30% opacity)
    }

    return colorOfIntersected;
}

void OpenGLWindow::showDataOfFirstStl(std::string& filepath)
{
    Triangulation triangulation1;
    STLReader reader1;
    STLWriter writer1;

    reader1.readSTL(filepath, triangulation1);
    writer1.writevector(coordinatesOfFirstStl, triangulation1);

    setData(coordinatesOfFirstStl);
    update();

}
void OpenGLWindow::showDataOfSecondStl(std::string& filepath)
{
    Triangulation triangulation2;
    STLReader reader2;
    STLWriter writer2;

    reader2.readSTL(filepath, triangulation2);
    writer2.writevector(coordinatesOfSecondStl, triangulation2);
    setData(coordinatesOfSecondStl);
    update();

}

void OpenGLWindow::showIntersectedPart(std::string& filepath1, string& filepath2)
{
    Triangulation triangulation1;
    Triangulation triangulation2;

    BooleanOperations intersectedPortion;

    coordinatesOfIntersectedArea = intersectedPortion.getIntersection(triangulation1, triangulation2, filepath1, filepath2);

    setData(coordinatesOfIntersectedArea);
    update();
}


void OpenGLWindow::clearDataOfFirstStl()
{
    // Clear rendering data for the first STL file
    coordinatesOfFirstStl.clear();
    update(); // Trigger repaint
}

void OpenGLWindow::clearDataOfSecondStl()
{
    // Clear rendering data for the second STL file
    coordinatesOfSecondStl.clear();
    update(); // Trigger repaint
}
void OpenGLWindow::clearDataOfIntersectedArea()
{
    coordinatesOfIntersectedArea.clear();
    update();

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




