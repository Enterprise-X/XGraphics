#ifndef U_XMLHELPER_H
#define U_XMLHELPER_H
#include <QObject>
#include <QtXml>
#include <QVariant>
#include "XGraphicsScene.h"
#include "AppXGraphicsFactory.h"
#include "XGraphicsView.h"
#include "XGraphicsEllipseItem.h"
#include "XGraphicsRectItem.h"
#pragma region 工具函数{

/*================保存操作================*/
///创建一个Xml元素节点
/// @param
/// doc:Xml文档
/// parEle:父节点
/// tagName:节点Tag名称
inline static QDomElement createXmlElement(QDomDocument &doc,QDomElement& parEle,const QString &tagName)
{
    QDomElement xmlEle= doc.createElement(tagName);
    parEle.appendChild(xmlEle);
    return xmlEle;
}

///在XML元素节点中添加一个属性
/// @param
/// xmlEle:需要添加的元素节点
/// name:属性名
/// var:属性值
inline static void addAttribute(QDomElement& xmlEle,const QString &name,const QVariant &var)
{
     xmlEle.setAttribute(name,var.toString());
}

///在XML元素节点中添加一个文本子节点
/// @param
/// doc:Xml文档
/// parEle:父节点
/// name:TextNode名称
/// var:节点值
/// @return
/// 添加的文本子节点
inline static QDomElement addTextNode(QDomDocument &doc,QDomElement &parEle,const QString &name,const QVariant &var)
{
    QDomElement textEle = doc.createElement(name);
    QDomText textNode = doc.createTextNode(var.toString());
    textEle.appendChild(textNode);
    parEle.appendChild(textEle);
    return textEle;
}

/*================读取操作================*/

///获取属性
/// @param
/// ele:需要获取属性的节点
/// name:属性名
/// @return
/// template T:进行转换的泛型
template<typename T>
inline static T getAttribute(QDomElement &ele,QString name)
{
    QVariant var=ele.attribute(name);
    return var.value<T>();
}

///获取子元素节点文本
/// @param
/// ele:需要获取文本子节点的节点
/// subTagName:子节点Tag名称
/// @return
/// template T:进行转换的泛型
template<typename T>
inline static T getTextNode(QDomElement &ele,QString subTagName)
{
    QDomElement subEle=ele.firstChildElement(subTagName);
    QVariant var(subEle.text());
    return var.value<T>();
}

#pragma endregion}

inline static bool saveScene(QString path,XGraphicsScene *scene)
{
    if(!scene) return false;
    qSetGlobalQHashSeed(0);//为了使 通过QDomElement保存节点属性时， 每次保存的属性的顺序固定，与qSetGlobalQHashSeed(-1);搭配着用
    QDomDocument     dom;
    QDomElement root = dom.documentElement();
    if(root.isNull())
    {
        root = dom.createElement(scene->objectName());
    }
    dom.appendChild(root);

    QDomElement eleXItems=dom.createElement("XItems");
    root.appendChild(eleXItems);


    QDomElement eleXLinks=dom.createElement("XLinks");
    root.appendChild(eleXLinks);

    foreach (auto xItem, scene->getXItems())
    {
        QDomElement eleXItem= dom.createElement("XItem");
        addAttribute(eleXItem,"Id",xItem->itemId());
        addAttribute(eleXItem,"Type",xItem->itemType());
        auto item=xItem->item();
        if(item)
        {
           addTextNode(dom,eleXItem,"Pos_X",item->pos().x());
           addTextNode(dom,eleXItem,"Pos_Y",item->pos().y());
        }
        addTextNode(dom,eleXItem,"Text",xItem->text());
        addTextNode(dom,eleXItem,"PixKey",xItem->showPixKey());

        eleXItems.appendChild(eleXItem);
    }


    foreach (auto xLink, scene->getXLinks())
    {
        QDomElement eleXLink= dom.createElement("XLink");
        addTextNode(dom,eleXLink,"StartXItem_Id",xLink->fatherXItem()->itemId());
        addTextNode(dom,eleXLink,"EndXItem_Id",xLink->sonXItem()->itemId());
        addTextNode(dom,eleXLink,"StartXItem_Key",xLink->fatherKey());
        addTextNode(dom,eleXLink,"EndXItem_Key",xLink->sonKey());
        addTextNode(dom,eleXLink,"Text",xLink->text());
        eleXLinks.appendChild(eleXLink);
    }

    QFile file(path);
    if (!file.open(QFile::WriteOnly|QFile::Truncate | QFile::Text))//1.QFile::WriteOnly如果文件不存在，则创建；2.QFile::Truncate打开的同时清空
    {
        return false;
    }
    QTextStream stream( &file );
    stream.setEncoding(QStringConverter::Utf8);
    dom.save(stream,4,QDomNode::EncodingFromTextStream);
    file.close();
    qSetGlobalQHashSeed(-1);
    return true;
}

