#ifndef MEDIAPLAYERQT_H
#define MEDIAPLAYERQT_H

//#include "../../Framework/include/MFramelessWidget/MFramelessWidget.h"
#include "../MMediaPlayer/IMediaPlayer.h"
#include <QFrame>

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
	//打开视频
	void slot_open_video(unsigned report_id, QList<QListWidgetItem*> list, int current_row);

	//截图路径返回给播放器
	void slot_snapshot_path_to_media_player(std::string path);

signals:
	//媒体播放器获取截图路径
	void signal_media_player_get_snapshot_path(int report_id);
	//媒体播放器截图完成通知
	void signal_media_player_snapshot_done(std::string path);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_btnMaximum_clicked();
    void on_btnClose_clicked();

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
    void slotPlayStatechanged(IMediaPlayer::EPlayState state);

private:
    Ui::MediaPlayerWidget       *ui;
    WidgetVolumeSlider           *m_widgetVoiceSlider;
    IMediaPlayer                 *m_player;
    bool                         m_bCircle = false;
    int                          m_iCurrentIndex = -1;
	int							 m_report_id = 0;
    std::string                m_strPlayFilePath;
    std::string                m_strPlayFileName;
};

#endif // MEDIAPLAYERQT_H
