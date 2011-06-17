/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2011 by Eric MAEKER, MD (France) <eric.maeker@free.fr>        *
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
#include "iusercalendar.h"

#include <coreplugin/icore.h>
#include <coreplugin/iuser.h>

using namespace Agenda;
//using namespace Internal;

static inline Core::IUser *user() {return Core::ICore::instance()->user();}

IUserCalendar::IUserCalendar() :
        m_Modified(false)
{
    m_Datas.insert(DbOnly_UserCalId, -1);
    m_Datas.insert(DbOnly_CalId, -1);
    m_Datas.insert(DbOnly_CatId, -1);
    m_Datas.insert(DbOnly_IsValid, false);
    m_Datas.insert(UserOwnerUid, user()->uuid());
}

IUserCalendar::~IUserCalendar()
{}

bool IUserCalendar::isValid() const
{
    /** \todo code here */
    return true;
}

bool IUserCalendar::isNull() const
{
    /** \todo code here */
    return false;
}

QVariant IUserCalendar::data(const int ref) const
{
    return m_Datas.value(ref);
}

bool IUserCalendar::setData(const int ref, const QVariant &value)
{
    m_Datas.insert(ref, value);
    m_Modified = true;
    return true;
}

bool IUserCalendar::isModified() const
{
    return m_Modified;
}

void IUserCalendar::setModified(const bool state)
{
    m_Modified=state;
}

void IUserCalendar::setDatabaseValue(const int ref, const QVariant &value)
{
    m_Datas.insert(ref, value);
    m_Modified = true;
}

int IUserCalendar::calendarId() const
{
    return m_Datas.value(DbOnly_CalId).toInt();
}

QString IUserCalendar::userOwnerUid() const
{
    return m_Datas.value(UserOwnerUid).toString();
}

int IUserCalendar::categoryId() const
{
    return m_Datas.value(DbOnly_CatId).toInt();
}

QString IUserCalendar::xmlOptions() const
{
    return QString();
}
