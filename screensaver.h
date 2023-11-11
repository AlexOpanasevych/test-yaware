#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <QFutureWatcher>
#include <QQmlEngine>
#include "database.h"

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

    QFutureWatcher<int> _watcher;
    QFuture<int> _future;

    Database* _database = new Database(this);

    State _state = Stopped;
};

#endif // SCREENSAVER_H
