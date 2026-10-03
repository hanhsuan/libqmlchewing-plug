#ifndef CHEWING_H
#define CHEWING_H

#include <QQuickItem>
#include <QString>
#include <QTextCodec>
#include <chewing/chewing.h>

class Chewing : public QQuickItem {
  Q_OBJECT
  Q_DISABLE_COPY(Chewing)
private:
  QTextCodec *codec;
  ChewingContext *ct;

public:
  explicit Chewing(QQuickItem *parent = nullptr);
  ~Chewing() override;

  /**
   * @brief Reset ChewingContext
   */
  Q_INVOKABLE void handleReset();

  /**
   * @brief Transfer pressed key to chewing engine
   * @param The string sent by the pressed key
   */
  Q_INVOKABLE void handleDefault(const QString &str);

  /**
   * @brief Transfer space key to chewing engine
   */
  Q_INVOKABLE void handleSpace();

  /**
   * @brief Transfer backspace key to chewing engine
   */
  Q_INVOKABLE void handleBackSpace();

  /**
   * @brief Transfer enter key to chewing engine
   * @return The committed string
   */
  Q_INVOKABLE QString handleEnter();

  /**
   * @brief Return preedit string that include bopomofo
   * @return preedit
   */
  Q_INVOKABLE QString getPreedit();

  /**
   * @brief Return candidate string
   * @return candidate
   */
  Q_INVOKABLE QString getCandidate();

  /**
   * @brief Read-only access to the underlying context (mainly for testing)
   */
  const ChewingContext *context() const { return this->ct; }
};

#endif // CHEWING_H
