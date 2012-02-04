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
/**
  \class Utils::MessageSender
  \brief This class is a message sender over the web.
  It works with PHP scripts that are available on the FreeMedForms server.\n
  You can set the type of message you want to send using the enumerator MessageSender::typeOfMessage and setTypeOfMessage().\n
  You must set the message using setMessage(), optionnally set the user using setUser(), and the post the message using postMessage().\n
  Get the sending state using isSending().\n
  <b>The object should not be destructed during the sending process.</b>
*/

#include "messagesender.h"

#include <utils/log.h>
#include <utils/global.h>
#include <translationutils/constants.h>
#include <translationutils/trans_spashandupdate.h>
#include <translationutils/trans_msgerror.h>

#include <QMessageBox>
#include <QHttp>
#include <QtNetwork>
#include <QBuffer>

using namespace Utils;
using namespace Utils::Internal;
using namespace Trans::ConstantTranslations;

namespace {
const char * const URL_DOSAGETRANSMISSION     = "http://www.freemedforms.com/appscripts/FMF_DosagesToStore.php";
const char * const URL_DRUGSARECORRECT        = "http://www.freemedforms.com/appscripts/FMF_Correct.php";
const char * const URL_DRUGSAREINCORRECT      = "http://www.freemedforms.com/appscripts/FMF_Uncorrect.php";
const char * const URL_DEVELOPPERMAILSENDING  = "http://www.freemedforms.com/appscripts/FMF_Bug.php";
}  // End Constants


namespace Utils {
namespace Internal {
class MessageSenderPrivate
{
public:
    MessageSenderPrivate() :
        m_Parent(0),
        m_Buffer(0),
        m_ShowMsgBox(false),
        m_IsSending(false),
        m_type(MessageSender::InformationToDevelopper)
    {}
    ~MessageSenderPrivate() { delete m_Buffer; }

    QUrl     url;
    QHttp    http;
    QWidget *m_Parent;
    QBuffer *m_Buffer;
    QString  m_User, m_Msg;
    bool     m_ShowMsgBox;
    QString  m_LastResult;
    bool     m_IsSending;
    MessageSender::TypeOfMessage m_type;
};
}
}


/** \brief Constructor */
MessageSender::MessageSender(QObject *parent)
             : QObject(parent), d(new MessageSenderPrivate())
{
    setObjectName("MessageSender");
    connect(&d->http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
}

/**
  \brief Destructor
  WARNING : Do not delete the object while sending the message
*/
MessageSender::~MessageSender()
{
    if (d) delete d;
    d=0;
}

/** \brief Set the parent QObject */
void MessageSender::setParent(QWidget * parent)
{ d->m_Parent = parent; }

/** \brief Set the user that is sending information */
void MessageSender::setUser(const QString & usr)
{ d->m_User = usr; }

/** \brief Set the the message to send */
void MessageSender::setMessage(const QString & msg)
{ d->m_Msg = msg ; }

/** \brief Return the currently used URL for the posting */
QString MessageSender::usedUrl() const
{ return d->url.toString(); }

/** \brief Returns the state of the transmission. */
bool MessageSender::isSending() const
{ return d->m_IsSending; }

/** \brief Set the result message box to shown or not at completion. */
void MessageSender::showResultingMessageBox(bool state)
{ d->m_ShowMsgBox = state; }

/** \brief The returned text of the server. */
QString MessageSender::resultMessage() const
{
    return d->m_LastResult;
}

/** \brief Set the type of message to send */
bool MessageSender::setTypeOfMessage(const TypeOfMessage &t)
{
    d->m_type = t;
    switch (t)
    {
        case CorrectDrugsCoding   : d->url = QUrl(::URL_DRUGSARECORRECT); break;
        case UncorrectDrugsCoding : d->url = QUrl(::URL_DRUGSAREINCORRECT); break;
        case InformationToDevelopper : d->url = QUrl(::URL_DEVELOPPERMAILSENDING); break;
        case DosageTransmission : d->url = QUrl(::URL_DOSAGETRANSMISSION); break;
        default : return false; break;
    }
    return true;
}

/** \brief Starts to post the message. Returns true if the sending has started. */
bool MessageSender::postMessage()
{
    if (!d->url.isValid())
        return false;
    if (d->url.scheme() != "http")
        return false;
    if (d->url.path().isEmpty())
        return false;
    if (d->m_Msg.isEmpty())
        return false;

    LOG(tkTr(Trans::Constants::STARTING_TASK_1).arg(tkTr(Trans::Constants::POST_TO_1).arg(d->url.toString())));

    d->http.setHost(d->url.host(), d->url.port(80));

    QHttpRequestHeader header("POST", d->url.path());
    header.setValue("Host", d->url.host());
    header.setContentType("application/x-www-form-urlencoded");

    QString s = "";
    if (d->m_User.isEmpty())
        s.append("user=anomynous");
    else
        s.append("user=" + d->m_User);
    if (d->m_type==InformationToDevelopper)
        s.append("&msg=" + d->m_Msg.toUtf8().toBase64());
    else
        s.append("&msg=" + d->m_Msg);

    d->http.setHost(d->url.host());
    d->m_Buffer = new QBuffer(qApp);
    d->m_Buffer->open(QBuffer::ReadWrite);
    d->http.request(header, s.toUtf8(), d->m_Buffer);
    d->m_IsSending = true;
    return true;
}

/**
  \brief Slot connected to the http object.
  If the message is correctly sended a message box shows the result sended by the server (see showResultingMessageBox()). \n
  Then the signal sent() is emitted.
*/
void MessageSender::httpDone(bool error)
{
    QString ret = "";
    if (!error) {
        ret = tkTr(Trans::Constants::MESSAGE_SENDED_OK);
        LOG(ret);
        LOG(d->m_Buffer->data());
    } else {
        ret = tkTr(Trans::Constants::ERROR_1_OCCURED_WHILE_2).arg(tkTr(Trans::Constants::POST_TO_1).arg(d->http.errorString()));
        LOG_ERROR(ret);
        LOG_ERROR(d->m_Buffer->data());
    }

    d->m_LastResult = QString(d->m_Buffer->data());

    // Show informations
    if (d->m_ShowMsgBox) {
        Utils::informativeMessageBox(ret , tkTr(Trans::Constants::INFORMATIVE_MESSAGE_1).arg(d->m_LastResult), "");
    }

    if (d->m_Buffer)
        delete d->m_Buffer;
    d->m_Buffer = 0;
    d->m_IsSending = false;

    Q_EMIT sent();
}
