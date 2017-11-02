#ifndef MODEL_H
#define MODEL_H

#include <frame.h>
#include <QObject>
#include <tool.h>

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
    void setTool(Tool _tool);
    void exit();    // a better name might be needed

private:
    bool isSaved = false;   // toggle to true when saved, make false after changes are made
    QList<Frame*> frames = QList<Frame*>();
    Frame *currentFrame;
    Tool selectedTool;

    void promptSave();

};

#endif // MODEL_H
