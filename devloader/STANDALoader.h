#pragma once

#include <ximc.h>

#include <QObject>

class STANDALoader : public QObject
{
	Q_OBJECT
public:
	STANDALoader(QObject *parent = nullptr);
	~STANDALoader();

public slots:
    void testappeasy();
    void left();
    void right();
    void stop();

signals:
    void failed();

private:
	device_t device;
};