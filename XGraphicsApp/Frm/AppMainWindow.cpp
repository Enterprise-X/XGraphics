#include "AppMainWindow.h"
#include "./ui_AppMainWindow.h"

#include <QDrag>
#include <QMimeData>
#include <QScrollBar>
#include "XGraphicsRectItem.h"
#include "XGraphicsItem.h"
#include "XGraphicsMimeData.h"
#include "XGraphicsUtils.h"
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>
#include "U_XmlHelper.h"

#pragma region 工具接口{
///打印子节点
inline static void printfSon(AppMainWindow *appFrm, XGraphicsItem* xItem,XGraphicsItem* startItem)
{
    if(!xItem) return;
    if(!startItem) return;
    auto outs=xItem->getSonConnectLinks();
    QString str= xItem->text()+"    Son/Outs:"+QString::number(outs.count());
    if(appFrm)
    {
       appFrm->addInfo(str);
    }
    foreach (auto out, outs)
    {
        auto iItem=out->sonXItem();
        if(iItem)
        {
            if(startItem==iItem) continue;
            printfSon(appFrm,iItem,startItem);
        }
    }
}
///打印父节点
inline static void printfFather(AppMainWindow *appFrm,XGraphicsItem* xItem,XGraphicsItem* startItem)
{
    if(!xItem) return;
    if(!startItem) return;
    auto ins=xItem->getFatherConnectLinks();
    QString str= xItem->text()+"    Father/Ins:"+QString::number(ins.count());
    if(appFrm)
    {
        appFrm->addInfo(str);
    }
    foreach (auto in, ins)
    {
        auto iItem=in->fatherXItem();
        if(iItem)
        {
            if(startItem==iItem) continue;
            printfFather(appFrm,iItem,startItem);
        }
    }
}

#pragma endregion}


AppMainWindow::AppMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AppMainWindow)
{
    ui->setupUi(this);
    setWindowTitle("XGraphics测试");
    initFrm();
}

AppMainWindow::~AppMainWindow()
{
    delete ui;
}


#define USE_FACTORY

void AppMainWindow::initFrm()
{
    initCtrl();
    initXItemBtn();
    initSceneSlots();
}

void AppMainWindow::initCtrl()
{
    ///Scene初始化
    m_Scene = new XGraphicsScene();
    m_Scene->setObjectName("XScene");
    m_Scene->setDisplayRect(0,0,4000,4000);
    m_Scene->getView()->centerOn(0,0);
    m_Scene->getView()->zoomToRect(QRectF(0,0,500,500));

    //m_Scene->config()->penMagneticLine.setBrush(QColor(0,0,0));
    //   m_Scene->config()->penMagneticLine.setWidth(2);
     //  m_Scene->config()->penMagneticLine.setStyle(Qt::DashDotDotLine);
    //auto vCfg=m_Scene->getView()->config();
    //vCfg->colBackground=QColor(Qt::white);
    QVBoxLayout *layout=new QVBoxLayout(ui->wdgGraphics);
    ui->wdgGraphics->setLayout(layout);
    layout->addWidget(m_Scene->getView());

    ///编辑框初始化
    m_xTextEdit=new XGraphicsTextEdit(m_Scene->getView());
    m_xTextEdit->hide();
    connect(m_xTextEdit,&XGraphicsTextEdit::focusOut,this,&AppMainWindow::onXTextEditFocusOut);

    ///XItem工厂初始化
    m_itemFactory=new AppXGraphicsItemFactory(m_Scene);
    m_linkFactory=new AppXGraphicsLinkFactory(m_Scene);
    m_itemFactory->setScene(m_Scene);
    m_linkFactory->setScene(m_Scene);
    m_Scene->setXLinkDelegateFactory(m_linkFactory);
#ifdef USE_FACTORY
    m_Scene->setXItemDelegateFactory((XGraphicsItemDelegateFactory*)m_itemFactory);
#endif

    ///界面设置控件初始化       
    ui->ckbShowGrid->setChecked( m_Scene->getView()->showGridBig());
    ui->ckbShowGridSmall->setChecked( m_Scene->getView()->showGridSmall());
}

void AppMainWindow::initXItemBtn()
{
    auto cmb=ui->cmbXItemType;
    cmb->clear();
    for (auto str:SC_XItemTypes)
    {
        cmb->addItem("Type_"+str,str);
    }
    connect(ui->btnXItem,&QPushButton::pressed,this,&AppMainWindow::onBtnXItemAdd);
}

