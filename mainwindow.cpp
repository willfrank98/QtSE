/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * mainwindow.cpp
 *
 *
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QVector>
#include <QShortcut>
#include <QFileDialog>

MainWindow::MainWindow(Model &model, QWidget *parent) :
	QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    this->_model = &model;

    // setup the main graphics view
    _ui->graphicsViewCanvas->setScene(_canvas);
    _ui->graphicsViewCanvas->scene()->setSceneRect(_ui->graphicsViewCanvas->rect());  // scales the canvas to the QGraphicsView
    _ui->graphicsViewCanvas->setEnabled(true);

    QColorDialog *colorPicker1 = new QColorDialog();
    QColorDialog *colorPicker2 = new QColorDialog();
    colorPicker1->setOption(QColorDialog::ShowAlphaChannel, true);
    colorPicker2->setOption(QColorDialog::ShowAlphaChannel, true);

    // adds palette buttons to a list (ordered)
    for (int i = 0; i < _ui->paletteButtons->buttons().count(); i++) {
        QString name = QString("palette"+QString::number(i + 1));
        QToolButton *button = _ui->frameToolsAndPalette->findChild<QToolButton*>(name);
        _paletteHistory.append(QColor(255, 255, 255, 255));

        connect(button, &QToolButton::clicked, this, [=]() {
            _canvas->setPrimaryColor(_paletteHistory.at(i));
            _ui->color1Box->setStyleSheet(colorToString(_paletteHistory.at(i)));
        });

        _paletteButtons.append(button);
    }

    // Connects primary and secondary color boxes, palette history, and other color things
    connect(_ui->color1Box, &QToolButton::clicked, _canvas, [=](){ colorPicker1->show(); });
    connect(colorPicker1, &QColorDialog::colorSelected, _canvas, &Canvas::setPrimaryColor);
    connect(_ui->color2Box, &QToolButton::clicked, _canvas, [=](){ colorPicker2->show(); });
    connect(colorPicker2, &QColorDialog::colorSelected, _canvas, &Canvas::setSecondaryColor);
    connect(colorPicker1, &QColorDialog::colorSelected, _ui->color1Box, [=](QColor color){
        QString newStyle = colorToString(color);
        _ui->color1Box->setStyleSheet(newStyle);
        if (_paletteHistory.size() == _ui->paletteButtons->buttons().count()) _paletteHistory.replace(_paletteHistoryIndex, color);
        else _paletteHistory.insert(_paletteHistoryIndex, color);
        updatePaletteHistory();
    });
    connect(colorPicker2, &QColorDialog::colorSelected, _ui->color2Box, [=](QColor color){
        QString newStyle = colorToString(color);
        _ui->color2Box->setStyleSheet(newStyle);
        if (_paletteHistory.size() == _ui->paletteButtons->buttons().count()) _paletteHistory.replace(_paletteHistoryIndex, color);
        else _paletteHistory.insert(_paletteHistoryIndex, color);
        updatePaletteHistory();
    });
    connect(_ui->swapColors, &QToolButton::clicked, this, [=](){
        QString temp = _ui->color1Box->styleSheet();
        _ui->color1Box->setStyleSheet(_ui->color2Box->styleSheet());
        _ui->color2Box->setStyleSheet(temp);
        _canvas->swapColors();
    });

    // Connects the toolButtons
    connect(_ui->penToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(PenTool); });
    connect(_ui->mirrorPenToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(MirrorPenTool); });
    connect(_ui->eraserToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(EraserTool); });
    connect(_ui->ditheringPenToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(DitheringTool); });
    connect(_ui->rectangleToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(RectangleTool); });
    connect(_ui->bucketPenToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(BucketFillTool); });
    connect(_ui->colorFillToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(ColorFillTool); });
    connect(_ui->ellipseToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(EllipseTool); });
    connect(_ui->rectangularSelectionToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(RectSelectTool); });
    connect(_ui->lineToolButton, &QToolButton::clicked, _canvas, [=](){ _canvas->setTool(LineTool); });

    // Connects the undo/redo actions
    connect(_ui->actionUndo, &QAction::triggered, &model, &Model::undo);
    connect(_ui->actionRedo, &QAction::triggered, &model, &Model::redo);
    connect(_canvas, &Canvas::pixelsModified, &model, &Model::updateUndoRedo);
    connect(&model, &Model::frameUpdated, _canvas, &Canvas::setFrame);

    // Connects the File>New actions
    connect(_ui->action8x8, &QAction::triggered, this, [=](){ newCanvas(8); });
    connect(_ui->action16x16, &QAction::triggered, this, [=](){ newCanvas(16); });
    connect(_ui->action32x32, &QAction::triggered, this, [=](){ newCanvas(32); });
    connect(_ui->action64x64, &QAction::triggered, this, [=](){ newCanvas(64); });

    // Connects the drawing surfaces (main drawing area, preview area, FPS, etc.)
    connect(_ui->spinBoxSpeed, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &model, &Model::setPreviewFPS);
    connect(&model, &Model::frameUpdated, _canvas, &Canvas::setFrame);
    connect(&model, &Model::previewFrame, this, [=](QImage image) {
        // TODO: do something with the image from previewFrame
        _ui->labelPreview->setPixmap(QPixmap::fromImage(image.scaled(_ui->labelPreview->size())));
    });
    connect(_ui->zoomLevelCheckbox, &QCheckBox::toggled, this, [=](bool toggled){
        // TODO: handle the zoom toggle
    });
    connect(_canvas, &Canvas::frameUpdated, this, [=](Frame *frame){
        QLabel *l = _frameButtons.checkedButton()->parent()->findChild<QLabel *>("view");
        l->setPixmap(QPixmap::fromImage(frame->pixels().scaled(l->size())));
    });

    // connects the File>Export actions
    connect(_ui->actionCurrentFrame, &QAction::triggered, this, [=]() {
        QString filename = QFileDialog::getSaveFileName(this, "Export to .PNG", "./", "PNG Files (.png)");
        this->_model->saveFrameToPNG(filename);
    });
    connect(_ui->actionAnimated_GIF, &QAction::triggered, &model, [=](){
        QString filename = QFileDialog::getSaveFileName(this, "Export to animated .GIF", "./", "GIF Files (.gif)");
        this->_model->saveAnimatedGIF(filename);
    });

    // connects the File>Exit action
    connect(_ui->actionExit, &QAction::triggered, &model, &Model::exit);

    connect(_ui->pushButtonAddFrame, &QPushButton::clicked, &model, &Model::createFrame);
    connect(&model, &Model::frameCreated, this, [=](int i){ newFrame(i); });

	//	connects File>Save and >Load
        connect(_ui->actionSave, &QAction::triggered, this, [=]() {
			QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "./", tr("Sprites (.ssp)"));
            this->_model->saveFrameToFile(filename);
		});
        connect(_ui->actionLoad, &QAction::triggered, this, [=]() {
			QString filename = QFileDialog::getOpenFileName(this, tr("Load File"), "./", tr("Sprites (.ssp)"));
            this->_model->loadFrameFromFile(filename);
		});

    // Connects the Shortcut Keys
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+1"), _ui->centralWidget);
    connect(shortcut, SIGNAL(activated()), _ui->penToolButton, SLOT(clicked()));

    // I kind of like these one-liner shortcut things better, but we can use whatever
