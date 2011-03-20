#include "polkaview.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>

#include <QtGui>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(polka);

    MApplication a(argc, argv);

    MApplicationWindow w;

    MApplicationPage p;

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget;
    p.setCentralWidget( proxy );

    PolkaView *view = new PolkaView;
    view->setFixedSize( 862, 368 );

    proxy->setWidget( view );

    w.show();

    p.appear();

    return a.exec();
}
