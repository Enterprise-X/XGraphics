﻿#include "XGraphicsRectItem.h"
#include <QGraphicsScene>
#include <QStaticText>
#include <QGraphicsSceneMouseEvent>



/***************************构造析构***************************/
XGraphicsRectItem::XGraphicsRectItem(QObject *parent)
    :XGraphicsItem{"","",parent}
{
    initItem();
}

XGraphicsRectItem::XGraphicsRectItem(QString type,QString id,QObject *parent)
    :XGraphicsItem{type,id,parent}
{
    initItem();
}

XGraphicsRectItem::~XGraphicsRectItem()
{

}

/***************************初始化***************************/
void XGraphicsRectItem::initItem()
{
    setPos(0,0);
    m_rWidth=150;
    m_rHeight=50;
    updateShape();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptHoverEvents(true);
    initConnectArea();
}

void XGraphicsRectItem::initConnectArea()
{
     m_mapConnectRectArea.clear();
     addConnectRect("Left",m_ptLeft);
     addConnectRect("Right",m_ptRight);
     addConnectRect("Top",m_ptTop);
     addConnectRect("Bottom",m_ptBottom);
}

void XGraphicsRectItem::addConnectRect(const QString &key, const QPointF &pt)
{
    double rSize=m_config.rConnectRectSize;
    double rPos=rSize/2;
    QRectF rect = QRectF((pt - QPointF(rPos, rPos)), QSizeF(rSize, rSize));
    if(!m_mapConnectRectArea.contains(key))
    {

        m_mapConnectRectArea.insert(key,SConnectRect(key,pt,rect));
    }
}


/***************************更新***************************/
void XGraphicsRectItem::updateShape()
{
    setRect({QPointF(-m_rWidth / 2, -m_rHeight / 2), QSizeF(m_rWidth, m_rHeight)});
    QRectF rect=this->boundingRect();
    m_ptCenter = pos();
    m_ptLeft=QPointF(rect.left(),0);
    m_ptRight=QPointF(rect.right(),0);
    m_ptTop=QPointF(0,rect.top());
    m_ptBottom=QPointF(0,rect.bottom());

    initConnectArea();
    emit shapeChanged();
}

/***************************重写父类接口***************************/

QGraphicsItem *XGraphicsRectItem::item()
{
    return this;
}

bool XGraphicsRectItem::isInConnectArea(const QPointF &sPt)
{
    foreach (auto data, m_mapConnectRectArea)
    {
        auto dataScene= sconnectDataMapToScene(data.data);
        if(dataScene.area.contains(sPt))
        {
            return true;
        }
    }
    return false;
}

bool XGraphicsRectItem::hasConnectKey(const QString &key)
{
    return m_mapConnectRectArea.contains(key);
}

bool XGraphicsRectItem::getConnectData(const QString &key, SConnectData &connData)
{
      if(m_mapConnectRectArea.contains(key))
      {
          connData= sconnectDataMapToScene(m_mapConnectRectArea[key].data);
          return true;
      }
      else
      {
          return false;
      }
}

bool XGraphicsRectItem::getConnectData(const QPointF &sPt, SConnectData &connData)
{
    foreach (auto data, m_mapConnectRectArea)
    {
         auto dataScene= sconnectDataMapToScene(data.data);
         if(dataScene.area.contains(sPt))
         {
             connData=dataScene;
             return true;
         }
    }
    return false;
}



QRectF XGraphicsRectItem::boundingRect() const
{
    double rSize=m_config.rConnectRectSize;
    double r=rSize/2;
    QRectF rect = QGraphicsRectItem::boundingRect();
    rect.setRect(rect.x()+r,rect.y()+r,rect.width()-rSize,rect.height()-rSize);
    return rect;
}

void XGraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter->save();
    if(isSelected())
    {
        painter->setPen(m_config.penSelectRect);
        painter->setBrush(m_config.brushSelectRect);
    }
    else
    {
        painter->setPen(m_config.penItemRect);
        painter->setBrush(m_config.brushItemRect);
    }
    if(m_bHighlight)
    {
        painter->setPen(m_config.penHighlightRect);
        painter->setBrush(m_config.brushHighlightRect);
    }
    painter->drawRoundedRect(this->boundingRect(),m_config.rRoundedRect,m_config.rRoundedRect);
    painter->restore();

    if(isSelected())
    {
        painter->setPen(m_config.penSelectBoundingRect);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(QGraphicsRectItem::boundingRect());
    }


    double rLeftSize=0;
    if(!m_strShowPixKey.isEmpty())//需要显示图像
    {
        rLeftSize=this->boundingRect().height();
    }
    QRectF rectText = this->boundingRect();
    rectText.setRect(rectText.x()+rLeftSize,rectText.y(),rectText.width()-rLeftSize,rectText.height());
    m_ItemRectText=this->rect();

    if(!m_ItemText.isEmpty())
    {           
        drawItemText(painter,m_ItemText,rectText);
    }

    if(!m_strShowPixKey.isEmpty())
    {
       if(m_mapPixData.contains(m_strShowPixKey))
        {
             auto data=m_mapPixData[m_strShowPixKey];
             drawItemPix(painter,data,PIX_OFFSET);
        }

    }
    auto funcGetLittleRect=[&](const QRectF &rect, double scale=0.5)
    {
        QPointF center=rect.center();
        double rSize=rect.width()*scale;
        double rPos=rSize/2;

        QRectF retRect = QRectF((center - QPointF(rPos, rPos)), QSizeF(rSize, rSize));
        return retRect;
    };
    if (m_eShowConnectAreaType==EShowConnectAreaType::ShowFull)
    {
        painter->setPen(m_ItemConfig.penConnectArea);
        painter->setBrush(m_ItemConfig.brushConnectArea);

        int nCount=m_mapConnectRectArea.count();

        foreach (auto data, m_mapConnectRectArea)
        {
            if(data.data.key==m_strShowFullKey)
            {

               painter->drawRect(data.rect);
            }
            else
            {
                painter->drawRect(funcGetLittleRect(data.rect));
            }

        }

    }
    else if(m_eShowConnectAreaType==EShowConnectAreaType::ShowLittle)
    {
        painter->setPen(m_ItemConfig.penConnectArea);
        painter->setBrush(m_ItemConfig.brushConnectArea);
        foreach (auto data, m_mapConnectRectArea)
        {
            painter->drawRect(funcGetLittleRect(data.rect));
        }
    }

}

/***************************内部辅助接口***************************/
void XGraphicsRectItem::drawItemText(QPainter *painter,const QString &text,const QRectF &rect)
{
    painter->save();
    painter->setFont(m_ItemConfig.fontText);
    QFontMetrics fontMetrics = painter->fontMetrics();
    painter->setPen(m_ItemConfig.penText);
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWrapAnywhere, text);
    painter->restore();

}

void XGraphicsRectItem::drawItemPix(QPainter *painter,  SXItemPixData* data,double widthOffset)
{
    if(!data) return;
    if(!data->pixmap) return;
    if(data->pixmap.isNull()) return;
     painter->save();
     QRectF rect = this->boundingRect();
     double width=rect.height()-widthOffset;
     QRectF rectResize=QRectF(rect.x()+widthOffset/2,rect.y()+widthOffset/2,width,width);

     if(data->bShowRect)
     {
         QRectF rectShow=QRectF(rect.x(),rect.y(),rect.height(),rect.height());
         painter->setPen(data->penShowRect);
         painter->setBrush(data->brushShowRect);
         painter->drawRoundedRect(rectShow,m_config.rRoundedRect,m_config.rRoundedRect);
     }
     painter->drawPixmap(rectResize.toRect(),data->pixmap);
     painter->restore();
}

/***************************限制位置***************************/

QVariant XGraphicsRectItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange  &&  scene()) // 控件发生移动
    {
        QPointF newPos = value.toPointF(); //即将要移动的位置
        QRectF rect(0, 0, scene()->width(), scene()->height()); // 你要限制的区域
        if (!rect.contains(newPos)) // 是否在区域内
        {
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            emit posChanged();
            return newPos;
        }
        emit posChanged();
     }

    return QGraphicsRectItem::itemChange(change, value);

}


/***************************重写父类接口***************************/
//*[工具接口]*
void XGraphicsRectItem::setItemToolTip()
{
    QString tip;
    if(!m_ItemText.isEmpty())
    {
        tip+=m_ItemText;
    }
    if(!m_ItemTip.isEmpty())
    {
        if(m_ItemText.isEmpty())
        {
            tip+=m_ItemTip;
        }
        else
        {
            tip+="\r\n"+m_ItemTip;
        }

    }
    this->setToolTip(tip);
}
void XGraphicsRectItem::setText(const QString &text)
{
    XGraphicsItem::setText(text);
    setItemToolTip();
}

void XGraphicsRectItem::setTip(const QString &tip)
{
    XGraphicsItem::setTip(tip);
    setItemToolTip();
}






