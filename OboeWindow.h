#ifndef OBOEWINDOW_H
#define OBOEWINDOW_H

#include <QMainWindow>

namespace Ui {
class OboeWindow;
}

class OboeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OboeWindow(QWidget *parent = nullptr);
    ~OboeWindow();

private:
    Ui::OboeWindow *ui;
};

#endif // OBOEWINDOW_H
