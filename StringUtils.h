#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include "definefeatures.h"
#include <QObject>
#include <QtCore>


#define CARD_IMAGE_SUFFIX "_card.jpg"
#define CAMERA_IMAGE_SUFFIX "_camera.jpg"

#define DENGJISTATUS_SAVE "1"
#define DENGJISTATUS_UPLOAD "4"

#define CHUZHISTATUS_SAVE "1"
#define CHUZHISTATUS_UPLOAD "1"


class StringUtils : public QObject
{
    Q_OBJECT
public:
    explicit StringUtils(QObject *parent = 0);

    static QString GetCurrentPicturePath();

    static QString GetCurrentLogPath();

signals:

public slots:
};

#endif // STRINGUTILS_H
