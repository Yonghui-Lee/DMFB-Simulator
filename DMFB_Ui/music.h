#ifndef MUSIC_H
#define MUSIC_H

#include <QSound>
#include <map>
using std::map;




class Music
{
public:
    Music();
    void loadMusic();//载入音乐
    void playMusic(QString name);//播放音乐name

private:
    map<QString,QString> musicList;
    QSound *background;
};


extern Music music;
#endif // MUSIC_H
