#ifndef CANVAS3D_H
#define CANVAS3D_H

#include <QObject>

namespace Editor
{

    class Canvas3D : public QObject
    {
        Q_OBJECT
    public:
        explicit Canvas3D(QObject *parent = 0);

    signals:

    public slots:
    };

} // end namespace Editor

#endif // CANVAS3D_H
