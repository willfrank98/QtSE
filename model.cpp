#include "model.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>

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

//Converts current frames into some kinda of portable (text) file
void Model::saveFrames(QString fileName)
{
	//QFileDialog dialog;

}

//Converts above text file back into editable frames
void Model::loadFramesAction()
{

}
