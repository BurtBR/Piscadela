#include "workercoder.h"

WorkerCoder::WorkerCoder(QObject *parent) : QObject{parent}{

}

void WorkerCoder::Translate(QString text){
    emit Translated(text);
}

void WorkerCoder::SetInsertBeginFrame(bool value){

}

void WorkerCoder::SetInsertHeader(bool value){

}

void WorkerCoder::SetInsert0b(bool value){

}

void WorkerCoder::SetInsertComma(bool value){

}
