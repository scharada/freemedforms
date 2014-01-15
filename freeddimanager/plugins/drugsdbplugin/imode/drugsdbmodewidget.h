/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2013 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
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
 *   Main developers : Eric Maeker
 *  Contributors:                                                          *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#ifndef DDIMANAGER_DRUGSDB_INTERNAL_DRUGSDBMODEWIDGET_H
#define DDIMANAGER_DRUGSDB_INTERNAL_DRUGSDBMODEWIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE
class QItemSelection;
QT_END_NAMESPACE

/**
 * \file drugsdbmodewidget.h
 * \author Eric Maeker
 * \version 0.10.0
 * \date 10 Jan 2014
*/

namespace DrugsDb {
namespace Internal {
class IDrugDatabase;
class DrugsDbModeWidgetPrivate;

class DrugsDbModeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrugsDbModeWidget(QWidget *parent = 0);
    ~DrugsDbModeWidget();

    void registerDrugDatabase(IDrugDatabase *drugDatabase);
    // bool initialize(IDrugDatabase *step);

private Q_SLOTS:
    void on_startJobs_clicked();
    bool on_download_clicked();
    void downloadFinished();
    void changeStepProgressRange(int min, int max);
    void onCurrentDrugsDatabaseChanged(const QItemSelection &current, const QItemSelection &previous);

//private:
//    void showEvent(QShowEvent *event);

private:
    Internal::DrugsDbModeWidgetPrivate *d;
};

} // namespace Internal
} // namespace DrugsDb

#endif // DDIMANAGER_DRUGSDB_INTERNAL_DRUGSDBMODEWIDGET_H

