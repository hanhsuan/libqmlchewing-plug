#ifndef CHEWING_H
#define CHEWING_H

#include <QQuickItem>
#include <QTextCodec>
#include <QString>
#include <chewing/chewing.h>

class Chewing : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Chewing)
private:
    QTextCodec *codec;
    ChewingContext* ct;
public:
    Chewing(QQuickItem *parent = 0);
    ~Chewing();

    /*Reset ChewingContext*/
    Q_INVOKABLE void handleReset();

    /*This function will transfer pressed key to chewing engine.*/
    Q_INVOKABLE void handleDefault(QString);

    /*This function will transfer space key to chewing engine
      when you don't have keyboard this is useful.*/
    Q_INVOKABLE void handleSpace();

    /*This function will transfer backspace key to chewing engine.*/
    Q_INVOKABLE void handleBackSpace();

    /*This function could switch input mode between chinese and others.*/
    Q_INVOKABLE void handleMode();

    /*This function will transfer enter key to chewing engine
      when you don't have keyboard to input.*/
    Q_INVOKABLE QString handleEnter();

    /*This function will return preedit string that include bopomofo.*/
    Q_INVOKABLE QString getPreedit();

    /*This function will return candidate string.*/
    Q_INVOKABLE QString getCandidate();

    /*This function will return Symbol string.*/
    Q_INVOKABLE QString getSymbol();
};

#endif // CHEWING_H
