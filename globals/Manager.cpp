#include "globals/Manager.h"

#include <QApplication>
#include "mediaCenterPlugins/XbmcSql.h"
#include "mediaCenterPlugins/XbmcXml.h"
#include "scrapers/Cinefacts.h"
#include "scrapers/OFDb.h"
#include "scrapers/TheTvDb.h"
#include "scrapers/TMDb.h"
#include "scrapers/TMDbConcerts.h"
#include "scrapers/VideoBuster.h"
#include "globals/Globals.h"

/**
 * @brief Manager::Manager
 * @param parent
 */
Manager::Manager(QObject *parent) :
    QObject(parent)
{
    m_scrapers.append(new TMDb(this));
    m_scrapers.append(new Cinefacts(this));
    m_scrapers.append(new OFDb(this));
    m_scrapers.append(new VideoBuster(this));
    m_tvScrapers.append(new TheTvDb(this));
    m_concertScrapers.append(new TMDbConcerts(this));
    m_movieFileSearcher = new MovieFileSearcher(this);
    m_tvShowFileSearcher = new TvShowFileSearcher(this);
    m_concertFileSearcher = new ConcertFileSearcher(this);
    m_movieModel = new MovieModel(this);
    m_tvShowModel = new TvShowModel(this);
    m_tvShowProxyModel = new TvShowProxyModel(this);
    m_concertModel = new ConcertModel(this);

    m_mediaCenters.append(new XbmcXml(this));
    m_mediaCenters.append(new XbmcSql(this, "xbmc"));
    m_mediaCenters.append(new XbmcSql(this, "xbmc"));

    m_mediaCentersTvShow.append(new XbmcXml(this));
    m_mediaCentersTvShow.append(new XbmcSql(this, "xbmcTvShow"));
    m_mediaCentersTvShow.append(new XbmcSql(this, "xbmcTvShow"));

    m_mediaCentersConcert.append(new XbmcXml(this));
    m_mediaCentersConcert.append(new XbmcSql(this, "xbmcConcert"));
    m_mediaCentersConcert.append(new XbmcSql(this, "xbmcConcert"));
}

/**
 * @brief Manager::~Manager
 */
Manager::~Manager()
{
}

/**
 * @brief Returns an instance of the Manager
 * @return Instance of Manager
 */
Manager* Manager::instance()
{
    static Manager *m_instance = 0;
    if (m_instance == 0) {
        m_instance = new Manager(qApp);
    }
    return m_instance;
}

/**
 * @brief Sets up the MediaCenterInterfaces based on the settings
 */
void Manager::setupMediaCenterInterface()
{
    qDebug() << "Entered";
    qDebug() << "Current MediaCenterInterface is";
    if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcXml)
        qDebug() << "Xbmc::Xml";
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcSqlite)
        qDebug() << "Xbmc::Sqlite";
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcMysql)
        qDebug() << "Xbmc::MySQL";

    if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcMysql) {
        MediaCenterInterface *interface = m_mediaCenters.at(1);
        static_cast<XbmcSql*>(interface)->connectMysql(Settings::instance()->xbmcMysqlHost(), Settings::instance()->xbmcMysqlDatabase(),
                                                       Settings::instance()->xbmcMysqlUser(), Settings::instance()->xbmcMysqlPassword());
        interface = m_mediaCentersTvShow.at(1);
        static_cast<XbmcSql*>(interface)->connectMysql(Settings::instance()->xbmcMysqlHost(), Settings::instance()->xbmcMysqlDatabase(),
                                                       Settings::instance()->xbmcMysqlUser(), Settings::instance()->xbmcMysqlPassword());
        interface = m_mediaCentersConcert.at(1);
        static_cast<XbmcSql*>(interface)->connectMysql(Settings::instance()->xbmcMysqlHost(), Settings::instance()->xbmcMysqlDatabase(),
                                                       Settings::instance()->xbmcMysqlUser(), Settings::instance()->xbmcMysqlPassword());
    } else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcSqlite) {
        MediaCenterInterface *interface = m_mediaCenters.at(2);
        static_cast<XbmcSql*>(interface)->connectSqlite(Settings::instance()->xbmcSqliteDatabase());

        interface = m_mediaCentersTvShow.at(2);
        static_cast<XbmcSql*>(interface)->connectSqlite(Settings::instance()->xbmcSqliteDatabase());

        interface = m_mediaCentersConcert.at(2);
        static_cast<XbmcSql*>(interface)->connectSqlite(Settings::instance()->xbmcSqliteDatabase());
    }
}

