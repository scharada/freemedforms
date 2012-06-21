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
 *   Main developers : Eric MAEKER, <eric.maeker@gmail.com>                *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#include "drugbasecore.h"
#include <drugsbaseplugin/constants.h>
#include <drugsbaseplugin/drugsbase.h>
#include <drugsbaseplugin/protocolsbase.h>
#include <drugsbaseplugin/interactionmanager.h>
#include <drugsbaseplugin/versionupdater.h>

#include <coreplugin/icore.h>

#include <utils/log.h>
#include <utils/global.h>
#include <datapackutils/datapackcore.h>
#include <datapackutils/ipackmanager.h>
#include <datapackutils/pack.h>

using namespace DrugsDB;
using namespace Internal;
//using namespace Trans::ConstantTranslations;

static inline DataPack::DataPackCore &dataPackCore() { return DataPack::DataPackCore::instance(); }
static inline DataPack::IPackManager *packManager() { return dataPackCore().packManager(); }

namespace DrugsDB {
namespace Internal {
class DrugBaseCorePrivate
{
public:
    DrugBaseCorePrivate(DrugBaseCore *base) :
        q(base),
        m_DrugsBase(0),
        m_ProtocolsBase(0),
        m_InteractionManager(0),
        m_VersionUpdater(0)
    {
    }

    ~DrugBaseCorePrivate()
    {
        if (m_VersionUpdater)
            delete m_VersionUpdater;
        m_VersionUpdater = 0;
    }

private:
    DrugBaseCore *q;

public:
    DrugsBase *m_DrugsBase;
    ProtocolsBase *m_ProtocolsBase;
    InteractionManager *m_InteractionManager;
    VersionUpdater *m_VersionUpdater;
};
}  // End Internal
}  // End DrugsDB


DrugBaseCore *DrugBaseCore::m_Instance = 0;

/** \brief Returns the unique instance of DrugsDB::DrugBaseCore. If it does not exist, it is created */
DrugBaseCore &DrugBaseCore::instance(QObject *parent)
{
    if (!m_Instance) {
        m_Instance = new DrugBaseCore(parent);
//        m_Instance->init();
    }
    return *m_Instance;
}

DrugBaseCore::DrugBaseCore(QObject *parent) :
    QObject(parent),
    d(new Internal::DrugBaseCorePrivate(this))
{
    d->m_DrugsBase = new DrugsBase(this);
    d->m_ProtocolsBase = new ProtocolsBase(this);

    connect(Core::ICore::instance(), SIGNAL(coreOpened()), this, SLOT(postCoreInitialization()));
    connect(packManager(), SIGNAL(packInstalled(DataPack::Pack)), this, SLOT(packChanged(DataPack::Pack)));
    connect(packManager(), SIGNAL(packRemoved(DataPack::Pack)), this, SLOT(packChanged(DataPack::Pack)));
//    connect(packManager(), SIGNAL(packUpdated(DataPack::Pack)), this, SLOT(packChanged(DataPack::Pack)));
}

DrugBaseCore::~DrugBaseCore()
{
    if (d) {
        delete d;
        d = 0;
    }
}

bool DrugBaseCore::init()
{
    d->m_VersionUpdater = new VersionUpdater;
    d->m_DrugsBase->init();
    d->m_ProtocolsBase->init();
    d->m_InteractionManager = new InteractionManager(this);
    // TODO: code here
    return true;
}

void DrugBaseCore::postCoreInitialization()
{
    connect(Core::ICore::instance(), SIGNAL(databaseServerChanged()), this, SLOT(onCoreDatabaseServerChanged()));
    init();
}

DrugsBase &DrugBaseCore::drugsBase() const
{
    Q_ASSERT(d->m_DrugsBase);
    return *d->m_DrugsBase;
}

ProtocolsBase &DrugBaseCore::protocolsBase() const
{
    Q_ASSERT(d->m_ProtocolsBase);
    return *d->m_ProtocolsBase;
}

InteractionManager &DrugBaseCore::interactionManager() const
{
    Q_ASSERT(d->m_InteractionManager);
    return *d->m_InteractionManager;
}

VersionUpdater &DrugBaseCore::versionUpdater() const
{
    Q_ASSERT(d->m_VersionUpdater);
    return *d->m_VersionUpdater;
}

void DrugBaseCore::onCoreDatabaseServerChanged()
{
    Q_ASSERT(d->m_DrugsBase);
    d->m_DrugsBase->onCoreDatabaseServerChanged();
    d->m_ProtocolsBase->onCoreDatabaseServerChanged();
}

void DrugBaseCore::packChanged(const DataPack::Pack &pack)
{
    if (pack.dataType() == DataPack::Pack::DrugsWithInteractions ||
            pack.dataType() == DataPack::Pack::DrugsWithoutInteractions) {
        d->m_DrugsBase->datapackChanged();
    }
}
