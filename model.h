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
    void frameUpdated(QImage);

public slots:
    void modifyFrame(QVector<QPoint>, QColor);
    void exit();    // a better name might be needed

private:
    bool isSaved = false;   // toggle to true when saved, make false after changes are made
    QList<Frame*> frames = QList<Frame*>();
    Frame *currentFrame;

    void promptSave();

};

#endif // MODEL_H
