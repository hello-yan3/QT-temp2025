#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QWidget>
#include <QProgressBar>

namespace Ui {
class SplashWindow;
}

class SplashWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SplashWindow(QWidget *parent = nullptr);
    ~SplashWindow() override;

    void myShow();
    QProgressBar *progressBar;
private:
    Ui::SplashWindow *ui;
};

#endif // SPLASHWINDOW_H
