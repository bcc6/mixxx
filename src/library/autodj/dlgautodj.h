#ifndef DLGAUTODJ_H
#define DLGAUTODJ_H

#include <QWidget>
#include <QString>
#include <QList>

#include "library/autodj/ui_dlgautodj.h"
#include "preferences/usersettings.h"
#include "track/track.h"
#include "library/libraryview.h"
#include "library/library.h"
#include "library/trackcollection.h"
#include "library/autodj/autodjprocessor.h"
#include "controllers/keyboard/keyboardeventfilter.h"

class PlaylistTableModel;
class WTrackTableView;

class DlgAutoDJ : public QWidget, public Ui::DlgAutoDJ {
    Q_OBJECT
  public:
    DlgAutoDJ(QWidget* parent, Library *pLibrary, AutoDJProcessor* pProcessor);
    virtual ~DlgAutoDJ();
    
    void onShow();
    void setTrackTableView(WTrackTableView* pTrackTableView);

  public slots:
    void shufflePlaylistButton(bool buttonChecked);
    void skipNextButton(bool buttonChecked);
    void fadeNowButton(bool buttonChecked);
    void toggleAutoDJButton(bool enable);
    void transitionTimeChanged(int time);
    void transitionSliderChanged(int value);
    void autoDJStateChanged(AutoDJProcessor::AutoDJState state);
    void updateSelectionInfo();

  signals:
    void addRandomButton(bool buttonChecked);
    void loadTrack(TrackPointer tio);
    void loadTrackToPlayer(TrackPointer tio, QString group, bool);
    void trackSelected(TrackPointer pTrack);

  private:
    AutoDJProcessor* m_pAutoDJProcessor;
    PlaylistTableModel* m_pAutoDJTableModel;
    Library* m_pLibrary;
};

#endif //DLGAUTODJ_H
