#include "model.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{

}

void Model::exit() {
    if (isSaved) QApplication::exit();
    else promptSave();
}

void Model::promptSave() {
    // TODO: prompt if the user wants to save the project or something
    qDebug() << "prompt to save";
}
