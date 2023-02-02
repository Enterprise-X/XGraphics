#include "AppXGraphicsFactory.h"
#include "XGraphicsRectItem.h"
#include "XGraphicsEllipseItem.h"
#include "XGraphicsConnectLink.h"
#include "XGraphicsScene.h"
#include <QStyle>
#include <QApplication>
AppXGraphicsItemFactory::AppXGraphicsItemFactory(QObject *parent)
    :XGraphicsItemDelegateFactory{parent}
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
        QPen pen;
        pen.setBrush(Qt::NoBrush);
        QBrush brush=(Qt::NoBrush);

        QPixmap pix= QPixmap(":/image/Image1.svg");
        item->addPixData(new SXItemPixData("A",pix,true,pen,brush));

        brush=QBrush(Qt::red);
        item->addPixData(new SXItemPixData("B",pix,true,pen,brush));

        brush=QBrush(Qt::green);
        item->addPixData(new SXItemPixData("C",pix,true,pen,brush));

        item->switchShowPixKey("A");
        /*********/

        //外观设置
        auto cfg=item->config();
        /*
        cfg->penItemRect.setStyle(Qt::DashLine);
        cfg->penItemRect.setColor(Qt::gray);

        cfg->penSelectRect.setStyle(Qt::DashLine);
        cfg->penSelectRect.setColor(QColor(247,105,0));

        item->setWidth(200);
        item->setHeight(60);
        item->updateShape();*/
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
        QPen pen;
        pen.setBrush(Qt::NoBrush);
        QBrush brush=(Qt::NoBrush);
        item->addPixData(new SXItemPixData("Smile", QPixmap(":/image/Image4.svg"),true,pen,brush));
        item->addPixData(new SXItemPixData("Cry", QPixmap(":/image/Image5.svg"),true,pen,brush));
        item->switchShowPixKey("Smile");
        /*********/

        //外观设置
        /*auto cfg=item->config();
        cfg->rRoundedRect=0;
        cfg->penItemRect.setStyle(Qt::SolidLine);
        cfg->penItemRect.setColor(Qt::yellow);

        cfg->penSelectRect.setStyle(Qt::DashLine);
        cfg->penSelectRect.setColor(QColor(247,105,0));

        auto cfgCom=item->itemConfig();
        cfgCom->fontText= QFont("Microsoft YaHei", 15, 2);

        item->setWidth(300);
        item->setHeight(100);
        item->updateShape();*/
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

        //图标设置
        QPen pen;
        pen.setBrush(Qt::NoBrush);
        QBrush brush=(Qt::NoBrush);
        item->addPixData(new SXItemPixData("A", QPixmap(":/image/Image1.svg"),true,pen,brush));

        brush=QBrush(Qt::red);
        item->addPixData(new SXItemPixData("B", QPixmap(":/image/Image2.svg"),true,pen,brush));

        brush=QBrush(Qt::green);
        item->addPixData(new SXItemPixData("C", QPixmap(":/image/Image3.svg"),true,pen,brush));

        brush=QBrush(Qt::NoBrush);
        item->addPixData(new SXItemPixData("D", QPixmap(":/image/Image4.svg"),true,pen,brush));

        item->addPixData(new SXItemPixData("E", QPixmap(":/image/Image5.svg"),true,pen,brush));

        item->switchShowPixKey("A");

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
        item->setSize(150,100);

        //图标设置
        QPen pen;
        pen.setBrush(Qt::NoBrush);
        QBrush brush=(Qt::NoBrush);
        item->addPixData(new SXItemPixData("A", QPixmap(":/image/Image1.svg"),true,pen,brush));

        brush=QBrush(Qt::red);
        item->addPixData(new SXItemPixData("B", QPixmap(":/image/Image2.svg"),true,pen,brush));

        brush=QBrush(Qt::green);
        item->addPixData(new SXItemPixData("C", QPixmap(":/image/Image3.svg"),true,pen,brush));

        brush=QBrush(Qt::NoBrush);
        item->addPixData(new SXItemPixData("D", QPixmap(":/image/Image4.svg"),true,pen,brush));

        item->addPixData(new SXItemPixData("E", QPixmap(":/image/Image5.svg"),true,pen,brush));

        item->switchShowPixKey("A");

        return item;
    }
    else
    {
        return nullptr;
    }
    return nullptr;
}

AppXGraphicsLinkFactory::AppXGraphicsLinkFactory(QObject *parent)
  :XGraphicsLinkDelegateFactory{parent}
{

}

XGraphicsConnectLink *AppXGraphicsLinkFactory::getXGraphicsLink()
{

    XGraphicsConnectLink* link=new XGraphicsConnectLink();
    auto cfg= link->linkConfig();
    cfg->fontText.setPointSize(24);

    return link;
}
