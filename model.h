/*
 * Team Deathstar IT
 * CS3505 - A7: Sprite Editor
 * model.h
 *
 *
 */

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QStack>
#include <QTimer>
#include <frame.h>
#include <tool.h>

class Model : public QObject
{
    Q_OBJECT

public:
	explicit Model(QObject *parent = nullptr);
    void newSurface(int dimension);
    void createFrame();
    void dupeFrame(int index);
    void deleteFrame(int index);
    void undo();
    void redo();

signals:
    void frameCreated(int);
    void frameUpdated(Frame*);
    void previewFrame(QImage);

public slots:
    void updateUndoRedo(QImage);
    void saveAnimatedGIF(QString filename);
    void saveFrameToPNG(QString filename);
    void saveFrameSequence(QString dir);
	void saveFrameToFile(QString filename);
	void loadFrameFromFile(QString filename);
    void setPreviewFPS(int secs);
    void previewDisplay();
    void setActiveFrame(int index);
    void save();
    void load();
    void exit();    // a better name might be needed

private:
    bool isSaved = true;   // toggle to true when saved, make false after changes are made
    QList<Frame*> frames = QList<Frame*>();
    Frame *currentFrame;
    QTimer previewAnimTimer;
    int previewAnimIndex = 0;
    QImage tempImage;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;
};

#endif // MODEL_H
