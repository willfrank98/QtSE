/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * model.cpp
 *
 *
 */

#include "model.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QFile>


//#include <Magick++.h>
//using namespace Magick;


Model::Model(QObject *parent) : QObject(parent)
{
    // Make Magick look in the current directory for the library files.
    //Magick::InitializeMagick(".");

    // TODO: hook up a timer to the previewFrame signal
    _previewAnimTimer.setInterval(200);
    connect(&_previewAnimTimer, SIGNAL(timeout()), this, SLOT(previewDisplay()));
    _previewAnimTimer.start();
}

void Model::previewDisplay(){
    emit previewFrame(_frames.at(_previewAnimIndex)->pixels());
    if(_previewAnimIndex + 1 == _frames.size()){
        _previewAnimIndex = 0;
    }
    else{
        _previewAnimIndex++;
    }
}

void Model::setPreviewFPS(int secs) {
    // TODO
    _previewAnimTimer.setInterval(1000/secs);
}

void Model::setActiveFrame(int index) {
    qDebug() << "hit";
    _currentFrame = _frames.at(index);
    emit frameUpdated(_currentFrame);
}

void Model::newSurface(int dimension) {
    if (!_isSaved) {
        // emit a signal that prompts to save or something
    }

    _previewAnimTimer.stop();

    Frame *newFrame = new Frame(dimension);
    _frames.clear();
    _frames.append(newFrame);
    _currentFrame = newFrame;
    emit frameCreated(_frames.indexOf(_currentFrame));
    emit frameUpdated(_currentFrame);

    _previewAnimIndex = 0;
    _previewAnimTimer.start(_previewAnimTimer.interval());
}

void Model::createFrame() {
    Frame *newFrame = new Frame(_frames.first()->size().width());
    _frames.insert(_frames.indexOf(_currentFrame)+1, newFrame);
    _currentFrame = newFrame;
    emit frameCreated(_frames.indexOf(_currentFrame));
    emit frameUpdated(_currentFrame);
}

void Model::dupeFrame(int index) {
    Frame *newFrame = new Frame(*_frames.at(index));
    _frames.insert(_frames.indexOf(_currentFrame)+1, newFrame);
    _currentFrame = newFrame;
    emit frameCreated(_frames.indexOf(_currentFrame));
    emit frameUpdated(_currentFrame);
}

void Model::deleteFrame(int index) {
<<<<<<< HEAD
    if (frames.size()<=1)
    {
        return;
    }
    if (index = 1)
    {

        frames.removeAt(0);
        currentFrame = frames.first();
    }
    else
    {
        frames.removeAt(index-1);
        currentFrame = frames.at(index-2);
    }
    emit frameUpdated(currentFrame);
}

void Model::updateUndoRedo(QImage newImage) {
    if (!_redoStack.isEmpty()) {
        _redoStack = QStack<QImage>();
    }
    _undoStack.push(newImage);
}

void Model::undo() {
    qDebug() << _undoStack;
    if (!_undoStack.isEmpty()) {
        _redoStack.push(_currentFrame->pixels());
        _tempImage = _undoStack.pop();
        _currentFrame->setPixels(_tempImage);
        emit frameUpdated(_currentFrame);
    }
}

void Model::redo() {
    qDebug() << _redoStack;
    if (!_redoStack.isEmpty()) {
        _undoStack.push(_currentFrame->pixels());
        _tempImage = _redoStack.pop();
        _currentFrame->setPixels(_tempImage);
        emit frameUpdated(_currentFrame);
    }
}

void Model::saveAnimatedGIF(QString filename) {
    /*
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
    */
}

void Model::saveFrameToPNG(QString filename) {
    if (!filename.toLower().endsWith(".png")) filename.append(".png");
    _currentFrame->pixels().save(filename);
}

void Model::saveToFile(QString filename)
{
	if (!filename.toLower().endsWith(".ssp"))
	{
		filename.append(".ssp");
	}

	QFile file(filename);
	file.open(QFile::WriteOnly);
    QTextStream outstream(&file);

    int sizeX = _currentFrame->size().rwidth();
    int sizeY = _currentFrame->size().rheight();
	outstream << sizeX << " " << sizeY << endl; //prints x and y dimensions

    outstream << _frames.size() << endl; //prints number of frames

    for (int i = 0; i < _frames.size(); i++)
	{
        QImage frame = _frames.at(i)->pixels();

		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				QRgb pixel = frame.pixel(x, y);
                outstream << qRed(pixel) << " " << qGreen(pixel) << " " << qBlue(pixel) << " " << qAlpha(pixel) << " ";
			}
            outstream << endl;
		}
	}

	file.close();
}

void Model::loadFromFile(QString filename)
{
    QFile f(filename);
	f.open(QIODevice::ReadOnly);
	QTextStream in(&f);

	QList<int> list;
	while (!in.atEnd())
	{
		int x;
		in >> x;
		list.append(x);
	}

	int sizeX = list.at(0);
	int sizeY = list.at(1);
	int frames = list.at(2);

	newSurface(sizeX);

	int listIter = 3;
	for (int f = 1; f <= frames; f++)
	{
		QImage tempImage;
		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				//reads in the next 4 values and creates a color
				QColor color(list.at(listIter++), list.at(listIter++), list.at(listIter++), list.at(listIter++));
                tempImage.setPixel(x, y, color.rgba());
			}
		}
        _currentFrame->setPixels(tempImage);

		//if necessary creates a new frame
		if (f < frames)
		{
			createFrame();
		}
	}

	f.close();
}

void Model::saveFrameSequence(QString dir) {
    // TODO
}

void Model::exit() {
    if (_isSaved) {
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
