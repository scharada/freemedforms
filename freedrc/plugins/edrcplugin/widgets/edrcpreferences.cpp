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
 *   Main Developpers:                                                     *
 *       Eric Maeker <e>                             *
 *   Contributors:                                                         *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#include "edrcpreferences.h"
#include "ui_edrcpreferences.h"
#include <edrcplugin/constants.h>

#include <translationutils/constants.h>
#include <translationutils/trans_current.h>

#include <coreplugin/icore.h>
#include <coreplugin/isettings.h>
#include <coreplugin/constants_menus.h>

using namespace eDRC;
using namespace Internal;
using namespace Trans::ConstantTranslations;

static inline Core::ISettings *settings() { return Core::ICore::instance()->settings(); }

/* ----------------------  Preferences Widget ---------------------- */

/*! Creates a new preferences widget with a given parent. */
EdrcPreferencesPageWidget::EdrcPreferencesPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EdrcPreferencesPageWidget)
{
    ui->setupUi(this);
    setDataToUi();
}

EdrcPreferencesPageWidget::~EdrcPreferencesPageWidget()
{
    delete ui;
}

/*! Sets data of a changed data model to the ui's widgets. */
void EdrcPreferencesPageWidget::setDataToUi()
{
    ui->realTimeChecking->setChecked(settings()->value(Constants::S_REALTIME_CR_CODING_CHECKING).toBool());
    ui->useModernLabelling->setChecked(settings()->value(Constants::S_CR_USE_MODERNLABEL).toBool());
    ui->mandatoryInBold->setChecked(settings()->value(Constants::S_CR_MANDATORYLABEL_IN_BOLD).toBool());
}

/*! \sa IOptionsPage::matches() */
QString EdrcPreferencesPageWidget::searchKeywords() const
{
    QString rc;
    // uncomment this lines to add some keywords from UI elements
    // QTextStream(&rc)
    //        << ui->someLabel->text()
    //        << ui->otherLabel->text();
    
    rc.remove(QLatin1Char('&'));
    return rc;
}

/*! Saves the settings in the ui to the settings data model. */
void EdrcPreferencesPageWidget::saveToSettings(Core::ISettings *sets)
{
    Q_UNUSED(sets);
    // if no sets given as param, take default interface
    Core::ISettings *s = sets? sets : settings();

    s->setValue(Constants::S_REALTIME_CR_CODING_CHECKING, ui->realTimeChecking->isChecked());
    s->setValue(Constants::S_CR_USE_MODERNLABEL, ui->useModernLabelling->isChecked());
    s->setValue(Constants::S_CR_MANDATORYLABEL_IN_BOLD, ui->mandatoryInBold->isChecked());
#ifdef FREEDRC
    s->sync();
#endif
}

/*! Writes the default settings to the data model. */
void EdrcPreferencesPageWidget::writeDefaultSettings(Core::ISettings *s)
{
    Q_UNUSED(s);
    //    LOG_FOR(tkTr(Trans::Constants::CREATING_DEFAULT_SETTINGS_FOR_1).arg("EdrcPreferencesPageWidget"));

    s->setValue(Constants::S_REALTIME_CR_CODING_CHECKING, true);
    s->setValue(Constants::S_CR_USE_MODERNLABEL, true);
    s->setValue(Constants::S_CR_MANDATORYLABEL_IN_BOLD, true);
#ifdef FREEDRC
    s->sync();
#endif
}

/*! Retranslates the ui widgets to the changed language. */
void EdrcPreferencesPageWidget::retranslateUi()
{
}

void EdrcPreferencesPageWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
}

/* ----------------------  Preferences Page ---------------------- */

/*!
 * \class Editor::EdrcPreferencesPage
 * \brief Generic FreeMedForms preferences page for Editor plugin.
 *
 *  The page is listed in the settings dialog under the category returned by \sa category().
 */

/*! Creates a new preferences page with a given parent. */
EdrcPreferencesPage::EdrcPreferencesPage(QObject *parent) :
    IOptionsPage(parent),
    m_Widget(0)
{
    setObjectName("EdrcPreferencesPage");
}

EdrcPreferencesPage::~EdrcPreferencesPage()
{
    if (m_Widget)
        delete m_Widget;
    m_Widget = 0;
}

/*! Returns the id if the preferences page. */
QString EdrcPreferencesPage::id() const
{
    return objectName();
}

/*! Returns the (translated) name of the preferences page. */
QString EdrcPreferencesPage::displayName() const
{
    return tr("French eDRC");
}

/*! Returns the (translated) category of the preferences page. */
QString EdrcPreferencesPage::category() const
{
    return tr("French eDRC");
}

/*! Returns the (translated) title of the preferences page. */
QString EdrcPreferencesPage::title() const
{
    return tr("French eDRC preferences");
}

/*! Returns the sorting order (pages are sorted starting from 0). */
int EdrcPreferencesPage::sortIndex() const
{
    return (Core::Constants::OPTIONINDEX_MAIN + 10);
}

/*! Resets the whole preferences page to the default settings of the settings data model. */
void EdrcPreferencesPage::resetToDefaults()
{
    m_Widget->writeDefaultSettings(settings());
    m_Widget->setDataToUi();
}

/*! Overridden function that apllies pending changes to the data model without closing the dialog. */
void EdrcPreferencesPage::apply()
{
    if (!m_Widget) {
        return;
    }
    m_Widget->saveToSettings(settings());
}

void EdrcPreferencesPage::finish()
{
    delete m_Widget;
}

/*! \brief Checks if the entered settings are valid.
 *
 * Overloads the interface method. For each empty value the default settings value is written.
 */
void EdrcPreferencesPage::checkSettingsValidity()
{
    QHash<QString, QVariant> defaultvalues;
    defaultvalues.insert(Constants::S_CR_USE_MODERNLABEL, true);
    defaultvalues.insert(Constants::S_CR_MANDATORYLABEL_IN_BOLD, true);
    defaultvalues.insert(Constants::S_REALTIME_CR_CODING_CHECKING, false);

    foreach(const QString &k, defaultvalues.keys()) {
        if (settings()->value(k) == QVariant())
            settings()->setValue(k, defaultvalues.value(k));
    }
}

bool EdrcPreferencesPage::matches(const QString &searchKeyWord) const
{
    return m_searchKeywords.contains(searchKeyWord, Qt::CaseInsensitive);
}

/*! Creates the settings page */
QWidget *EdrcPreferencesPage::createPage(QWidget *parent)
{
    if (m_Widget)
        delete m_Widget;
    m_Widget = new EdrcPreferencesPageWidget(parent);
    if (m_searchKeywords.isEmpty())
        m_searchKeywords = m_Widget->searchKeywords();
    return m_Widget;
}

