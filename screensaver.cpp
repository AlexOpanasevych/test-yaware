#include "screensaver.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QPixmap>
#include <QtConcurrent>
#include <QSqlError>
#include <QImageReader>

namespace {
    const char * fmt = "png";
//    const uint32_t hashSize = 64;
}

ScreenSaver::ScreenSaver(QObject *parent) : QObject(parent)
{

    connect(&_screenshotTimer, &QTimer::timeout, this, &ScreenSaver::makeScreenshot);
    connect(&_watcher, &QFutureWatcher<int>::finished, this, [this](){
        save(_future.result());
    });

    _screenshotTimer.setInterval(std::chrono::minutes(1));


    auto byteArray = _database->imageModel()->record(_database->imageModel()->rowCount() - 1).value("image").toByteArray();
    qDebug() << QImageReader::supportedImageFormats();
    _prevScreenImage = QImage::fromData(QByteArray::fromBase64(byteArray), fmt);
    qDebug() << _prevScreenImage;

}

void ScreenSaver::makeScreenshot()
{
    auto screen = QGuiApplication::primaryScreen();
    if(!screen)
        return;
    _currentScreenImage = screen->grabWindow(0).toImage();


    difference();

    _prevScreenImage = _currentScreenImage;
//    _prevImageHash = _currentImageHash;
}

void ScreenSaver::start()
{
    _screenshotTimer.start();
    setState(Running);
}

void ScreenSaver::stop()
{
    _screenshotTimer.stop();
    setState(Stopped);
}

void ScreenSaver::save(int difference)
{
    QSqlRecord newRecord;
    QSqlField imageField("image", QVariant::ByteArray);
    QByteArray imageData;
    QBuffer buff(&imageData);
    _currentScreenImage.save(&buff, fmt);
    imageField.setValue(imageData.toBase64());
    newRecord.append(imageField);
    QSqlField diffField("diff", QVariant::Int);
    diffField.setValue(difference);
    newRecord.append(diffField);
    QSqlField hashField("hash", QVariant::ByteArray);
    hashField.setValue(QCryptographicHash::hash(imageData, QCryptographicHash::Md5));
    newRecord.append(hashField);
    _database->imageModel()->insertRecord(0, newRecord);
    _database->imageModel()->submitAll();

}

void ScreenSaver::difference()
{


    QImage prevCopy = _prevScreenImage.copy();
    QImage currentCopy = _currentScreenImage.copy();

    _future = QtConcurrent::run([prevCopy, currentCopy, this](){

        if(prevCopy.isNull()) {
            return -1; // first image
        }
        int pixelDiff = 0;

        for(int i = 0; i < currentCopy.width(); i++) {
            for(int j = 0; j < currentCopy.height(); j++) {
                if(prevCopy.pixel(i, j) != currentCopy.pixel(i, j))
                    pixelDiff++;
            }
        }

        return (int)std::lround(100 * (1. - (pixelDiff / double(currentCopy.width() * currentCopy.height()))));
//        _currentImageHash = ::dhash(currentCopy, hashSize);
//        return ::hammingDistance(_prevImageHash, _currentImageHash);
    });

    _watcher.setFuture(_future);

}



Database *ScreenSaver::database() const
{
    return _database;
}

const ScreenSaver::State &ScreenSaver::state() const
{
    return _state;
}

void ScreenSaver::setState(const State &newState)
{
    if (_state == newState)
        return;
    _state = newState;
    emit stateChanged();
}
