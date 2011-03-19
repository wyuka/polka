#include "polkaview.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>

#include <QtGui>

int main(int argc, char *argv[])
{
    MApplication a(argc, argv);

    MApplicationWindow w;

    MApplicationPage p;

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    p.setCentralWidget( proxy );

    PolkaView *view = new PolkaView;

    proxy->setWidget( view );

    w.show();

    p.appear();

    return a.exec();
}
