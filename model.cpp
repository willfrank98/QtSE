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
    previewAnimTimer.setInterval(200);
    connect(&previewAnimTimer, SIGNAL(timeout()), this, SLOT(previewDisplay()));
    previewAnimTimer.start();
}

void Model::previewDisplay(){
    emit previewFrame(frames.at(previewAnimIndex)->pixels());
    if(previewAnimIndex + 1 == frames.size()){
        previewAnimIndex = 0;
    }
    else{
        previewAnimIndex++;
    }
}

void Model::setPreviewFPS(int secs) {
    // TODO
    previewAnimTimer.setInterval(1000/secs);
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
    currentFrame->pixels().save(filename);
}

void Model::saveFrameToFile(QString filename)
{
	if (!filename.toLower().endsWith(".ssp"))
	{
		filename.append(".ssp");
	}

	QFile file(filename);
	file.open(QFile::WriteOnly);
	QTextStream outstream(&file);

	int sizeX = currentFrame->size().rwidth();
	int sizeY = currentFrame->size().rheight();
	outstream << sizeX << " " << sizeY << endl; //prints x and y dimensions

	outstream << frames.size() << endl; //prints number of frames

	for (int i = 0; i < frames.size(); i++)
	{
		QImage frame = frames.at(i)->pixels();

		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				QRgb pixel = frame.pixel(x, y);
				outstream << qRed(pixel) << " " << qGreen(pixel) << " " << qBlue(pixel) << " " << qAlpha(pixel) << " ";
			}
			outstream << endl;
		}

//		QVector<QRgb> pixels = frame.colorTable(); //returns a list of all pixels as a vector of QRgb's

//		foreach (QRgb pixel, pixels)
//		{
//			outstream << qRed(pixel) << " " << qGreen(pixel) << " " << qBlue(pixel) << " " << qAlpha(pixel) << " ";
//			currentPixel++;
//			if (currentPixel%sizeX == 0)
//			{
//				outstream << endl;
//			}
//		}
	}

	file.close();
}

void Model::loadFrameFromFile(QString filename)
{
//	QFile f(filename);
//	f.open(QIODevice::ReadOnly);
//	QTextStream in(&f);
//	QRegExp rx("(\\ )"); //RegEx split empty space ' '
//	int lineCounter, numberOfFrames, heightCounter, imageCounter;
//	lineCounter = 0;
//	heightCounter = 0;
//	imageCounter = 0;
//	QString line = in.readLine();    //read the first line
//	QStringList query = line.split(rx);
//	QString qs = query.at(0);
//	int sizeY = qs.toInt();
//	qs = query.at(1);
//	int sizeX = qs.toInt();
//	line = in.readLine();     //read the 2nd line
//	query = line.split(rx);
//	numberOfFrames = line.toInt();
//	QImage i(sizeX, sizeY, QImage::Format_ARGB32);
//	QImage iCanvas(sizeX, sizeY, QImage::Format_ARGB32);
//	setPixSize(sizeX, sizeY, ui->graphicsViewCanvas->height());
//	imageHeight = sizeY * pixSize;
//	int testCount = 0;
//	while (!in.atEnd())
//	{
//		heightCounter = lineCounter % sizeY;
//		line = in.readLine();
//		query = line.split(rx);
//		int widthCounter = 0;
//		for(int x1 = 0; x1 < sizeX; x1++) {
//			int r,g,b,a;
//			for (int x2 = 0; x2< 4; x2++){
//				qs = query.at(x1*4+x2);
//				int val = qs.toInt();
//				QPoint qp;
//				qp.setX(widthCounter);
//				QString str;
//				if (x2==0)
//					r = val;
//				if (x2==1)
//					g = val;
//				if (x2==2)
//					b = val;
//				if (x2==3)
//					a = val;
//			}
//			QColor color;
//			color.setRgb(r,g,b,a);
//			QBrush brush(color, Qt::SolidPattern);
//			i.setPixel(x1, heightCounter, brush.color().rgb());
//			std::string temp = std::to_string(x1)+"."+std::to_string(heightCounter);
//			singleMap.try_emplace(temp, color);
//		}
//		if (heightCounter == sizeY - 1)
//		{
//			addFramePreview(i,sizeX,sizeY);
//			scenes.at(imageCounter)->redraw(singleMap,0);
//			scenes.at(imageCounter)->currentState = singleMap;
//			singleMap.clear();
//			imageCounter++;
//		}
//		if (testCount == sizeY - 1)
//		{
//			iCanvas = QImage(i);
//		}
//		lineCounter++;
//		heightCounter++;
//		testCount++;
//	}
//	f.close();
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
