#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QLineEdit>

#if _MSC_VER >= 1600
#pragma execution_character_set( "utf-8")
#endif

namespace Ui {
class SettingPage;
}

class SettingPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPage(QWidget *parent = nullptr);
    ~SettingPage() override;

    bool openSerial();
    QSerialPort *getSerial() const;

    const QString &getPortName() const;
    static QString dataPath;
private:
    Ui::SettingPage *ui;

    QSerialPort *serial;
    QTimer *timer;
    QStringList oldPortStringList;

    QList<QLineEdit*> lineEditsList[2];

    int baudRateIndex=0;
    QString portName = "NO";
    int bitIndex=0;
    int ParityIndex=0;
    int stopIndex=0;

    void variableInit();
    void viewInit();
    void connectInit();
    void UpdatePort();

    bool saveSQLite();

    void delay(const int msec);
private slots:
    void save();
    void ReadData();
    void chooseFilePath();
};

#endif // SETTINGPAGE_H
