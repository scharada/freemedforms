/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2012 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *   Main Developper : Eric MAEKER, <eric.maeker@gmail.com>                *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#include "global.h"
#include "quazip.h"
#include "quazipfile.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

#include <QDebug>

/**
  \namespace QuaZipTools
  \brief Some generic funtions related to QuaZip (zip).
*/
namespace QuaZipTools {


/** Unzip the specified file named \e fileName to the specified path \e pathToUnZippedFiles. If the output path is empty, unzip in the same dir as the zip file. */
bool unzipFile(const QString &fileName, const QString &pathToUnZippedFiles)
{
    Q_ASSERT_X(QFile(fileName).exists() , "Function unzipFile()",
               qPrintable(QString("File %1 does not exists").arg(fileName)));

    QString outputPath = pathToUnZippedFiles;
    if (pathToUnZippedFiles.isEmpty())
        outputPath = QFileInfo(fileName).absolutePath();

    Q_ASSERT_X(QDir(outputPath).exists() , "Function unzipFile()",
               qPrintable(QString("Dir %1 does not exists").arg(outputPath)));

    qWarning() << "QuaZip try to unzip" << fileName << outputPath;

    QuaZip zip(fileName);
    if (!zip.open(QuaZip::mdUnzip)) {
        qWarning() << QString("QuaZip Error: %1: %2").arg(fileName).arg(zip.getZipError());
        return false;
    }

    QuaZipFile file(&zip);
    QFile out;
    QString name;
    char c;

    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << QString("QuaZip Error: %1: %2").arg(fileName).arg(zip.getZipError());
            qWarning() << __FILE__ << __LINE__;
            return false;
        }

        name = file.getActualFileName();

        if (file.getZipError() != UNZ_OK) {
            qWarning() << QString("QuaZip Error: %1: %2").arg(fileName).arg(zip.getZipError());
            qWarning() << __FILE__ << __LINE__;
            return false;
        }
        out.setFileName(outputPath + QDir::separator() + name);

        // inform user
        qWarning() << QString("Zip : extracting : %1").arg(out.fileName());

        // create path if not exists
        if (!QDir(QFileInfo(out).absolutePath()).exists()) {
            QDir().mkpath(QFileInfo(out).absolutePath());
        }

        // open the output file
        if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << QString("QuaZip Error: %1: %2").arg(out.fileName()).arg(out.error());
            qWarning() << __FILE__ << __LINE__;
            return false;
        }
        while (file.getChar(&c)) out.putChar(c);
        out.close();

        if (file.getZipError() != UNZ_OK) {
            qWarning() << QString("QuaZip Error: %1: %2").arg(fileName).arg(zip.getZipError());
            return false;
        }

        if (!file.atEnd()) {
            qWarning() << QString("Zip : read all but not EOF : ") + fileName;
            return false;
        }
        file.close();

        if (file.getZipError() != UNZ_OK) {
            qWarning() << QString("QuaZip Error: %1: %2").arg(fileName).arg(zip.getZipError());
            return false;
        }
    }
    zip.close();
    if (zip.getZipError() != UNZ_OK) {
        qWarning() << QString("QuaZip Error: %1: %2").arg(fileName).arg(zip.getZipError());
        return false;
    }

    return true;
}

bool unzipAllFilesIntoDirs(const QStringList &paths)
{
    foreach(QString p, paths) {
        QDir d(p);
        if (!d.exists())
            continue;

        // get all zip files in dir
        d.setNameFilters(QStringList() << "*.zip");
        d.setFilter(QDir::Files | QDir::NoSymLinks | QDir::Readable);
        QStringList zipFiles = d.entryList();

        foreach(QString f, zipFiles) {
            // if file if already unzipped dir with its baseName exists and is populated with files
            QDir dz(p);
            dz.setFilter(QDir::Files | QDir::NoSymLinks | QDir::Readable | QDir::NoDotAndDotDot);
            if ((dz.cd(QFileInfo(f).baseName())) &&
                    (dz.entryList().count()))
                continue;

            // d must not change here +++
            // file was not unzipped by me, so do it
            // in first create the output directory
            if (!d.cd(QFileInfo(f).baseName() )) {
                d.mkdir(QFileInfo(f).baseName());
                dz.cd(QFileInfo(f).baseName());
            }
            else d.cdUp();

            // in second unzip file to the output directory
            unzipFile(d.absolutePath() + QDir::separator() + f, dz.absolutePath());
        }
    }
    return true;
}


};
