/*
*  Copyright 2019  Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of Latte-Dock
*
*  Latte-Dock is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  Latte-Dock is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WINDOWSYSTEMWINDOWSTRACKER_H
#define WINDOWSYSTEMWINDOWSTRACKER_H

// local
#include "../windowinfowrap.h"

// Qt
#include <QObject>

#include <QHash>
#include <QMap>

namespace Latte {
class View;
namespace WindowSystem {
class AbstractWindowInterface;
class SchemeColors;
namespace Tracker {
class LastActiveWindow;
class TrackedViewInfo;
}
}
}

namespace Latte {
namespace WindowSystem {
namespace Tracker {

class Windows : public QObject {
    Q_OBJECT

public:
    Windows(AbstractWindowInterface *parent);
    ~Windows() override;

    void addView(Latte::View *view);
    void removeView(Latte::View *view);

    bool enabled(Latte::View *view);
    void setEnabled(Latte::View *view, const bool enabled);

    bool activeWindowMaximized(Latte::View *view) const;
    bool activeWindowTouching(Latte::View *view) const;
    bool existsWindowActive(Latte::View *view) const;
    bool existsWindowMaximized(Latte::View *view) const;
    bool existsWindowTouching(Latte::View *view) const;
    SchemeColors *activeWindowScheme(Latte::View *view) const;
    SchemeColors *touchingWindowScheme(Latte::View *view) const;
    LastActiveWindow *lastActiveWindow(Latte::View *view);

    bool isValidFor(const WindowId &wid) const;
    QIcon iconFor(const WindowId &wid);
    QString appNameFor(const WindowId &wid);
    WindowInfoWrap infoFor(const WindowId &wid) const;

    void setPlasmaDesktop(WindowId wid);

    AbstractWindowInterface *wm();

signals:
    void enabledChanged(const Latte::View *view);
    void activeWindowMaximizedChanged(const Latte::View *view);
    void activeWindowTouchingChanged(const Latte::View *view);
    void existsWindowActiveChanged(const Latte::View *view);
    void existsWindowMaximizedChanged(const Latte::View *view);
    void existsWindowTouchingChanged(const Latte::View *view);
    void activeWindowSchemeChanged(const Latte::View *view);
    void touchingWindowSchemeChanged(const Latte::View *view);

    //! overloading WM signals in order to update first m_windows and afterwards
    //! inform consumers for window changes
    void activeWindowChanged(const WindowId &wid);
    void windowChanged(const WindowId &wid);
    void windowRemoved(const WindowId &wid);

private slots:
    void updateAvailableScreenGeometries();

private:
    void init();
    void initViewHints(Latte::View *view);
    void cleanupFaultyWindows();

    void updateViewsHints();
    void updateHints(Latte::View *view);

    void setActiveWindowMaximized(Latte::View *view, bool activeMaximized);
    void setActiveWindowTouching(Latte::View *view, bool activeTouching);
    void setExistsWindowActive(Latte::View *view, bool windowActive);
    void setExistsWindowMaximized(Latte::View *view, bool windowMaximized);
    void setExistsWindowTouching(Latte::View *view, bool windowTouching);
    void setActiveWindowScheme(Latte::View *view, WindowSystem::SchemeColors *scheme);
    void setTouchingWindowScheme(Latte::View *view, WindowSystem::SchemeColors *scheme);

    bool inCurrentDesktopActivity(const WindowInfoWrap &winfo);
    bool intersects(Latte::View *view, const WindowInfoWrap &winfo);
    bool isActive(const WindowInfoWrap &winfo);
    bool isActiveInViewScreen(Latte::View *view, const WindowInfoWrap &winfo);
    bool isMaximizedInViewScreen(Latte::View *view, const WindowInfoWrap &winfo);
    bool isTouchingView(Latte::View *view, const WindowSystem::WindowInfoWrap &winfo);
    bool isTouchingViewEdge(Latte::View *view, const WindowInfoWrap &winfo);

private:
    AbstractWindowInterface *m_wm;
    QHash<Latte::View *, TrackedViewInfo *> m_views;
    QMap<WindowId, WindowInfoWrap > m_windows;

};

}
}
}

#endif