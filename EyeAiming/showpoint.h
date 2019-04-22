#ifndef SHOWPOINT_H
#define SHOWPOINT_H

#include <QDialog>
#include <QTimer>
#include <QPaintEvent>
#include <QPoint>
namespace Ui {
class ShowPoint;
}

class ShowPoint : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPoint(QWidget *parent = nullptr);
    ~ShowPoint();

public slots:
    void updateImage();

private:
    Ui::ShowPoint *ui;
    QTimer theTimer;

    int i = 0;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // SHOWPOINT_H
