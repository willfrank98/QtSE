#ifndef MODEL_H
#define MODEL_H

#include <frame.h>
#include <QObject>

class Model : public QObject
{
    Q_OBJECT

public:
	explicit Model(QObject *parent = nullptr);
    void createFrame(int dimension);
    QSize frameDimensions();

signals:
    void frameCreated(QImage);

public slots:
    void exit();    // a better name might be needed

private:
    bool isSaved = false;   // toggle to true when saved, make false after changes are made
    QList<Frame*> frames = QList<Frame*>();
    int currentFrame = 0;

    void promptSave();

};

#endif // MODEL_H
