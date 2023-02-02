#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include <QMainWindow>
#include "XGraphicsScene.h"
#include "XGraphicsTextEdit.h"
#include "AppXGraphicsFactory.h"


QT_BEGIN_NAMESPACE
namespace Ui { class AppMainWindow; }
QT_END_NAMESPACE

class AppMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppMainWindow(QWidget *parent = nullptr);
    ~AppMainWindow();
protected:
    ///初始化界面
    void initFrm();
    ///初始化控件
    void initCtrl();
    ///初始XItem添加按钮
    void initXItemBtn();
    ///初始化Scene槽函数Slot
    void initSceneSlots();

public://工具接口
    /// 添加信息
    void addInfo(const QString &info);

private slots:
    ///XItem拖动添加
    void onBtnXItemAdd();
    ///XItem重命名编辑框失焦
    void onXTextEditFocusOut();
    ///显示网格
    void on_ckbShowGrid_stateChanged(int arg1);
    ///缩放到合适范围
    void on_btnZoomToFit_clicked();
    ///显示选择的XItem后代
    void on_btnShowSon_clicked();
    ///显示选择的XItem祖先
    void on_btnShowFather_clicked();

    void on_btnHideICon_clicked();



    void on_btnLoad_clicked();

    void on_btnSave_clicked();

private:
    Ui::AppMainWindow *ui;
    AppXGraphicsItemFactory* m_itemFactory=nullptr;
    AppXGraphicsLinkFactory* m_linkFactory=nullptr;
    XGraphicsTextEdit *m_xTextEdit=nullptr;
    XGraphicsScene* m_Scene=nullptr;
};
#endif // APPMAINWINDOW_H
