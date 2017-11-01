#ifndef MODEL_H
#define MODEL_H

#include <QObject>

class Model : public QObject
{
public:
	explicit Model(QObject *parent = nullptr);

signals:

public slots:
    void exit();    // a better name might be needed

private:
    bool isSaved = false;   // toggle to true when saved, make false after changes are made

    void promptSave();

};

#endif // MODEL_H
