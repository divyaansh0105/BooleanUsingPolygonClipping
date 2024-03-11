#pragma once

#include <QtWidgets/QMainWindow>

#include "Visualizer.h"

#include "Triangulation.h"

#include "STLReader.h"

#include "STLWriter.h"


class OpenGLWindow;

class Visualizer : public QMainWindow

{

    Q_OBJECT

public:

    Visualizer(QWindow* parent = nullptr);

    ~Visualizer();

private:

    void setupUi();

private:

    QGridLayout* mGridLayout;

    QWidget* mWidget;

    QPushButton* mPushButton1;

    QPushButton* mPushButton2;

    QCheckBox* mButtonforLeft;

    QCheckBox* mButtonforRight;

    QCheckBox* mButtonforIntersection;

    QWidget* mCentralWidget;

    OpenGLWindow* mRenderer;

    string mFilePath1;
    string mFilePath2;


private:

    void openFileDialog1();

    void openFileDialog2();

    void DisplayLeftPart();

    void DisplayRightPart();

    void DisplayIntersectedPart();

};