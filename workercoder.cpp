#include "workercoder.h"

WorkerCoder::WorkerCoder(QObject *parent) : QObject{parent}{

}

QString WorkerCoder::CharToBinary(QChar &c){
    QString text;
    char charaux = c.toLatin1();

    for(int i=0; i<8 ;i++){
        text.prepend(QString::number((charaux >> i) & 1));
    }

    return text;
}

void WorkerCoder::Translate(QString text){

    QString textresult;
    QString textaux;

    if(beginframe){
        if(binarytext)
            textaux = "0b";

        textaux.append("1111111111111111");

        if(commatext)
            textaux.append(",");

        textaux.append(" ");

        textresult = textaux;
    }

    for(int i=0; i<text.size() ;i++){
        if(binarytext)
            textaux = "0b";
        else
            textaux = "";

        if(headertext)
            textaux.append("11100110");


        textaux.append(CharToBinary(text[i]));

        if(commatext)
            textaux.append(",");

        textaux.append(" ");

        textresult.append(textaux);
    }

    emit Translated(textresult);
}

void WorkerCoder::SetInsertBeginFrame(bool value){
    beginframe = value;
}

void WorkerCoder::SetInsertHeader(bool value){
    headertext = value;
}

void WorkerCoder::SetInsert0b(bool value){
    binarytext = value;
}

void WorkerCoder::SetInsertComma(bool value){
    commatext = value;
}
