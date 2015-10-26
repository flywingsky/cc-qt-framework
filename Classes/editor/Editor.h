#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <Singleton.h>

namespace Editor
{

class Editor : public QObject, public Singleton<Editor>
{
    Q_OBJECT
public:
    explicit Editor(QObject *parent = 0);
    ~Editor();

    void testProperty();

signals:

public slots:
};

}// end namespace Editor

#endif // EDITOR_H