void AppMainWindow::initSceneSlots()
{
    connect(m_Scene,&XGraphicsScene::xLinkConnectSuccess,this,[&](XGraphicsConnectLink*  link)
    {
        if(link)
        {
            auto father=link->fatherXItem();
            auto son=link->sonXItem();
               addInfo(QString("连线成功:%1→%2").arg(father->text()).arg(son->text()));
        }

    });
    connect(m_Scene,&XGraphicsScene::sceneContextMenuRequested,this,[&](const QPoint &pos)
    {
        QPointF sPt=m_Scene->getView()->mapToScene(pos);
        auto item=m_Scene->getXItemByPos(pos);
        auto link=m_Scene->getXLinkByPos(pos);
        if(item)
        {
            QMenu menu;
            auto action=menu.addAction(QString("Item设置文本"),this,[&]()
            {
                onXItemSetText(item);
            }
            );

            action=menu.addAction(QString("切换图标"),this,[&]()
            {
                onXItemSwitchPix(item);
            }
            );

            menu.exec(QCursor::pos());
        }

        if(link)
        {
            QMenu menu;
            auto action=menu.addAction(QString("Link设置文本"),this,[&]()
            {
                onXLinkSetText(link);
            }
            );
            menu.exec(QCursor::pos());
        }


    });

    ///双击设置
    connect(m_Scene,&XGraphicsScene::mouseDoubleClickXItem,this, [&](XGraphicsItem* item)
    {
        int nMDType=ui->cmbMDoubleSet->currentIndex();
        if(nMDType==0) //重命名
        {
           onXItemSetText(item);
        }
        else if(nMDType==1)//切换图标
        {
            onXItemSwitchPix(item);

        }

    }
    );
    connect(m_Scene,&XGraphicsScene::mouseDoubleClickXLink,this,&AppMainWindow::onXLinkSetText);
    ///连接前验证
    connect(m_Scene,&XGraphicsScene::judgeCantConnectXItem,this, [this](XGraphicsItem* fatherXItem,const QString &startKey,
            XGraphicsItem* sonXItem,const QString &sonKey)
    {
        //*[防止生成环路]
        {

            if(existAncestorXItem(fatherXItem,sonXItem))
            {
                QString str=QString("%1-->%2:存在环路").arg(fatherXItem->text()).arg(sonXItem->text());
                addInfo(str);
               // return true;
            }
        }
        //*[防止重复连接子孙]*
        {
            auto sons=fatherXItem->getSonConnectLinks();
            foreach (auto son, sons)
            {
                if(son->sonXItem()==sonXItem)
                {
                    return false;
                }
            }
            if(existDescendantsXItem(fatherXItem,sonXItem))
            {
                QString str=QString("%1-->%2:存在后代节点").arg(fatherXItem->text()).arg(sonXItem->text());
                addInfo(str);
             //   return true;
            }
        }
         return false;
    });
    ///连接后验证
    connect(m_Scene,&XGraphicsScene::judgeCantConnectXLink,this, [this](XGraphicsConnectLink* link)
    {
        if(!link) return true;
        auto start=link->fatherXItem();
        auto end=link->sonXItem();
        if(!start||!end)
        {
            return true;
        }
        //*[防止同一个连接]*
        if(start->itemId()==end->itemId())
        {
            QString str=QString("%1-->%2:不可连接,为同一个item").arg(start->text()).arg(end->text());
            addInfo(str);
            return true;
        }
        //*[防止重复连接]*
        {
        auto outs=start->getSonConnectLinks();
        QList<XGraphicsConnectLink*>  lstLinked;
        foreach (auto out, outs)
        {
            auto item= out->sonXItem();
            if(item)
            {
                if(item==end)
                {
                    lstLinked.append(out);
                }
            }
        }

        if(lstLinked.count()>1)
        {
            XGraphicsConnectLink* linkOld=nullptr;
            foreach (auto out, lstLinked)
            {
                if(out->linkId()!=link->linkId())
                {
                    linkOld=out;
                    break;
                }
            }
            if(linkOld)
            {
                linkOld->setFatherKey(link->fatherKey());
                linkOld->setSonKey(link->sonKey());
            }

            QString str=QString("%1-->%2:当前连线(%3)的XItem重复连接,更新原有连线(%4)").arg(start->text()).arg(end->text()).arg(link->linkId()).arg(linkOld->linkId());
            addInfo(str);

            return true;
        }
        }


        return false;
    });
    connect(m_Scene,&XGraphicsScene::mouseClicked,this,[&](QGraphicsSceneMouseEvent* event)
     {

        if(event->button()==Qt::LeftButton)
        {
         foreach (auto link, m_Scene->getXLinks())
         {
             link->setHighlight(false,false);
         }
         foreach (auto item, m_Scene->getXItems())
         {
             item->setHighlight(false,false);
         }
        }
     }
     );
    ///单击XItem时显示连接的XItems
    connect(m_Scene,&XGraphicsScene::xItemSingleSelect,this,[this](XGraphicsItem* xItem)
    {      
       if(xItem)
       {
         auto sons=  xItem->getSonConnectLinks();
         auto fathers=xItem->getFatherConnectLinks();
         foreach (auto son,sons)
         {
             auto linkPen= son->highLightPen();
             linkPen.setColor(QColor(255,255,0));
             son->setHighLightPen(linkPen);


             auto pen=son->sonXItem()->highlightPen();
             pen.setColor(QColor(255,255,0));
             son->sonXItem()->setHighlightPen(pen);

             son->setHighlight(true,false);
             son->sonXItem()->setHighlight(true,false);
         }
         foreach (auto father,fathers)
         {
             auto linkPen= father->highLightPen();
             linkPen.setColor(QColor(255,128,64));
            father->setHighLightPen(linkPen);

             auto pen=father->fatherXItem()->highlightPen();
             pen.setColor(QColor(255,255,0));
             father->fatherXItem()->setHighlightPen(pen);


             father->setHighlight(true,false);
             father->fatherXItem()->setHighlight(true,false);
         }

         m_Scene->getView()->viewport()->update();

       }

    });

    connect(m_Scene,&XGraphicsScene::xLinkSingleSelect,this,[this](XGraphicsConnectLink* link)
    {
        if(link)
        {
            link->sonXItem()->setHighlight(true);
            link->fatherXItem()->setHighlight(true);
        }
    });
}

