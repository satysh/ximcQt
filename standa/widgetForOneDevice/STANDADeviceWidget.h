#pragma once

#include <QWidget>

class STANDADevice;
class QPushButton;
class QLabel;
class QLineEdit;

class STANDADeviceWidget : public QWidget
{
    Q_OBJECT
public:
    STANDADeviceWidget(QWidget *parent = nullptr);
    ~STANDADeviceWidget();

    void makeButtons();
    void makeLabels();
    void makeEditors();
    void makeConnections();
    void makeLayout();

private:
    STANDADevice *pdevice;

    QPushButton *pcmdOk;
    QPushButton *pcmdMove;

    QLabel *plblName;
    QLabel *plblId;
    QLabel *plblPos;
    QLabel *plblStep;

    QLineEdit *pnameEdit;
    QLineEdit *pidEdit;
    QLineEdit *pposEdit;
    QLineEdit *pstepEdit;
};
