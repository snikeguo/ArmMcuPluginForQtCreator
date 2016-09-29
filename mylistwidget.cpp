#include "mylistwidget.h"
#include<QMenu>
MyListWidget::MyListWidget(QWidget *parent):QListWidget(parent)
{
    popMenu = new QMenu(this);
    del=new QAction(tr("delete"), this);
    popMenu->addAction(del);
    connect(del,SIGNAL(triggered(bool)),this,SLOT(deleteItem()));
}
void MyListWidget::AddRightMenu(QAction *d)
{
    if(d==Q_NULLPTR)
        return;
    popMenu->addAction(d);
}
void MyListWidget::contextMenuEvent(QContextMenuEvent *event)
{

    QListWidgetItem *item=this->currentItem();
    if(item==Q_NULLPTR)
    {
        return;
    }
    popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
}

void MyListWidget::deleteItem()
{
    QListWidgetItem *item=this->currentItem();
    if(item!=Q_NULLPTR)
    {

        this->removeItemWidget(item);
        delete item;
    }
}
