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
    emit framePreview(frames);
    emit frameUpdated(currentFrame->pixels());
}

void Model::modifyFrame(QVector<QPoint> pixels, QColor color) {
    currentFrame->drawPen(pixels, color);
    emit framePreview(frames);
    //New changes clear the redo stack but not undo stack.
    if(!redoStack.isEmpty()){
        QStack<QImage> temp;
        redoStack = temp;
    }
    undoStack.push(currentFrame->pixels());
    emit frameUpdated(currentFrame->pixels());
}

void Model::undo(){
    if(!undoStack.isEmpty()){
        redoStack.push(currentFrame->pixels());
        tempImage = undoStack.pop();
        currentFrame->setPixels(tempImage);
        emit frameUpdated(currentFrame->pixels());
    }
}

void Model::redo(){
    if(!redoStack.isEmpty()){
        undoStack.push(currentFrame->pixels());
        tempImage = redoStack.pop();
        currentFrame->setPixels(tempImage);
        emit frameUpdated(currentFrame->pixels());
    }

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


void Model::setActiveFrame(int index){
    currentFrame = frames.at(index);
}
void Model::removeFrame(int index){
    frames.removeOne(frames.at(index));
}
