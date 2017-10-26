#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MyGraphicsView.h"



MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int numberOfCells = 32;

    QGridLayout *controlsLayout = new QGridLayout;
    int windowSize=640;
    int cellSize = windowSize / numberOfCells;
    QGridLayout *grid = new QGridLayout;
    QPushButton *but1 = new QPushButton("pen");
    QPushButton *but2 = new QPushButton("erase");
    QPushButton *but3 = new QPushButton("rect");
    QWidget *container = new QWidget();
    container->setLayout(controlsLayout);
    grid->addWidget(but1, 0, 0, 1, 2);
    grid->addWidget(but2, 0, 2, 1, 2);
    grid->addWidget(but3, 0, 4, 1, 2);
    grid->addWidget(container, 1, 1, 4, 4);
    QGraphicsScene *scene = new QGraphicsScene(0,0,windowSize,windowSize,this);
    signalMapper = new QSignalMapper(this);
    for(int i = 0 ; i < numberOfCells ; i++ )
    {
        for(int j = 0 ; j < numberOfCells ; j++ )
        {
            QString s = QString::number(i)+"."+QString::number(j);
            button[i][j] = new QPushButton();
            std::string temp = "";
            button[i][j]->setObjectName(s);
            button[i][j]->setObjectName(s);
            connect(button[i][j], SIGNAL(pressed()), signalMapper, SLOT(map()));
                    signalMapper->setMapping(button[i][j], button[i][j]->objectName());
            button[i][j]->setStyleSheet("background-color: rgba(255, 255, 255, 10);");
            controlsLayout->addWidget(button[i][j], i, j);
        }
    }
    connect(signalMapper, SIGNAL(mapped(QString)), this, SIGNAL(pressed(QString)));
    connect(this, &MainWindow::pressed, this, &MainWindow::buttonClicked);

    QGraphicsView *v2 = new QGraphicsView(scene);
    v2->setLayout(grid);
    controlsLayout->setHorizontalSpacing(0);
    controlsLayout->setVerticalSpacing(0);
    setCentralWidget(v2);
    v2->setGeometry(100,100,640,640);
    v2->setGeometry(QRect(50, 50, 640, 640));
    QImage image(":/Checkered.png");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    v2->show();
    x2=v2;
}

void MainWindow::buttonClicked(const QString &text){
    QStringList pieces = text.split( "." );
    QString needednum1 = pieces.value( pieces.length() - 2 );
    QString needednum2 = pieces.value( pieces.length() - 1 );
    int x,y;
    stringstream(needednum1.toStdString()) >> x;
    stringstream(needednum2.toStdString()) >> y;
    button[x][y]->setStyleSheet("background-color: black;");
}


void MainWindow::wheelEvent ( QWheelEvent * event )
{
 int numDegrees = event->delta() /8;
 int numSteps = numDegrees / 15; // see QWheelEvent documentation
 _numScheduledScalings += numSteps;
 if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
 _numScheduledScalings = numSteps;
 QTimeLine *anim = new QTimeLine(350, this);
 anim->setUpdateInterval(20);
 connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
 connect(anim, SIGNAL (finished()), SLOT (animFinished()));
 anim->start();
}

void MainWindow::animFinished()
{
 if (_numScheduledScalings > 0)
 _numScheduledScalings--;
 else
 _numScheduledScalings++;
 sender()->~QObject();
}

void MainWindow::scalingTime(qreal x)
{
 qreal factor = 1.0+ qreal(_numScheduledScalings) / 300.0;
 x2->scale(factor, factor);
 emit scale(factor, factor);
}

MainWindow::~MainWindow()
{
    delete ui;
}
