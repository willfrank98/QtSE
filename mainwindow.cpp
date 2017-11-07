#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frame.h"
using namespace std;


MainWindow::MainWindow(Model *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sizeX = 32;
    sizeY = 32;
    QImage dummyImage(sizeX, sizeY, QImage::Format_ARGB32);
	ui->setupUi(this);
    this->model = model;
    animationCounter = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    cd = new QColorDialog();
    cd->setCurrentColor(*new QColor(Qt::black));
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->setAlignment(Qt::AlignLeft);
    layout1->setMargin(10);
    ui->frameContainer->addLayout(layout1);
    //This is ugly i know but not sure how to make a better way to iterate through the buttons.
    colorHistoryButtons.append(ui->palette1);
    colorHistoryButtons.append(ui->palette2);
    colorHistoryButtons.append(ui->palette3);
    colorHistoryButtons.append(ui->palette4);
    colorHistoryButtons.append(ui->palette5);
    colorHistoryButtons.append(ui->palette6);
    colorHistoryButtons.append(ui->palette7);
    colorHistoryButtons.append(ui->palette8);
    colorHistoryButtons.append(ui->palette9);
    colorHistoryButtons.append(ui->palette10);
    colorHistoryButtons.append(ui->palette11);
    colorHistoryButtons.append(ui->palette12);
    colorHistoryButtons.append(ui->palette13);
    colorHistoryButtons.append(ui->palette14);

    //set the default colors
    Canvas::c1 = QColor(Qt::black);
    Canvas::c2 = QColor(Qt::white);
    // connects the buttons

    connect(ui->color1Box, &QAbstractButton::clicked, this, &MainWindow::colorBox1Clicked);
    connect(ui->color2Box, &QAbstractButton::clicked, this, &MainWindow::colorBox2Clicked);

    // connects the undo/redo buttons
    //connect(ui->actionUndo, &QAction::triggered, this, Canvas::undo());
    //connect(ui->actionRedo, &QAction::triggered, this, Canvas::redo());

    // connects the File>New actions\

    connect(ui->actioncustom_2, &QAction::triggered, this, [this](){ custom_clicked(); });
    connect(ui->action2x3, &QAction::triggered, this, [this](){ createCanvas(2,3); });
    connect(ui->action4x8, &QAction::triggered, this, [this](){ createCanvas(4,8); });
    connect(ui->action8x4, &QAction::triggered, this, [this](){ createCanvas(8,4); });
    connect(ui->action8x8, &QAction::triggered, this, [this](){ createCanvas(8,8); });
    connect(ui->action16x16, &QAction::triggered, this, [this](){ createCanvas(16,16); });
    connect(ui->action32x32, &QAction::triggered, this, [this](){ createCanvas(32,32); });
    connect(ui->action64x64, &QAction::triggered, this, [this](){ createCanvas(64,64); });
    connect(ui->action128x128, &QAction::triggered, this, [this](){ createCanvas(128,128); });
    // connects the File>Export actions
//    connect(ui->actionAnimated_GIF, &QAction::triggered, &model, [=](){
        /*
         * This is just a lambda function to check if FreeImage fails for anyone.
         * Looks like Window will be difficult to get linked correctly.
         * Will fix Monday soon.
         */
//        FreeImage_Initialise();
//        qDebug() << "Initialized FreeImage " << FreeImage_GetVersion();
//        FreeImage_DeInitialise();
    //});

    // connects the File>Exit action
//    connect(ui->actionExit, &QAction::triggered, &model, &Model::exit);
    // connects the File>Exit action  

	//connects save and load actions to functions which bring up a FileDialog, then call to the model
	connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveAction);
	connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::loadAction);

//	connect(this, &MainWindow::createSaveFile, &model, &Model::saveFramesToFile);
//	connect(this, &MainWindow::loadSaveFile, &model, &Model::loadFramesFromFile);
}

