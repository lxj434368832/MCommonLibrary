#ifndef MEDIAPLAYERQT_H
#define MEDIAPLAYERQT_H
#include <QFrame>
#include <QMediaPlayer>

namespace Ui {
class MediaPlayerWidget;
}
class IMediaPlayer;
class QStandardItemModel;
class QListWidgetItem;
class WidgetVolumeSlider;


class MediaPlayerWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MediaPlayerWidget(QWidget *parent = 0);
    ~MediaPlayerWidget();

public slots:
void slot_open_video(QList<QListWidgetItem*> list, int current_row);

private slots:
    void on_btnPlayOrPause_clicked();

    void on_btnStop_clicked();

    void on_btnNext_clicked();

    void on_sliderPlayProcess_sliderReleased();

    void on_btnCutPicture_clicked();

    void on_btnCircle_clicked();

    void on_btnVolume_clicked();

    void on_btnFullScreen_clicked();

    void on_lwVideoList_itemDoubleClicked(QListWidgetItem *item);

    //以下为接收相关播放状态的槽
    void slotLengthChanged(qint64 length);
    void slotPositionChanged(qint64 position);
    void slotVolumeChanged(int value);
    void slotPlayStatechanged(QMediaPlayer::State state);
    void slotMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    Ui::MediaPlayerWidget        *ui;
    WidgetVolumeSlider                *m_widgetVoiceSlider;
	QStandardItemModel			*m_modelVideoList;
    IMediaPlayer                         *m_player;
    std::string                                m_strPlayFilePath;
    std::string                                m_strPlayFileName;
    int                                             m_iCurrentIndex = -1;
    bool                                         m_bCircle = true;
};

#endif // MEDIAPLAYERQT_H