inline static bool loadScene(QString path,XGraphicsScene *scene)
{
    if(!scene) return false;
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        return false;
    }
    QDomDocument dom;
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!dom.setContent(&file, false, &errorStr, &errorLine,&errorColumn))
    {
        return false;
    }
    file.close();

    QDomElement root = dom.documentElement();
    if(root.isNull())
    {
       return false;
    }

    QDomElement eleXItems= root.firstChildElement("XItems");
    if (eleXItems.isNull())
    {
      return false;
    }
    QDomElement eleXLinks= root.firstChildElement("XLinks");
    if (eleXLinks.isNull())
    {
      return false;
    }
    QMap<QString,XGraphicsItem*> mapXItems;
    QList<XGraphicsConnectLink*> lstXLinks;

    AppXGraphicsItemFactory* xItemfactory=new AppXGraphicsItemFactory(scene);
    xItemfactory->setScene(scene);
    AppXGraphicsLinkFactory* xLinkfactory=new AppXGraphicsLinkFactory(scene);
    xLinkfactory->setScene(scene);
    int nCount=eleXItems.childNodes().count();
    for (int i = 0; i < nCount; ++i)
    {
        QDomElement eleXItem=eleXItems.childNodes().at(i).toElement();
        QString id=getAttribute<QString>(eleXItem,"Id");
        QString type=getAttribute<QString>(eleXItem,"Type");
        QString text=getTextNode<QString>(eleXItem,"Text");
        QString pixKey=getTextNode<QString>(eleXItem,"PixKey");
        double posX=getTextNode<double>(eleXItem,"Pos_X");
        double posY=getTextNode<double>(eleXItem,"Pos_Y");
        auto xItem= xItemfactory->getXGraphicsItem(type);
        if(!xItem)
        {
            continue;
        }
        xItem->setItemId(id);
        xItem->setText(text);
        xItem->setTip(QString("Id:%1\r\n类型:%2").arg(xItem->itemId()).arg(xItem->itemType()));
        xItem->switchShowPixKey(pixKey,"");
        auto item=xItem->item();
        if(item)
        {
            item->setPos(posX,posY);
        }
        mapXItems.insert(xItem->itemId(),xItem);
    }
    nCount=eleXLinks.childNodes().count();
    for (int i = 0; i < nCount; ++i)
    {
        QDomElement eleXLink=eleXLinks.childNodes().at(i).toElement();
        QString startId=getTextNode<QString>(eleXLink,"StartXItem_Id");
        QString endId=getTextNode<QString>(eleXLink,"EndXItem_Id");
        QString startKey=getTextNode<QString>(eleXLink,"StartXItem_Key");
        QString endKey=getTextNode<QString>(eleXLink,"EndXItem_Key");
        QString text=getTextNode<QString>(eleXLink,"Text");
        auto startItem=mapXItems[startId];
        auto endItem=mapXItems[endId];
        if(!startItem||!endItem)
        {
            continue;
        }
        if(!startItem->hasConnectKey(startKey)||
           !endItem->hasConnectKey(endKey))
        {
            continue;
        }
        auto xLink=xLinkfactory->getXGraphicsLink();
        xLink->setFatherXItemKey(startItem,startKey);
        xLink->setSonXItemKey(endItem,endKey);
        xLink->setLinkState(true);
        xLink->setText(text);
        lstXLinks.append(xLink);
    }

    xItemfactory->deleteLater();
    xLinkfactory->deleteLater();

    scene->clearXItemAndXLink();
    foreach (auto xItem, mapXItems)
    {
        scene->addXItem(xItem);
    }
    foreach (auto xLink, lstXLinks)
    {
        scene->addXLink(xLink);
        xLink->onXItemUpdate();
    }

    return true;
}
#endif // U_XMLHELPER_H