void AppMainWindow::addInfo(const QString &info)
{
   QString strTime=QTime::currentTime().toString("hh:mm:ss.zzz");

   QString strLog=strTime+":    "+info;
   auto ptxt=ui->ptxtInfo;
   //设置光标到文本末尾
   ptxt->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
   //文本框使用以上设定
   ptxt->appendPlainText(strLog);
   //移动滚动条到底部
   QScrollBar *scrollbar =  ptxt->verticalScrollBar();
   if(scrollbar)
   {
      scrollbar->setSliderPosition(scrollbar->maximum());
   }

}


///XItem添加
void AppMainWindow::onBtnXItemAdd()
{
    auto curData= ui->cmbXItemType->currentData();
    auto xItemType=curData.toString();

#ifndef USE_FACTORY
    if(!m_itemFactory) return;
    QDrag *drag = new QDrag(this);
    XGraphicsMimeData *data = new XGraphicsMimeData();
    data->clear();
    data->setXItem(m_itemFactory->getXGraphicsItem(xItemType));
    data->setText(xItemType);
    drag->setMimeData(data);
    drag->exec(Qt::MoveAction);
#else
    QDrag *drag = new QDrag(this);
    QMimeData *data = new QMimeData();
    data->clear();
    data->setText(xItemType);
    drag->setMimeData(data);
    drag->exec(Qt::MoveAction);
#endif
}

///XItem编辑失焦
void AppMainWindow::onXTextEditFocusOut()
{
    if(m_xTextEdit->xItem())
    {
        auto str=m_xTextEdit->toPlainText();
        m_xTextEdit->xItem()->setText(str);
        m_xTextEdit->setXItem(nullptr);
    }
    if(m_xTextEdit->xLink())
    {
        auto str=m_xTextEdit->toPlainText();
        m_xTextEdit->xLink()->setText(str);
        m_xTextEdit->setXLink(nullptr);
    }
    m_Scene->getView()->setZoomAble(true);
    m_xTextEdit->hide();
}

///显示网格
void AppMainWindow::on_ckbShowGrid_stateChanged(int arg1)
{
    bool bShow= ui->ckbShowGrid->isChecked();
    m_Scene->getView()->setShowGridBig(bShow);
    m_Scene->update();
    m_Scene->getView()->update();
}