MainWindow::~MainWindow()
{
	delete ui;
}
//is called immediately after the save button is called to get a file path
void MainWindow::saveAction()
{
    QString filename = "1.ssp";
    QFile file(filename);
    file.open(QFile::WriteOnly);
    QStringList list;
    QString xS = QString::number(sizeY);
    QString yS = QString::number(sizeX);
    QString numFrames = QString::number(scenes.size());
    list.push_back(xS + " " + yS); //push line 1
    list.push_back(numFrames); //push line 2
    const QChar delimeter = '\n';
    for (int z = 0; z< scenes.size(); z++){
        QString arst[128][128];
        std::unordered_map<std::string, QColor> temp = scenes.at(z)->currentState;
        for (std::pair<std::string, QColor> element : temp)
        {
            std::string delimiter = ".";
            std::string s1  = element.first.substr(0, element.first.find(delimiter));
            std::string s2  = element.first.substr(element.first.find(delimiter)+1, element.first.size());
            int x1 = std::stoi(s1);
            int y1 = std::stoi(s2);
            QColor tempCol = element.second;
            int r = tempCol.red();
            int g = tempCol.green();
            int b = tempCol.blue();
            int a = tempCol.alpha();
            arst[x1][y1] = QString::number(r)+ " " + QString::number(g) + " " + QString::number(b) + " " + QString::number(a);
        }
        for (int x = 0; x< sizeY; x++){
            QString line;
            for (int y = 0; y < sizeX; y++){
                if(arst[x][y] == "")
                    line = line + "0 0 0 0";
                else
                    line = line + arst[x][y];
                line = line + " ";
            }
            list.push_back(line);
        }
    }
    QTextStream outstream(&file);
    outstream << list.join(delimeter) << "\n";
    //this is for demo purposes but can later be used in the export
    //QByteArray ba;
    //QBuffer buffer(&ba);
    //buffer.open(QIODevice::WriteOnly);
    //i.save(&buffer, "PNG"); // writes image into ba in PNG formal. output bool if save was succes.
    //QFile filez("a.png"); //write into the debug folder
    //filez.open(QIODevice::WriteOnly);
    //filez.write(ba);
    //filez.close();
}

void MainWindow::custom_clicked()
{
    popup.show();
}

void MainWindow::loadAction()
{
    std::unordered_map<std::string, QColor> singleMap;
    QString filename = QFileDialog::getOpenFileName();
    QFile f( filename );
    f.open(QIODevice::ReadOnly);
    QTextStream in(&f);
    QRegExp rx("(\\ )"); //RegEx split empty space ' '
    int lineCounter, numberOfFrames, heightCounter, imageCounter;
    lineCounter = 0;
    heightCounter = 0;
    imageCounter = 0;
    QString line = in.readLine();    //read the first line
    QStringList query = line.split(rx);
    QString qs = query.at(0);
    sizeY = qs.toInt();
    qs = query.at(1);
    sizeX = qs.toInt();
    line = in.readLine();     //read the 2nd line
    query = line.split(rx);
    numberOfFrames = line.toInt();
    QImage i(sizeX, sizeY, QImage::Format_ARGB32);
    QImage iCanvas(sizeX, sizeY, QImage::Format_ARGB32);
    setPixSize(sizeX, sizeY, ui->graphicsViewCanvas->height());
    imageHeight = sizeY * pixSize;
    int testCount = 0;
    while (!in.atEnd()) {
        heightCounter = lineCounter % sizeY;
        line = in.readLine();
        query = line.split(rx);
        int widthCounter = 0;
        for(int x1 = 0; x1 < sizeX; x1++) {
            int r,g,b,a;
            for (int x2 = 0; x2< 4; x2++){
                qs = query.at(x1*4+x2);
                int val = qs.toInt();
                QPoint qp;
                qp.setX(widthCounter);
                QString str;
                if (x2==0)
                    r = val;
                if (x2==1)
                    g = val;
                if (x2==2)
                    b = val;
                if (x2==3)
                    a = val;
            }
            QColor color;
            color.setRgb(r,g,b,a);
            QBrush brush(color, Qt::SolidPattern);
            i.setPixel(x1, heightCounter, brush.color().rgb());
            std::string temp = std::to_string(x1)+"."+std::to_string(heightCounter);
            singleMap.try_emplace(temp, color);
        }
        if (heightCounter == sizeY - 1){
            addFramePreview(i,sizeX,sizeY);
            scenes.at(imageCounter)->redraw(singleMap,0);
            scenes.at(imageCounter)->currentState = singleMap;
            singleMap.clear();
            imageCounter++;
        }
        if (testCount == sizeY - 1){
            iCanvas = QImage(i);
        }
        lineCounter++;
        heightCounter++;
        testCount++;
    }
    f.close();
}

void MainWindow::createCanvas(int sizex, int sizey)
{
    sizeX = sizex;
    sizeY = sizey;
    QImage dummyImage(sizex,sizey,QImage::Format_ARGB32);
    addFramePreview(dummyImage, sizex, sizey);
}

