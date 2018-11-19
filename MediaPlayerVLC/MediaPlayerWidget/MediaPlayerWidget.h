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
	//����Ƶ
	void slot_open_video(unsigned report_id, QList<QListWidgetItem*> list, int current_row);

	//��ͼ·�����ظ�������
	void slot_snapshot_path_to_media_player(std::string path);

signals:
	//ý�岥������ȡ��ͼ·��
	void signal_media_player_get_snapshot_path(int report_id);
	//ý�岥������ͼ���֪ͨ
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

    //����Ϊ������ز���״̬�Ĳ�
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
