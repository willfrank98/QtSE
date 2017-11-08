#include "model.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QFile>

#include <Magick++.h>
using namespace Magick;

Model::Model(QObject *parent) : QObject(parent)
{
    // Make Magick look in the current directory for the library files.
    Magick::InitializeMagick(".");

    // TODO: hook up a timer to the previewFrame signal
}

void Model::setPreviewFPS(int secs) {
    // TODO
}

void Model::setActiveFrame(int index) {
    qDebug() << "hit";
    currentFrame = frames.at(index);
    emit frameUpdated(currentFrame);
}

void Model::newSurface(int dimension) {
    if (!isSaved) {
        // emit a signal that prompts to save or something
    }

    previewAnimTimer.stop();

    Frame *newFrame = new Frame(dimension);
    frames.clear();
    frames.append(newFrame);
    currentFrame = newFrame;
    emit frameCreated(frames.indexOf(currentFrame));
    emit frameUpdated(currentFrame);

    previewAnimIndex = 0;
    previewAnimTimer.start(previewAnimTimer.interval());
}

void Model::createFrame() {
    Frame *newFrame = new Frame(frames.first()->size().width());
    frames.insert(frames.indexOf(currentFrame)+1, newFrame);
    currentFrame = newFrame;
    emit frameCreated(frames.indexOf(currentFrame));
    emit frameUpdated(currentFrame);
}

void Model::dupeFrame(int index) {
    Frame *newFrame = new Frame(*frames.at(index));
    frames.insert(frames.indexOf(currentFrame)+1, newFrame);
    currentFrame = newFrame;
    emit frameCreated(frames.indexOf(currentFrame));
    emit frameUpdated(currentFrame);
}

void Model::deleteFrame(int index) {
    if (index < frames.size()) frames.removeAt(index);
    if (index < frames.size() - 1) currentFrame = frames.last();
    else currentFrame = frames.at(index+1);
    emit frameUpdated(currentFrame);
}

void Model::updateUndoRedo(QImage newImage) {
    if (!redoStack.isEmpty()) {
        redoStack = QStack<QImage>();
    }
    undoStack.push(newImage);
}

void Model::undo() {
    qDebug() << undoStack;
    if (!undoStack.isEmpty()) {
        redoStack.push(currentFrame->pixels());
        tempImage = undoStack.pop();
        currentFrame->setPixels(tempImage);
        emit frameUpdated(currentFrame);
    }
}

void Model::redo() {
    qDebug() << redoStack;
    if (!redoStack.isEmpty()) {
        undoStack.push(currentFrame->pixels());
        tempImage = redoStack.pop();
        currentFrame->setPixels(tempImage);
        emit frameUpdated(currentFrame);
    }
}

void Model::saveAnimatedGIF(QString filename) {
    if (!filename.toLower().endsWith(".gif")) filename.append(".gif");

    QString tempFile = QString(filename).replace(".gif", ".png");
    QList<Image> newFrames;
    for (int i = 0; i < frames.size(); i++) {
        Magick::Image f;
        frames.at(i)->pixels().save(tempFile);
        f.read(tempFile.toStdString());
        f.animationDelay(previewAnimTimer.interval() / 10);
        f.gifDisposeMethod(3);  // disposes previous frame
        newFrames.push_back(f);
        QFile(tempFile).remove();
    }
    writeImages(newFrames.begin(), newFrames.end(), filename.toStdString());
}

void Model::saveFrameToPNG(QString filename) {
    if (!filename.toLower().endsWith(".png")) filename.append(".png");
    currentFrame->pixels().save(filename);
}

void Model::saveFrameSequence(QString dir) {
    // TODO
}

void Model::exit() {
    if (isSaved) {
        QApplication::exit();
    }
    else {
        // emit a signal that triggers a dialog that asks if the user would like to save (or something)
    }
}

void Model::save() {
    // TODO
}

void Model::load() {
    // TODO
}
