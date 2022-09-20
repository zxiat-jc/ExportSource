#pragma once

// 导入Qt核心库
#include <QtCore>
#include <QFile>
#include <QTextCodec>
#include <QRegExp>
#include <icu.h>

class FileUtils
{
private:
	FileUtils() {};
public:
	/// <summary>
	/// 读取文本文件内容并返回QString
	/// </summary>
	/// <param name="path">文件路径</param>
	/// <returns>文本内容</returns>
	static QString readFile(QString path) {
		// icu识别字符编码
		UErrorCode status = U_ZERO_ERROR;
		UCharsetDetector* detector = ucsdet_open(&status);
		if (U_FAILURE(status)) {
			return "";
		}
		// 读取文件内容
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly)) {
			return "";
		}
		QByteArray data = file.readAll();
		file.close();
		// 识别字符编码
		ucsdet_setText(detector, data.data(), data.size(), &status);
		if (U_FAILURE(status)) {
			return "";
		}
		const UCharsetMatch* match = ucsdet_detect(detector, &status);
		if (U_FAILURE(status)) {
			return "";
		}
		const char* charset = ucsdet_getName(match, &status);
		if (U_FAILURE(status)) {
			return "";
		}
		// 转换编码
		QTextCodec* codec = QTextCodec::codecForName(charset);
		QString text = codec->toUnicode(data);
		// 释放资源
		ucsdet_close(detector);
		return text;
	}


	/// <summary>
	/// 追加文字导出
	/// </summary>
	/// <param name="path">文件路径</param>
	/// <param name="text">文本</param>
	static void exportString(QString path, QString text) {
		QFile file(path);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
			return;
		}
		QTextStream out(&file);
		out << text.replace("\r\n", "\n");
		file.close();
	}

	/// <summary>
	/// 导出文件
	/// </summary>
	/// <param name="path">源目录路径</param>
	/// <param name="reDir">是否递归目录</param>
	/// <param name="selRegs">匹配的正则表达式</param>
	/// <param name="exRegs">排除的正则表达式</param>
	/// <param name="isSelReg">使用匹配的正则表达式</param>
	static void exportFile(
		QString path,
		bool reDir,
		QList<QRegExp> selRegs,
		QList<QRegExp> exRegs,
		bool isSelReg,
		QString outPath)
	{
		QDir dir(path);
		if (!dir.exists()) {
			return;
		}
		// 获取目录下所有文件
		QFileInfoList list = dir.entryInfoList();
		for (auto&& fileInfo : list) {
			if (fileInfo.isDir()) {
				// 如果是.或..则跳过
				if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
					continue;
				}
				qDebug() << "dir:" << fileInfo.filePath();
				// 如果是递归目录
				if (reDir) {
					exportFile(fileInfo.filePath(), reDir, selRegs, exRegs, isSelReg, outPath);
				}
			}
			else {
				qDebug() << "file:" << fileInfo.fileName();
				QList<QRegExp>& regs = isSelReg ? selRegs : exRegs;
				for (auto&& reg : regs) {
					if (!(isSelReg ^ reg.exactMatch(fileInfo.fileName()))) {
						qDebug() << "file name:" << fileInfo.fileName() << "\tstatus:" <<
							reg.exactMatch(fileInfo.fileName());
						// 读取文件内容
						QString text = readFile(fileInfo.filePath());
						// 导出文件
						exportString(outPath, text);
						break;
					}
				}

			}
		}
	}
};