void AppMainWindow::on_ckbShowGridSmall_stateChanged(int arg1)
{
    bool bShow= ui->ckbShowGridSmall->isChecked();
    m_Scene->getView()->setShowGridSmall(bShow);
    m_Scene->update();
    m_Scene->getView()->update();
}
void AppMainWindow::on_btnShowSon_clicked()
{
    auto items=m_Scene->getSelectXItem();
    if(items.count()>0)
    {
        auto item=dynamic_cast<XGraphicsItem*>(items[0]);
        if(item)
        {
            printfSon(this,item,item);
        }
    }
}


void AppMainWindow::on_btnShowFather_clicked()
{
    auto items=m_Scene->getSelectXItem();
    if(items.count()>0)
    {
        auto item=dynamic_cast<XGraphicsItem*>(items[0]);
        if(item)
        {
            printfFather(this,item,item);
        }
    }
}


void AppMainWindow::on_btnHideICon_clicked()
{
    auto items=m_Scene->selectedItems();
    if(items.count()>0)
    {
        auto item=dynamic_cast<XGraphicsItem*>(items[0]);
        if(item)
        {
           item->switchShowPixKey("");
        }
    }
}


void AppMainWindow::on_btnZoomToFit_clicked()
{
    m_Scene->zoomToItemRect();
}


void AppMainWindow::on_btnLoad_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("加载"),   QDir::currentPath(),tr("xml File(*.xml)"));
    if(filePath.isNull())
    {
        return;
    }
    if(loadScene(filePath,m_Scene))
    {
        m_Scene->zoomToItemRect();
    }
}


void AppMainWindow::on_btnSave_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                tr("保存"),
                                                QDir::currentPath() + "/" +\
                                                 + "XGraphics.xml",
                                                tr("xml File(*.xml)"));
    if(filePath.isNull())
    {
        return;
    }
    saveScene(filePath,m_Scene);

}

void AppMainWindow::onXItemSetText(XGraphicsItem *item)
{
    QRectF rect=item->textRect();
    if(rect.isValid()&&m_xTextEdit)
    {
        if(!item->item()) return;
        auto sRect=item->item()->mapRectToScene(rect);
        auto vTLPos=  m_Scene->getView()->mapFromScene(sRect.topLeft());
        auto vBRPos=m_Scene->getView()->mapFromScene(sRect.bottomRight());
        QRectF itemRect = QRectF(vTLPos, vBRPos);

        m_xTextEdit->setXItem(item);
        m_xTextEdit->setFixedSize(itemRect.width(), itemRect.height());
        m_xTextEdit->setFont(item->textFont());
        m_xTextEdit->move(itemRect.topLeft().toPoint());
        m_xTextEdit->setFocus();
        m_xTextEdit->show();
        m_xTextEdit->setText(item->text());
        m_Scene->getView()->setZoomAble(false);
    }

}

void AppMainWindow::onXItemSwitchPix(XGraphicsItem *item)
{
    auto pixMaps=item->pixdata();
    if(pixMaps.count()>0)
    {
        auto curKey=item->showPixKey();
        auto iterator_1 = pixMaps.constBegin();

        while (iterator_1 != pixMaps.constEnd())
        {
            if(iterator_1.key()==curKey)
            {
                 ++iterator_1;
                break;
            }
            ++iterator_1;
        }
        if( iterator_1!= pixMaps.constEnd())
        {
            curKey=iterator_1.key();
        }
        else
        {
             curKey=pixMaps.constBegin().key();
        }
        item->switchShowPixKey(curKey);
    }
}

void AppMainWindow::onXLinkSetText(XGraphicsConnectLink *link)
{
    if(link)
    {
        QRectF rect=link->textRect();
        if(rect.isValid()&&m_xTextEdit)
        {
            auto sRect=link->mapRectToScene(rect);
            auto vTLPos=  m_Scene->getView()->mapFromScene(sRect.topLeft());
            auto vBRPos=m_Scene->getView()->mapFromScene(sRect.bottomRight());
            QRectF itemRect = QRectF(vTLPos, vBRPos);

            m_xTextEdit->setXLink(link);
            m_xTextEdit->setFixedSize(itemRect.width(), itemRect.height());
            m_xTextEdit->setFont(link->textFont());
            m_xTextEdit->move(itemRect.topLeft().toPoint());
            m_xTextEdit->setFocus();
            m_xTextEdit->show();
            m_xTextEdit->setText(link->text());
            m_Scene->getView()->setZoomAble(false);
        }
    }
}




