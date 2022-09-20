#pragma once

#include <QtWidgets/QWidget>
#include "ui_ExportSource.h"

class ExportSource : public QWidget
{
    Q_OBJECT

public:
    ExportSource(QWidget *parent = nullptr);
    ~ExportSource();
    void execute();
private:
    Ui::ExportSourceClass ui;
};