//    ui->penToolButton->setShortcut(Qt::CTRL | Qt::Key_1);
//    ui->actionUndo->setShortcut(Qt::CTRL | Qt::Key_Z);
//    ui->actionRedo->setShortcut(Qt::CTRL | Qt::Key_R);

    model.newSurface(32);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

QString MainWindow::colorToString(QColor color) {
    return "background-color: rgb(" +
            QString::number(color.red()) + "," +
            QString::number(color.green()) + "," +
            QString::number(color.blue()) + ");";
}

void MainWindow::updatePaletteHistory() {
    QString newStyle;
    newStyle = colorToString(_paletteHistory.at(_paletteHistoryIndex));
    _paletteButtons.at(_paletteHistoryIndex)->setStyleSheet(newStyle);
    _paletteHistoryIndex++;

    if (_paletteHistoryIndex == _ui->paletteButtons->buttons().count()) _paletteHistoryIndex = 0;
}

// These signals and slots are getting ridiculous.
void MainWindow::newCanvas(int dimension) {
    emit resetCanvas();
    this->disconnect();
    _model->newSurface(dimension);
}

void MainWindow::newFrame(int index) {
    QFrame *newFrame = new QFrame();
    newFrame->setGeometry(0, 0, 75, 75);
    newFrame->setMinimumWidth(75);
    newFrame->setMinimumHeight(75);
    newFrame->setMaximumWidth(75);
    newFrame->setMaximumHeight(75);
    connect(this, &MainWindow::resetCanvas, this, [=]() {
        newFrame->hide();
        _ui->frameContainer->layout()->removeWidget(newFrame);
        newFrame->setParent(nullptr);
        delete newFrame;
    });

    QLabel *framePreview = new QLabel();
    framePreview->setGeometry(0, 0, 75, 75);
    framePreview->setObjectName("view");
    framePreview->setMinimumWidth(75);
    framePreview->setMinimumHeight(75);
    framePreview->setMaximumWidth(75);
    framePreview->setMaximumHeight(75);
    framePreview->setAttribute(Qt::WA_TransparentForMouseEvents);

    QPushButton *frameSelected = new QPushButton(newFrame);
    frameSelected->setGeometry(0, 0, 75, 75);
    frameSelected->setObjectName("button");
    frameSelected->setCheckable(true);
    _frameButtons.addButton(frameSelected, index);
    connect(frameSelected, &QPushButton::clicked, _model, [=](){
        int frameNum = _ui->frameContainer->layout()->indexOf(newFrame);
        _model->setActiveFrame(frameNum);
    });
    framePreview->setParent(newFrame);

    QToolButton *dupeFrame = new QToolButton(newFrame);
    dupeFrame->setGeometry(55, 55, 20, 20);
    dupeFrame->setAutoRaise(true);
    dupeFrame->setObjectName("dupe");
    dupeFrame->setText("D");
    connect(dupeFrame, &QToolButton::clicked, _model, [=](){
        int frameNum = _ui->frameContainer->layout()->indexOf(newFrame);
        frameSelected->setChecked(true);
        _model->setActiveFrame(frameNum);
        _model->dupeFrame(_frameButtons.id(_frameButtons.checkedButton()));
    });

    QToolButton *removeFrame = new QToolButton(newFrame);
    removeFrame->setGeometry(6, 6, 13, 13);
    removeFrame->setAutoRaise(true);
    removeFrame->setObjectName("remove");
    removeFrame->setText("X");
//   connect(removeFrame, &QToolButton::clicked, model, [=](){
//       int frameNum = ui->frameContainer->layout()->indexOf(newFrame);
//        frameSelected->setChecked(true);
//        model->setActiveFrame(frameNum);
//        model->deleteFrame(frameButtons.id(frameButtons.checkedButton()));
//    });


    // remove the spacer
    _ui->frameContainer->layout()->removeItem(_ui->frameContainer->layout()->itemAt(_ui->frameContainer->layout()->count()-1));

    // add the newFrame
    _ui->frameContainer->findChild<QHBoxLayout *>("horizontalLayout")->insertWidget(index, newFrame);
    newFrame->findChild<QPushButton *>("button")->setChecked(true);

    // re-add a horizontal spacer
    _ui->frameContainer->layout()->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));
}
