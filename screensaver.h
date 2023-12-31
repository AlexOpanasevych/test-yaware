#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <QFutureWatcher>
#include <QQmlEngine>
#include "database.h"
//namespace {

//QByteArray dhash(const QImage & image, int hashSize = 8) {
//    QImage resized = image.scaled(hashSize + 1, hashSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

//    QByteArray hash;
//    for (int y = 0; y < hashSize; ++y) {
//        for (int x = 0; x < hashSize; ++x) {
//            hash.append(resized.pixelColor(x + 1, y).value() > resized.pixelColor(x, y).value() ? '1' : '0');
//        }
//    }

//    return hash;
//}

//int hammingDistance(const QByteArray & hash1, const QByteArray & hash2) {
//    if(hash1.size() != hash2.size())
//        throw std::range_error("hash1.size() != hash2.size()");  // actually don't know what return here, maybe throw exception
//    int distance = 0;
//    for (int i = 0; i < hash1.size(); ++i) {
//        if (hash1.at(i) != hash2.at(i)) {
//            ++distance;
//        }
//    }
//    return distance;
//}
//}

class ScreenSaver : public QObject
{
    Q_OBJECT


    Q_PROPERTY(Database* database READ database CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    enum State {
        Running,
        Stopped
    };
    Q_ENUM(State)

    ScreenSaver(QObject * parent = nullptr);

    static ScreenSaver * instance() {
        static ScreenSaver _screenSaver;
        return &_screenSaver;
    }

    static QObject * qmlInstance(QQmlEngine * engine, QJSEngine *) {
        auto obj = instance();
        engine->setObjectOwnership(obj, QQmlEngine::CppOwnership);
        return obj;
    }

    Q_SLOT void makeScreenshot();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    void save(int difference);

    void difference();

    Database *database() const;

    const State &state() const;
    void setState(const State &newState);

signals:
    void stateChanged();

private:
    QTimer _screenshotTimer;
    QImage _prevScreenImage;
    QImage _currentScreenImage;

//    QByteArray _currentImageHash;
//    QByteArray _prevImageHash;

    QFutureWatcher<int> _watcher;
    QFuture<int> _future;

    Database* _database = new Database(this);

    State _state = Stopped;
};

#endif // SCREENSAVER_H
