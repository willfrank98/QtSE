#include "model.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>

Model::Model(QObject *parent) : QObject(parent)
{

}

void Model::addFrame(Frame f){
    //frames.push_back(f);
}

void Model::exit() {
    if (isSaved) QApplication::exit();
    else promptSave();
}

void Model::promptSave() {
    // TODO: prompt if the user wants to save the project or something
    qDebug() << "prompt to save";
}