/**
 * @brief Calls the shutdown function in every MediaCenterInterface
 */
void Manager::shutdownMediaCenterInterfaces()
{
    for (int i=0, n=m_mediaCenters.count() ; i<n ; ++i)
        m_mediaCenters.at(i)->shutdown();
}

/**
 * @brief Returns the active MediaCenterInterface
 * @return Instance of a MediaCenterInterface
 */
MediaCenterInterface *Manager::mediaCenterInterface()
{
    if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcXml)
        return m_mediaCenters.at(0);
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcMysql)
        return m_mediaCenters.at(1);
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcSqlite)
        return m_mediaCenters.at(2);

    return m_mediaCenters.at(0);
}

/**
 * @brief Returns the active MediaCenterInterface for TV Shows
 * @return Instance of a MediaCenterinterface
 */
MediaCenterInterface *Manager::mediaCenterInterfaceTvShow()
{
    if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcXml)
        return m_mediaCentersTvShow.at(0);
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcMysql)
        return m_mediaCentersTvShow.at(1);
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcSqlite)
        return m_mediaCentersTvShow.at(2);

    return m_mediaCentersTvShow.at(0);
}

/**
 * @brief Returns the active MediaCenterInterface for Concerts
 * @return Instance of a MediaCenterinterface
 */
MediaCenterInterface *Manager::mediaCenterInterfaceConcert()
{
    if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcXml)
        return m_mediaCentersConcert.at(0);
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcMysql)
        return m_mediaCentersConcert.at(1);
    else if (Settings::instance()->mediaCenterInterface() == MediaCenterInterfaces::XbmcSqlite)
        return m_mediaCentersConcert.at(2);

    return m_mediaCentersConcert.at(0);
}

/**
 * @brief Returns an instance of the movie file searcher
 * @return Instance of movie searcher
 */
MovieFileSearcher *Manager::movieFileSearcher()
{
    return m_movieFileSearcher;
}

/**
 * @brief Returns an instance of the tv show file searcher
 * @return Instance of tv show file searcher
 */
TvShowFileSearcher *Manager::tvShowFileSearcher()
{
    return m_tvShowFileSearcher;
}

/**
 * @brief Returns an instance of the concert file searcher
 * @return Instance of tv show file searcher
 */
ConcertFileSearcher *Manager::concertFileSearcher()
{
    return m_concertFileSearcher;
}

/**
 * @brief Returns a list of all movie scrapers
 * @return List of pointers of movie scrapers
 */
QList<ScraperInterface*> Manager::scrapers()
{
    return m_scrapers;
}

/**
 * @brief Returns a list of all tv scrapers
 * @return List of pointers of tv scrapers
 */
QList<TvScraperInterface*> Manager::tvScrapers()
{
    return m_tvScrapers;
}

/**
 * @brief Returns a list of all concert scrapers
 * @return List of pointers of concert scrapers
 */
QList<ConcertScraperInterface*> Manager::concertScrapers()
{
    return m_concertScrapers;
}

/**
 * @brief Returns an instance of the MovieModel
 * @return Instance of the MovieModel
 */
MovieModel *Manager::movieModel()
{
    return m_movieModel;
}

/**
 * @brief Returns an instance of the TvShowModel
 * @return Instance of the TvShowModel
 */
TvShowModel *Manager::tvShowModel()
{
    return m_tvShowModel;
}

/**
 * @brief Returns an instance of the ConcertModel
 * @return Instance of the ConcertModel
 */
ConcertModel *Manager::concertModel()
{
    return m_concertModel;
}

/**
 * @brief Returns an instance of the TvShowProxyModel
 * @return Instance of the TvShowProxyModel
 */
TvShowProxyModel *Manager::tvShowProxyModel()
{
    return m_tvShowProxyModel;
}