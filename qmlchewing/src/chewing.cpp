#include "chewing.h"

namespace {

ChewingContext *new_context() {
  ChewingContext *ct = chewing_new();
  if (!ct) {
    qFatal("Chewing: failed to create ChewingContext");
    return nullptr;
  }
  chewing_set_maxChiSymbolLen(ct, 10);
  chewing_set_candPerPage(ct, 9);
  return ct;
}

} // namespace

Chewing::Chewing(QQuickItem *parent) : QQuickItem(parent) {
  this->codec = QTextCodec::codecForName("UTF-8");
  this->ct = new_context();
}

Chewing::~Chewing() { chewing_delete(this->ct); }

char convert_to_Eng(const QString &input_bopomofo) {
  struct bopomofo_to_Eng {
    QChar bopomofo;
    char Eng;
  };

  static const struct bopomofo_to_Eng table[] = {
      {u'ㄅ', '1'}, {u'ㄉ', '2'}, {u'ˇ', '3'},  {u'ˋ', '4'},  {u'ㄓ', '5'},
      {u'ˊ', '6'},  {u'˙', '7'},  {u'ㄚ', '8'}, {u'ㄞ', '9'}, {u'ㄢ', '0'},
      {u'ㄦ', '-'}, {u'ㄆ', 'q'}, {u'ㄊ', 'w'}, {u'ㄍ', 'e'}, {u'ㄐ', 'r'},
      {u'ㄔ', 't'}, {u'ㄗ', 'y'}, {u'ㄧ', 'u'}, {u'ㄛ', 'i'}, {u'ㄟ', 'o'},
      {u'ㄣ', 'p'}, {u'ㄇ', 'a'}, {u'ㄋ', 's'}, {u'ㄎ', 'd'}, {u'ㄑ', 'f'},
      {u'ㄕ', 'g'}, {u'ㄘ', 'h'}, {u'ㄨ', 'j'}, {u'ㄜ', 'k'}, {u'ㄠ', 'l'},
      {u'ㄤ', ';'}, {u'ㄈ', 'z'}, {u'ㄌ', 'x'}, {u'ㄏ', 'c'}, {u'ㄒ', 'v'},
      {u'ㄖ', 'b'}, {u'ㄙ', 'n'}, {u'ㄩ', 'm'}, {u'ㄝ', ','}, {u'ㄡ', '.'},
      {u'ㄥ', '/'},
  };

  if (input_bopomofo.isEmpty()) {
    return '\0';
  }
  const QChar pressed = input_bopomofo.at(0);
  for (unsigned i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
    if (pressed == table[i].bopomofo) {
      return table[i].Eng;
    }
  }
  return pressed.unicode() < 0x80 ? static_cast<char>(pressed.unicode()) : '\0';
}

QString generateCandidate(ChewingContext *ctx) {
  QString candidate_string;
  if (chewing_cand_open(ctx) != 0) {
    return candidate_string;
  }
  chewing_cand_Enumerate(ctx);
  while (chewing_cand_hasNext(ctx)) {
    char *str = chewing_cand_String(ctx);
    if (str) {
      candidate_string += QString::fromUtf8(str);
      candidate_string += QLatin1Char(' ');
      chewing_free(str);
    }
  }
  chewing_cand_close(ctx);
  return candidate_string;
}

void Chewing::handleReset() {
  chewing_delete(this->ct);
  this->ct = new_context();
}

void Chewing::handleDefault(const QString &str) {
  chewing_handle_Default(this->ct, convert_to_Eng(str));
}

void Chewing::handleSpace() { chewing_handle_Space(this->ct); }

void Chewing::handleBackSpace() { chewing_handle_Backspace(this->ct); }

QString Chewing::handleEnter() {
  chewing_handle_Enter(this->ct);
  char *buf = chewing_commit_String(this->ct);
  QString committed = buf ? QString::fromUtf8(buf) : QString();
  chewing_free(buf);
  chewing_delete(this->ct);
  this->ct = new_context();
  return committed;
}

QString Chewing::getPreedit() {
  QString preedit_string;
  if (chewing_buffer_Check(this->ct)) {
    char *buf = chewing_buffer_String(this->ct);

    if (buf) {
      preedit_string = QString::fromUtf8(buf);
      chewing_free(buf);
    }
  }

  const char *bopomofo_str = chewing_bopomofo_String_static(this->ct);
  if (bopomofo_str) {
    preedit_string += QString::fromUtf8(bopomofo_str);
  }

  return preedit_string;
}

QString Chewing::getCandidate() { return generateCandidate(this->ct); }
