#include "chewing.h"
#include <QtTest/QtTest>

class TestChewing : public QObject {
  Q_OBJECT
private:
  Chewing c;
private slots:
  void getSymbol() {
    c.handleReset();
    QString symbol = c.getSymbol();

    QString expect = QString("， 、 。 ． ？ ！ ； ︰ ‧ ‥ ﹐ ﹒ ˙ · ‘ ’ “ ” 〝 "
                             "〞 ‵ ′ 〃 ～ ＄ ％ ＠ ＆ ＃ ＊ ");
    QVERIFY(QString::compare(symbol, expect));
  }
  void getCandidate() {
    c.handleReset();
    c.handleDefault("ㄒ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    c.handleDefault(" ");
    QString candidate = c.getCandidate();
    QCOMPARE(candidate,
             QString("心 新 辛 薪 欣 鋅 馨 鑫 莘 炘 歆 芯 昕 訢 鈊 盺 兟 廞 忻 "
                     "妡 噷 掀 伈 伒 忥 杺 斪 惢 焮 騂 邤 俽 惞 忄 㣺 "));
  }
  void getPreedit() {
    c.handleReset();
    c.handleDefault("ㄒ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    c.handleDefault(" ");
    c.handleDefault("ㄎ");
    c.handleDefault("ㄨ");
    c.handleDefault("ˋ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    c.handleDefault(" ");
    QString preedit = c.getPreedit();
    QCOMPARE(preedit, QString("新酷音"));
  }
};

QTEST_MAIN(TestChewing)
#include "test_chewing.moc"
