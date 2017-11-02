#include "model.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{

}

void Model::createFrame(int dimension) {
    Frame *newFrame;
    if (frames.count() > 0) newFrame = new Frame(frames.first()->size().width());
    else newFrame = new Frame(dimension);
    newFrame->drawEllipse(0, 0, 20, 20);
    frames.insert(++currentFrame, newFrame);
    emit frameCreated(newFrame->pixels());
}

void Model::exit() {
    if (isSaved) QApplication::exit();
    else promptSave();
}

void Model::promptSave() {
    // TODO: prompt if the user wants to save the project or something
    qDebug() << "prompt to save";
}
