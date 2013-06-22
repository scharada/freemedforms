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
 *  This program is distributed in the hope that it will be useful, *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *  Main developer: Eric MAEKER, <eric.maeker@gmail.com>                   *
 *  Contributors:                                                          *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#ifndef EDRC_INTERNAL_RCCRITERIASMODEL_H
#define EDRC_INTERNAL_RCCRITERIASMODEL_H

#include <QAbstractTableModel>

/**
 * \file rccriteriasmodel.h
 * \author Eric Maeker
 * \version 0.9.0
 * \date 18 June 2013
*/

namespace eDRC {
namespace Internal {
class RcCriteriasModelPrivate;

class RcCriteriasModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Datarepresentation {
        Id = 0,
        Label,
        ItemWeight, // Pondération
        Indentation,
        ColumnCount
    };

    RcCriteriasModel(QObject *parent = 0);
    ~RcCriteriasModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setFilterOnRcId(const int crId);
    void testCoding(const QList<int> &ids);

private:
    RcCriteriasModelPrivate *d;
};

} // namespace eDRC
} // namespace Internal

#endif  // EDRC_INTERNAL_RCCRITERIASMODEL_H
