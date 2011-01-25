/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2010 by Eric MAEKER, MD (France) <eric.maeker@free.fr>        *
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
 *   Main Developper : Eric MAEKER, <eric.maeker@free.fr>                  *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#ifndef FORMPLACEHOLDER_H
#define FORMPLACEHOLDER_H

#include <formmanagerplugin/formmanager_exporter.h>

#include <QWidget>
#include <QTreeView>
#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class  QTreeWidgetItem;
class QStackedLayout;
class QModelIndex;
QT_END_NAMESPACE

namespace Form {
class EpisodeModel;

namespace Internal {

class FormPlaceHolderPrivate;

class FormItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    FormItemDelegate(QObject *parent = 0);
    void setEpisodeModel(EpisodeModel *model);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    mutable QModelIndex pressedIndex;
    EpisodeModel *m_EpisodeModel;
};
}  // End namespace Internal


class FORM_EXPORT FormPlaceHolder : public QWidget
{
    Q_OBJECT
    friend class FormManager;

public:
    FormPlaceHolder(QWidget *parent = 0);
    ~FormPlaceHolder();

    void setEpisodeModel(EpisodeModel *model);

    QTreeView *formTree() const;
    QStackedLayout *formStackLayout() const;
    
    void addTopWidget(QWidget *top);
    void addBottomWidget(QWidget *bottom);

public Q_SLOTS:
    void setCurrentForm(const QString &formUuid);
    void setCurrentEpisode(const QModelIndex &index);

protected Q_SLOTS:
    void handlePressed(const QModelIndex &index);
    void handleClicked(const QModelIndex &index);
    void reset();
    void newEpisode();
    void removeEpisode();
    void validateEpisode();

private:
    Internal::FormPlaceHolderPrivate *d;
};

}  // End namespace Form

#endif // FORMPLACEHOLDER_H
