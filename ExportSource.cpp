#include "ExportSource.h"
#include "FileUtils.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>

ExportSource::ExportSource(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.executeBtn, &QPushButton::clicked, this, &ExportSource::execute);
	connect(ui.selSourceDirBtn, &QPushButton::clicked, this, [this]() {
		QString dir = QFileDialog::getExistingDirectory(this, tr("Select Source Directory"), ui.sourceLineEdit->text());
		if (!dir.isEmpty()) {
			ui.sourceLineEdit->setText(dir);
		}
		});
	connect(ui.selOutDirBtn, &QPushButton::clicked, this, [this]() {
		QString dir = QFileDialog::getSaveFileName(this, tr("Select Output File"), ui.outLineEdit->text());
		if (!dir.isEmpty()) {
			ui.outLineEdit->setText(dir);
		}
		});
}

ExportSource::~ExportSource()
{
}

void ExportSource::execute()
{
	QString selDir = ui.sourceLineEdit->text();
	if (selDir.isEmpty())
	{
		// 提示输入源目录
		QMessageBox::warning(this, tr("Warning"), tr("Please input source directory!"));
		return;
	}

	QString outDir = ui.outLineEdit->text();
	if (outDir.isEmpty())
	{
		outDir = "./out.txt";
	}
	QStringList selFilterStr = ui.regSelTextEdit->toPlainText().replace("\r\n", "\n").split("\n");
	QList<QRegExp> selFilter = QList<QRegExp>();
	for (auto& f : selFilterStr)
	{
		if (!f.isEmpty())
		{
			selFilter.append(QRegExp(f));
		}
	}

	QStringList ignoreFilterStr = ui.exceptRegTextEdit->toPlainText().replace("\r\n", "\n").split("\n");
	QList<QRegExp> ignoreFilter = QList<QRegExp>();
	for (auto& f : ignoreFilterStr)
	{
		if (!f.isEmpty())
		{
			ignoreFilter.append(QRegExp(f));
		}
	}
	// 删除输出文件
	QFile::remove(outDir);
	bool reDir = ui.recursionCheckBox->isChecked();
	bool isSelReg = ui.recursionCheckBox->isChecked();
	FileUtils::exportFile(selDir, reDir, selFilter, ignoreFilter, isSelReg, outDir);
	// 导出完成
	QMessageBox::information(this, tr("Info"), tr("Export completed!"));
}
