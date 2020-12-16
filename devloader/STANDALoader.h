#pragma once

#include <QObject>

class STANDALoader : public QObject
{
	Q_OBJECT
public:
	STANDALoader(QObject *parent = nullptr);
	~STANDALoader();

public slots:
    void testappeasy();

signals:
    void failed();
	
};