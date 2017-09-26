#include "chewing.h"

Chewing::Chewing(QQuickItem *parent):
    QQuickItem(parent)
{
    this->codec = QTextCodec::codecForName("UTF-8");
    this->ct =chewing_new();
    chewing_set_maxChiSymbolLen(this->ct, 10);
    chewing_set_candPerPage(this->ct, 9);
}

Chewing::~Chewing()
{
    chewing_delete(this->ct);
}

char convert_to_Eng(QString input_bopomofo){
    typedef struct bopomofo_to_Eng{
         QString bopomofo;
         char Eng;
    }BtoE;

    BtoE table[41] =
    {
        {"ㄅ", '1'},{"ㄉ",'2'},{"ˇ",'3'},{"ˋ",'4'},{"ㄓ",'5'},{"ˊ",'6'},
        {  "˙", '7' },{  "ㄚ", '8' },{  "ㄞ", '9' },{  "ㄢ", '0' },{  "ㄦ", '-' },{  "ㄆ", 'q' },
        {  "ㄊ", 'w' },{  "ㄍ", 'e' },{  "ㄐ", 'r' },{  "ㄔ", 't' },{  "ㄗ", 'y' },{  "ㄧ", 'u' },
        {  "ㄛ", 'i' },{  "ㄟ", 'o' },{  "ㄣ", 'p' },{  "ㄇ", 'a' },{  "ㄋ", 's' },{  "ㄎ", 'd' },
        {  "ㄑ", 'f' },{  "ㄕ", 'g' },{  "ㄘ", 'h' },{  "ㄨ", 'j' },{  "ㄜ", 'k' },{  "ㄠ", 'l' },
        {  "ㄤ", ';' },{  "ㄈ", 'z' },{  "ㄌ", 'x' },{  "ㄏ", 'c' },{  "ㄒ", 'v' },{  "ㄖ", 'b' },
        {  "ㄙ", 'n' },{  "ㄩ", 'm' },{  "ㄝ", ',' },{  "ㄡ", '.' },{  "ㄥ", '/' },
    };

    for(int i=0;i<=40;i++){
       if(input_bopomofo==table[i].bopomofo){
           return table[i].Eng;
       }
    }
    return input_bopomofo.toLocal8Bit ().data()[0];
}

/*generate cnadidate string for Chinese words or symbols*/
QString generateCandidate( ChewingContext* ctx){
    QString candidate_string;
    chewing_cand_open(ctx);
    while(chewing_cand_hasNext(ctx)) {
        char *str = chewing_cand_String(ctx);
        if (str) {
            candidate_string +=  QString(QByteArray(str));
            candidate_string +=" ";
            chewing_free(str);
        }
    }
    chewing_cand_close(ctx);
    return candidate_string;
}

/*Reset ChewingContext*/
void Chewing::handleReset(){
    chewing_delete(this->ct);
    this->ct =chewing_new();
}

/*This function will transfer pressed key to chewing engine.*/
void Chewing::handleDefault(QString str){
    chewing_handle_Default(this->ct,convert_to_Eng(str));
}

/*This function will transfer space key to chewing engine
  when you don't have keyboard to input.*/
void Chewing::handleSpace(){
    chewing_handle_Space(this->ct);
}

/*This function will transfer backspace key to chewing engine.*/
void Chewing::handleBackSpace(){
    chewing_handle_Backspace(this->ct);
}

/*This function will transfer enter key to chewing engine
  when you don't have keyboard to input.*/
QString Chewing::handleEnter(){
    chewing_handle_Enter(this->ct);
    QString buf = chewing_commit_String(this->ct);
    return buf;
}

/*This function will return preedit string that include bopomofo.*/
QString Chewing::getPreedit(){
    QString preedit_string;
    if (chewing_buffer_Check(this->ct)) {
        char *buf = chewing_buffer_String(this->ct);

        if (buf) {
            preedit_string = QString(QByteArray(buf));
            chewing_free(buf);
        }
    }

    const char *bopomofo_str =chewing_bopomofo_String_static(this->ct);
    if (bopomofo_str) {
        preedit_string += QString(QByteArray(bopomofo_str));
    }

    return preedit_string;
}

/*This function will return candidate string of Chinese words.*/
QString Chewing::getCandidate(){
    return generateCandidate(this->ct);
}
/*This function will return candidate string of symbols.*/
QString Chewing::getSymbol(){
    this->ct =chewing_new();
    chewing_handle_Default(this->ct,'`');
    chewing_handle_Down(this->ct);
    chewing_cand_Enumerate(this->ct);
    chewing_handle_Default(this->ct, '3');
    return generateCandidate(this->ct);
}

