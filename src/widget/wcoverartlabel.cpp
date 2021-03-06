#include "widget/wcoverartlabel.h"

#include <QtDebug>

#include "library/dlgcoverartfullsize.h"
#include "library/coverartutils.h"
#include "util/compatibility.h"

static const QSize s_labelDisplaySize = QSize(100, 100);

WCoverArtLabel::WCoverArtLabel(QWidget* parent)
        : QLabel(parent),
          m_pCoverMenu(new WCoverArtMenu(this)),
          m_pDlgFullSize(new DlgCoverArtFullSize(this, nullptr)),
          m_defaultCover(CoverArtUtils::defaultCoverLocation()) {
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setFrameShape(QFrame::Box);
    setAlignment(Qt::AlignCenter);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(slotCoverMenu(QPoint)));
    connect(m_pCoverMenu, SIGNAL(coverInfoSelected(const CoverInfoRelative&)),
            this, SIGNAL(coverInfoSelected(const CoverInfoRelative&)));
    connect(m_pCoverMenu, SIGNAL(reloadCoverArt()),
            this, SIGNAL(reloadCoverArt()));

    m_defaultCover.setDevicePixelRatio(getDevicePixelRatioF(this));
    m_defaultCover = m_defaultCover.scaled(s_labelDisplaySize * getDevicePixelRatioF(this),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);
    setPixmap(m_defaultCover);
}

WCoverArtLabel::~WCoverArtLabel() {
    delete m_pCoverMenu;
    delete m_pDlgFullSize;
}

void WCoverArtLabel::setCoverArt(const CoverInfo& coverInfo,
                                 QPixmap px) {
    qDebug() << "WCoverArtLabel::setCoverArt" << coverInfo << px.size();

    m_loadedCover = px.scaled(s_labelDisplaySize * getDevicePixelRatioF(this),
            Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_loadedCover.setDevicePixelRatio(getDevicePixelRatioF(this));
    m_pCoverMenu->setCoverArt(coverInfo);

    if (m_loadedCover.isNull()) {
        setPixmap(m_defaultCover);
    } else {
        setPixmap(m_loadedCover);
    }

    QSize frameSize = pixmap()->size() / getDevicePixelRatioF(this);
    frameSize += QSize(2,2); // margin
    setMinimumSize(frameSize);
    setMaximumSize(frameSize);
}

void WCoverArtLabel::slotCoverMenu(const QPoint& pos) {
    m_pCoverMenu->popup(mapToGlobal(pos));
}

void WCoverArtLabel::loadTrack(TrackPointer pTrack) {
    m_pLoadedTrack = pTrack;
}

void WCoverArtLabel::mousePressEvent(QMouseEvent* event) {
    if (m_pCoverMenu->isVisible()) {
        return;
    }

    if (event->button() == Qt::LeftButton) {
        if (m_pDlgFullSize->isVisible()) {
            m_pDlgFullSize->close();
        } else {
            m_pDlgFullSize->init(m_pLoadedTrack);
        }
    }
}

