#include "music.h"

#include <QTextStream>
#include <QDebug>
#include <QFileInfo>

Music::Music()
{
    loadMusic();

}




QString loadMusicList[]={
    "move",
    "merge",
    "splitbegin",
    "splitend"
};
int musicTot=4;


void Music::loadMusic(){
    for (int i=0;i<musicTot;i++){
        QString fileDir;
        QTextStream(&fileDir)<<":/music/music/"<<loadMusicList[i]<<".wav";
        qDebug()<<"openMusic: "<<fileDir;
        musicList[loadMusicList[i]]=fileDir;
    }
}

void Music::playMusic(QString name){
    if (musicList.find(name) == musicList.end()){
        qDebug()<<"[ERROR] music not found: "<<name;
        return;
    }
    qDebug()<<"play music: "<<name;
    QSound::play(musicList[name]);
}
