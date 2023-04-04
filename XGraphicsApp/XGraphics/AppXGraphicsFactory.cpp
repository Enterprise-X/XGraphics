#include "AppXGraphicsFactory.h"
#include "XGraphicsRectItem.h"
#include "XGraphicsEllipseItem.h"
#include "XGraphicsConnectLink.h"
#include "XGraphicsScene.h"
#include <QStyle>
#include <QApplication>
AppXGraphicsItemFactory::AppXGraphicsItemFactory(XGraphicsScene* scene,QObject *parent)
    :XGraphicsItemDelegateFactory{scene,parent}
{

}

XGraphicsItem *AppXGraphicsItemFactory::getXGraphicsItem(const QString &type)
{
     QPixmap pixmap1=QPixmap(":/image/Image1.svg");
     QPixmap pixmap2=QPixmap(":/image/Image2.svg");
     QPixmap pixmap3=QPixmap(":/image/Image3.svg");
     QPixmap pixmap4=QPixmap(":/image/Image4.svg");
     QPixmap pixmap5=QPixmap(":/image/Image5.svg");

    if(type=="A")
    {
        auto lst=  m_parScene->getXItemsByType("A");
        int nIdx=lst.count()+1;
        XGraphicsRectItem *item=new XGraphicsRectItem("A","",m_parScene);

        item->setText(QString(" A %1").arg(nIdx));
        item->setTip(QString("Id:%1\r\n类型:%2").arg(item->itemId()).arg(item->itemType()));

        //图标设置

        /*********/




        return item;
    }
    else if(type=="B")
    {
        auto lst=  m_parScene->getXItemsByType("B");
        int nIdx=lst.count()+1;

        XGraphicsRectItem *item=new XGraphicsRectItem("B","",m_parScene);
        item->setText(QString("B %1").arg(nIdx));
        item->setTip(QString("Id:%1\r\n类型:%2").arg(item->itemId()).arg(item->itemType()));

        //图标设置

        /*********/

        return item;
    }
    else if(type=="C")
    {
        auto lst=  m_parScene->getXItemsByType("C");
        int nIdx=lst.count()+1;

        XGraphicsRectItem *item=new XGraphicsRectItem("C","",m_parScene);
        item->setText(QString("C %1").arg(nIdx));
        item->setTip(QString("Id:%1\r\n类型:%2").arg(item->itemId()).arg(item->itemType()));

        //外观设置
     /*   auto cfg=item->config();
        cfg->rRoundedRect=0;
        cfg->brushItemRect=QColor(100,100,100);
        cfg->brushSelectRect=QBrush(Qt::NoBrush);

        auto cfgCom=item->itemConfig();
        cfgCom->penText=QColor(Qt::white);*/
        /*********/

        return item;
    }
    else if(type=="OnlyD")
    {
        XGraphicsRectItem *item=new XGraphicsRectItem("OnlyD","OnlyD",m_parScene);
        item->setText(QString("OnlyD"));
        item->setTip(QString("Id:%1\r\n类型:%2").arg(item->itemId()).arg(item->itemType()));


        return item;
    }
    else if(type=="E1")
    {
        auto lst=  m_parScene->getXItemsByType("E1");
        int nIdx=lst.count()+1;

        XGraphicsEllipseItem *item=new XGraphicsEllipseItem("E1","",m_parScene);
        item->setText(QString("E1 %1").arg(nIdx));
        item->setTip(QString("Id:%1\r\n类型:%2").arg(item->itemId()).arg(item->itemType()));

        return item;
    }
    else if(type=="E2")
    {
        auto lst=  m_parScene->getXItemsByType("E2");
        int nIdx=lst.count()+1;

        XGraphicsEllipseItem *item=new XGraphicsEllipseItem("E2","",m_parScene);
        item->setText(QString("E2 %1").arg(nIdx));
        item->setTip(QString("Id:%1\r\n类型:%2").arg(item->itemId()).arg(item->itemType()));
        item->setSize(100,100);
        //图标设置

        return item;
    }
    else
    {
        return nullptr;
    }
    return nullptr;
}

AppXGraphicsLinkFactory::AppXGraphicsLinkFactory(XGraphicsScene* scene,QObject *parent)
  :XGraphicsLinkDelegateFactory{scene,parent}
{

}

XGraphicsConnectLink *AppXGraphicsLinkFactory::getXGraphicsLink()
{

    XGraphicsConnectLink* link=new XGraphicsConnectLink();
    auto font= link->textFont();
    font.setPointSize(24);
    link->setTextFont(font);


    return link;
}
