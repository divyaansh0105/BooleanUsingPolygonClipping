#include <vector>
#include <QString>
#include "stdafx.h"


#include "OpenGLWindow.h"
#include "Visualizer.h"


Visualizer::Visualizer(QWindow* parent) : QMainWindow(nullptr)
{
    setupUi();

    connect(mPushButton1, &QPushButton::clicked, this, &Visualizer::openFileDialog1);

    connect(mPushButton2, &QPushButton::clicked, this, &Visualizer::openFileDialog2);

    connect(mButtonforLeft, &QPushButton::clicked, this, &Visualizer::DisplayLeftPart);

    connect(mButtonforRight, &QPushButton::clicked, this, &Visualizer::DisplayRightPart);

    connect(mButtonforIntersection, &QPushButton::clicked, this, &Visualizer::DisplayIntersectedPart);
}

Visualizer::~Visualizer()
{}

void Visualizer::setupUi()
{
    resize(1000, 1000);

    mGridLayout = new QGridLayout(this);

    mPushButton1 = new QPushButton("Load STL File 1", this);
    mPushButton1->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 10px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");

    mGridLayout->addWidget(mPushButton1, 1, 6, 2, 1);

    mPushButton2 = new QPushButton("Load STL File 2", this);
    mPushButton2->setStyleSheet("QPushButton { background-color: #008CBA; color: white; border: none; padding: 10px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");

    mGridLayout->addWidget(mPushButton2, 1, 7, 2, 1);

    mButtonforLeft = new QCheckBox("Left Portion", this);
    mButtonforLeft->setStyleSheet("QCheckBox { background-color: #f44336; color: white; padding: 10px; border: none; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");

    mGridLayout->addWidget(mButtonforLeft, 3, 6, 1, 2);

    mButtonforRight = new QCheckBox("Right Portion", this);
    mButtonforRight->setStyleSheet("QCheckBox { background-color: #f44336; color: white; padding: 10px; border: none; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");

    mGridLayout->addWidget(mButtonforRight, 4, 6, 1, 2);

    mButtonforIntersection = new QCheckBox("Common Portion", this);
    mButtonforIntersection->setStyleSheet("QCheckBox { background-color: #f44336; color: white; padding: 10px; border: none; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");

    mGridLayout->addWidget(mButtonforIntersection, 5, 6, 1, 2);

    mRenderer = new OpenGLWindow(QColor(0, 1, 0), this);

    mGridLayout->addWidget(mRenderer, 0, 0, 8, 6);

    mWidget = new QWidget(this);

    mWidget->setLayout(mGridLayout);

    setCentralWidget(mWidget);

    setWindowTitle(QCoreApplication::translate("BooleanUsingPolygonClipping", "BooleanUsingPolygonClipping", nullptr));
}

void Visualizer::openFileDialog1()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open STL File 1"), QDir::homePath(), tr("STL Files (*.stl)"));
    mFilePath1 = filePath.toStdString();
}

void Visualizer::openFileDialog2()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open STL File 2"), QDir::homePath(), tr("STL Files (*.stl)"));
    mFilePath2 = filePath.toStdString();
}

void Visualizer::DisplayLeftPart()
{
    if (mButtonforLeft->isChecked()) {
        if (mFilePath1.empty()) {
            openFileDialog1();
        }
        mRenderer->showDataOfFirstStl(mFilePath1);
        update();
    }
    else {
        mRenderer->clearDataOfFirstStl(); // Clear the rendering data
        update();
    }
}

void Visualizer::DisplayRightPart()
{
    if (mButtonforRight->isChecked()) {
        if (mFilePath2.empty()) {
            openFileDialog2();
        }
        mRenderer->showDataOfSecondStl(mFilePath2);
        update();
    }
    else {
        mRenderer->clearDataOfSecondStl(); // Clear the rendering data
        update();
    }
}

void Visualizer::DisplayIntersectedPart()
{
    if (mButtonforIntersection->isChecked()) {
        if (mFilePath1.empty() || mFilePath2.empty()) {
            // If either file path is empty, prompt the user to load both files
            openFileDialog1();
            openFileDialog2();
        }
        mRenderer->showIntersectedPart(mFilePath1, mFilePath2);
        update();
    }
    else {
        // If the intersected part button is unchecked, clear the rendering data for file 2
        mRenderer->clearDataOfIntersectedArea();
        update(); // Trigger repaint
    }
}