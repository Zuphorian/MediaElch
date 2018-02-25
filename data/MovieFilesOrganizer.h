#ifndef MOVIEFILESORGANIZER_H
#define MOVIEFILESORGANIZER_H

#include <QDir>
#include <QThread>

#include "globals/Globals.h"
#include "movies/Movie.h"

/**
 * @brief The MovieFilesOrganizer class
 */
class MovieFilesOrganizer : public QThread
{
    Q_OBJECT
public:
    explicit MovieFilesOrganizer(QObject *parent = 0);
    ~MovieFilesOrganizer();
    void canceled(QString msg);
    void moveToDirs(QString dir);
};

#endif // MOVIEFILESORGANIZER_H

