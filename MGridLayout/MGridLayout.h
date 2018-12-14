#ifndef MGRIDLAYOUT_H
#define MGRIDLAYOUT_H

#include <QGridLayout>

class MGridLayout : public QGridLayout
{
    Q_OBJECT

public:
    MGridLayout(QWidget *parent = 0);
    ~MGridLayout();
    void clearAllWidget();
};

#endif // MGRIDLAYOUT_H
