#ifndef MODEL_H
#define MODEL_H
#include "frame.h"
#include <QObject>

class Model : public QObject
{
public:
    std::vector<Frame> frames;
	explicit Model(QObject *parent = nullptr);


signals:

public slots:
    void exit();    // a better name might be needed
    //void saveFramesToFile(QString fileName);
    //void loadFramesFromFile(QString fileName);
    void addFrame(Frame f);

private:
    bool isSaved = false;   // toggle to true when saved, make false after changes are made

    void promptSave();

};

#endif // MODEL_H
