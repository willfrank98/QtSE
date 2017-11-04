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
    void framePreview(QList<Frame*>);

public slots:
    void modifyFrame(QVector<QPoint>, QColor);
    void setTool(Tool _tool);
    void setActiveFrame(int);
    void removeFrame(int);
    void exit();    // a better name might be needed

private:
    bool isSaved = false;   // toggle to true when saved, make false after changes are made
    QList<Frame*> frames = QList<Frame*>();
    Frame *currentFrame;
    int currIndex = 0;
    Tool selectedTool;
    void promptSave();


};

#endif // MODEL_H
