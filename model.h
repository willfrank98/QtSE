#ifndef MODEL_H
#define MODEL_H

#include <frame.h>
#include <QObject>
#include <tool.h>
#include <QStack>

class Model : public QObject
{
    Q_OBJECT

public:
	explicit Model(QObject *parent = nullptr);
    void createFrame(int dimension);
    QSize frameDimensions();

signals:
    void frameUpdated(QImage);

public slots:
    void modifyFrame(QVector<QPoint>, QColor);
    void undo();
    void redo();
    void setTool(Tool _tool);
    void exit();    // a better name might be needed

private:
    bool isSaved = false;   // toggle to true when saved, make false after changes are made
    QList<Frame*> frames = QList<Frame*>();
    Frame *currentFrame;
    QImage tempImage;
    Tool selectedTool;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;

    void promptSave();


};

#endif // MODEL_H
