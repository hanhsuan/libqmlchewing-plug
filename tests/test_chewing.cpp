#include "chewing.h"
#include <QtTest/QtTest>

#include <QRegularExpression>

/*
 * These tests must pass against any supported libchewing version
 * (0.5.1 through 0.11.x). Assertions are deliberately
 * dictionary-independent and behavior-independent of version quirks:
 * candidate strings and the symbol page depend on the shipped dictionary,
 * and on libchewing <= 0.8.5 the commit buffer is never cleared once
 * filled (the plugin works around that by replacing the context on
 * reset/enter). The plugin's contract is therefore: after handleReset()
 * or handleEnter() the context is completely clean.
 */
class TestChewing : public QObject {
  Q_OBJECT
private:
  Chewing c;

private slots:
  void init() { c.handleReset(); }

  void testEmptyState() {
    QVERIFY(!chewing_buffer_Check(c.context()));
    QCOMPARE(c.getPreedit(), QString());
    QCOMPARE(c.getCandidate(), QString());
    QCOMPARE(chewing_commit_String_static(c.context()), QString());
  }

  void testUnmappedKeyCommitsDirectly() {
    /* uppercase letters are not bopomofo keys in the standard layout on
       any version: they bypass the preedit buffer and are committed
       as-is (<= 0.8.5 lowercases the committed ASCII char, so compare
       case-insensitively) */
    c.handleDefault("A");
    QVERIFY(!chewing_buffer_Check(c.context()));
    QCOMPARE(
        QString::fromUtf8(chewing_commit_String_static(c.context())).toLower(),
        QString("a"));
  }

  void testBopomofoStaysInPreeditUntilSpace() {
    c.handleDefault("ㄒ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    /* unfinished syllable shows only bopomofo, nothing committed yet */
    QVERIFY(!chewing_buffer_Check(c.context()));
    QCOMPARE(c.getPreedit(), QString("ㄒㄧㄣ"));
    QCOMPARE(chewing_commit_String_static(c.context()), QString());
  }

  void testSpaceCommitsSyllableIntoBuffer() {
    c.handleDefault("ㄒ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    c.handleSpace();
    /* space finishes the syllable: the selected (first) candidate moves
       into the buffer, nothing is sent to the caller yet */
    QVERIFY(chewing_buffer_Check(c.context()));
    const QString selected = c.getPreedit();
    QVERIFY(!selected.isEmpty());
    QCOMPARE(chewing_commit_String_static(c.context()), QString());
    /* the selected word must be the first entry of the candidate list */
    QCOMPARE(c.getCandidate()
                 .trimmed()
                 .split(QRegularExpression("\\s+"), Qt::SkipEmptyParts)
                 .value(0),
             selected);
  }

  void testBackspaceClearsBuffer() {
    c.handleDefault("ㄒ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    c.handleSpace();
    QVERIFY(chewing_buffer_Check(c.context()));
    QVERIFY(!c.getPreedit().isEmpty());
    c.handleBackSpace();
    QVERIFY(!chewing_buffer_Check(c.context()));
    QCOMPARE(c.getPreedit(), QString());
  }

  void testHandleEnterFlushesAndLeavesCleanContext() {
    c.handleDefault("ㄒ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    c.handleSpace();
    const QString buffered = c.getPreedit();
    QVERIFY(!buffered.isEmpty());
    /* enter flushes the whole buffer to the caller and empties it */
    QCOMPARE(c.handleEnter(), buffered);
    QVERIFY(!chewing_buffer_Check(c.context()));
    QCOMPARE(c.getPreedit(), QString());
    /* the context is clean: enter has nothing left to commit */
    QCOMPARE(c.handleEnter(), QString());
  }

  void testResetClearsBufferAndStaysUsable() {
    c.handleDefault("ㄒ");
    c.handleDefault("ㄧ");
    c.handleDefault("ㄣ");
    c.handleSpace();
    QVERIFY(chewing_buffer_Check(c.context()));
    c.handleReset();
    /* the replaced context is clean and still usable */
    QVERIFY(!chewing_buffer_Check(c.context()));
    QCOMPARE(c.getPreedit(), QString());
    QCOMPARE(chewing_commit_String_static(c.context()), QString());
    c.handleDefault("ㄅ");
    QCOMPARE(c.getPreedit(), QString("ㄅ"));
  }
};

QTEST_MAIN(TestChewing)
#include "test_chewing.moc"
