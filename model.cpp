#include "model.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{

}

void Model::createFrame(int dimension) {
    Frame *newFrame;
//    if (frames.count() > 0) newFrame = new Frame(frames.first()->size().width());
    newFrame = new Frame(dimension);
    currentFrame = newFrame;
    frames.append(currentFrame);
    selectedTool = PenTool;
    emit frameUpdated(currentFrame->pixels());
}

void Model::modifyFrame(QVector<QPoint> pixels, QColor color) {
    currentFrame->drawPen(pixels, color);
    emit frameUpdated(currentFrame->pixels());
}

void Model::setTool(Tool tool) {
    selectedTool = tool;
}

void Model::exit() {
    if (isSaved) QApplication::exit();
    else promptSave();
}

void Model::promptSave() {
    // TODO: prompt if the user wants to save the project or something
    qDebug() << "prompt to save";
}
