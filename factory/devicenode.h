#ifndef DEVICENODE_H
#define DEVICENODE_H


#include "basenode.h"

#include <QObject>

class DeviceNode : public BaseNode
{
    Q_OBJECT
public:
    explicit DeviceNode(QString filePath,QObject *parent = 0);
    explicit DeviceNode(QObject *parent = 0);
signals:
    
public slots:
    
};

#endif // DEVICENODE_H