void MainWindow::addFramePreview(QImage image, int x, int y){
    Frame *gv = new Frame(x,y);
    gv->setMaximumHeight(100);
    gv->setMinimumHeight(100);
    gv->setMaximumWidth(100);
    gv->setMinimumHeight(100);
    gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    frameID = scenes.size();
    Canvas *c = new Canvas(image, x, y, frameID);//canvas now set sizeof the smallest graphicsview.
    scenes.push_back(c);
    c->drawMode = 0;
    gv->setScene(c);
    gv->setEnabled(true);
    gv->show();
    ui->frameContainer->layout()->addWidget(gv);
    connect(c, SIGNAL(clickToGV(QGraphicsSceneMouseEvent*,int)), gv, SLOT(mousePressEvent(QGraphicsSceneMouseEvent*,int)));
    connect(gv, &Frame::updateGV, this, &MainWindow::updateFocus);
    ui->graphicsViewCanvas->setScene(c);
    ui->graphicsViewCanvas->setEnabled(true);
    ui->graphicsViewCanvas->fitInView(scenes.at(animationCounter)->sceneRect(), Qt::KeepAspectRatio);
    gv->fitInView(scenes.at(animationCounter)->sceneRect(), Qt::KeepAspectRatio);
}
void MainWindow::updateFocus(int frameNum){
    ui->graphicsViewCanvas->setScene(scenes.at(frameNum));
}
void MainWindow::colorBox1Clicked(){
    Canvas::c1 = QColor(cd->getColor());
    QString temp = QString::fromStdString("background-color: "+Canvas::c1.name().toStdString()+";");
    colorHistory.append(temp);
    updateColorHistory();
    ui->color1Box->setStyleSheet(temp);
}
void MainWindow::colorBox2Clicked(){
    Canvas::c2 = QColor(cd->getColor());
    QString temp = QString::fromStdString("background-color: "+Canvas::c2.name().toStdString()+";");
    colorHistory.append(temp);
    updateColorHistory();
    ui->color2Box->setStyleSheet(temp);
}

void MainWindow::updateColorHistory(){
    for(int i = 0, j = colorHistory.size(); j > 0 && i < 14 ; i++, j--){
        colorHistoryButtons[i]->setStyleSheet(colorHistory[j - 1]);
    }
}

void MainWindow::setPixSize(int sizex, int sizey, int gvsize){
    if (sizex >= sizey){
        pixSize = gvsize/(qreal)sizex;
    }
    else if (sizey > sizex){
        pixSize = gvsize/(qreal)sizey;
    }
}


// Adds a background to the canvas at Z-layer -1 (background)
void MainWindow::addBackground(QGraphicsScene &scene) {
    // TODO
}

void MainWindow::addSurface(QGraphicsScene &scene) {
    QBrush *brush = new QBrush(QColor::fromRgbF(0.0, 0.0, 0.0, 1.0));

    for (qreal y = 0; y < canvasSize; y++) {
        for (qreal x = 0; x < canvasSize; x++) {

        }
    }
}

// Adds a grid to the canvas at Z-layer 1 (foreground)
// Interestingly, the canvas can pan very slightly regardless of size.
void MainWindow::addGrid(QGraphicsScene &scene) {
    QPen *pen = new QPen(QColor::fromRgbF(0.2, 0.2, 0.2, 0.75));
    pen->setWidthF(0.25);

    for (qreal y = 0; y < canvasSize; y++) {
        for (qreal x = 0; x < canvasSize; x++) {
            scene.addRect(pixelSize * x, pixelSize * y, pixelSize, pixelSize, *pen);
        }
    }
    scene.createItemGroup(scene.items())->setZValue(1);
}

void MainWindow::updateAnimation(){
    if (animationCounter == scenes.size())
        animationCounter = 0;
    ui->graphicsViewPreview->setScene(scenes.at(animationCounter));
    ui->graphicsViewPreview->fitInView(scenes.at(animationCounter)->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsViewPreview->update();
    animationCounter++;
}

void MainWindow::on_spinBoxSpeed_valueChanged(int arg1)
{
    //set the initial delay to 1 fps(setting 1) and increase by 1 fps for each additional speed.
    if (arg1 == 0){
        timer->stop();
    }
    else{
        speed = 1000/arg1;
        timer->start(speed);
    }

}

void MainWindow::on_penTool_clicked()
{

}

void MainWindow::on_eraseButton_clicked()
{

}

void MainWindow::on_penToolButton_clicked()
{
    Canvas::drawMode = 0;
}

void MainWindow::on_eraserToolButton_clicked()
{
    Canvas::drawMode = 1;
}

void MainWindow::on_rectangleToolButton_clicked()
{
    Canvas::drawMode = 3;
}

void MainWindow::on_ellipseToolButton_clicked()
{
    Canvas::drawMode = 2;
}

void MainWindow::on_pushButtonAddFrame_clicked()
{
    addFramePreview(dummyImage, sizeX, sizeY);
}


