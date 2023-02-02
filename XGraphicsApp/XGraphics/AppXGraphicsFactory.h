#ifndef XNODEFACTORY_H
#define XNODEFACTORY_H

#include "XGraphicsDelegateFactory.h"

const static QString SC_XItemTypes[]
{
  "A","B","C","OnlyD",
  "E1","E2"
};
class AppXGraphicsItemFactory: public XGraphicsItemDelegateFactory
{
    Q_OBJECT
public:
    AppXGraphicsItemFactory(QObject* parent=nullptr);

    // XNodeItemDelegateFactory interface
public:

    XGraphicsItem *getXGraphicsItem(const QString &type) override;

};

class AppXGraphicsLinkFactory:public XGraphicsLinkDelegateFactory
{
    Q_OBJECT
public:
    AppXGraphicsLinkFactory(QObject* parent=nullptr);

    // XGraphicsLinkDelegateFactory interface
public:
    XGraphicsConnectLink *getXGraphicsLink() override;
};

#endif // XNODEFACTORY_H
