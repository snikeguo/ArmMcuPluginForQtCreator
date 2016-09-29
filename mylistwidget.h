#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QListWidget>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = Q_NULLPTR);
    void AddRightMenu(QAction *d);
protected:
    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void deleteItem();
private:
    QMenu *popMenu;
    QAction *del;
};

#endif // MYLISTWIDGET_H
