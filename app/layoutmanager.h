/*
*  Copyright 2017  Smith AR <audoban@openmailbox.org>
*                  Michail Vourlakos <mvourlakos@gmail.com>
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

#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

// local
#include "launcherssignals.h"
#include "settings/settingsdialog.h"

// Qt
#include <QAction>
#include <QObject>
#include <QPointer>

// KDE
#include <KLocalizedString>

namespace Plasma {
class Containment;
class Types;
}

namespace KActivities {
class Controller;
}

namespace Latte {
class Corona;
class Importer;
class CentralLayout;
class LaunchersSignals;
class SharedLayout;
class View;
namespace Layout {
class GenericLayout;
}
}

namespace Latte {

//! This class is responsible to manipulate all layouts.
//! add,remove,rename, update configurations etc.
class LayoutManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentLayoutName READ currentLayoutName NOTIFY currentLayoutNameChanged)

    Q_PROPERTY(QStringList layouts READ layouts NOTIFY layoutsChanged)
    Q_PROPERTY(QStringList menuLayouts READ menuLayouts NOTIFY menuLayoutsChanged)

    Q_PROPERTY(LaunchersSignals *launchersSignals READ launchersSignals NOTIFY launchersSignalsChanged)

public:
    LayoutManager(QObject *parent = nullptr);
    ~LayoutManager() override;

    Latte::Corona *corona();
    Importer *importer();

    void load();
    void loadLayoutOnStartup(QString layoutName);
    void unload();
    void hideAllViews();
    void pauseLayout(QString layoutName);
    void syncLatteViewsToScreens();
    void syncActiveLayoutsToOriginalFiles();

    bool latteViewExists(Latte::View *view) const;
    bool layoutExists(QString layoutName) const;

    QString shouldSwitchToLayout(QString activityId);

    QString currentLayoutName() const;
    QString defaultLayoutName() const;

    QStringList layouts() const;
    QStringList menuLayouts() const;
    QStringList presetsPaths() const;
    QStringList storedSharedLayouts() const;

    Types::LayoutsMemoryUsage memoryUsage() const;
    void setMemoryUsage(Types::LayoutsMemoryUsage memoryUsage);

    //! returns an central layout with that #id (name), it returns null if such
    //! layout cant be found
    CentralLayout *centralLayout(QString id) const;
    int centralLayoutPos(QString id) const;
    SharedLayout *sharedLayout(QString id) const;
    //! return an central or shared layout with #id (name), it returns null if such
    //! loaded layout was not found
    Layout::GenericLayout *layout(QString id) const;

    //! returns the current and central layout based on activities and user preferences
    CentralLayout *currentLayout() const;
    LaunchersSignals *launchersSignals();

    QStringList activities();
    QStringList runningActivities();
    QStringList orphanedActivities(); //! These are activities that haven't been assigned to specific layout

    void importDefaultLayout(bool newInstanceIfPresent = false);
    void importPresets(bool includeDefault = false);

    bool registerAtSharedLayout(CentralLayout *central, QString id);

public slots:
    void showAboutDialog();

    void hideLatteSettingsDialog();
    Q_INVOKABLE void showLatteSettingsDialog(int page = Latte::Types::LayoutPage);

    //! switch to specified layout, default previousMemoryUsage means that it didn't change
    Q_INVOKABLE bool switchToLayout(QString layoutName, int previousMemoryUsage = -1);

    Q_INVOKABLE int layoutsMemoryUsage();

    //! creates a new layout with layoutName based on the preset
    Q_INVOKABLE QString newLayout(QString layoutName, QString preset = i18n("Default"));

    Q_INVOKABLE QStringList centralLayoutsNames();
    Q_INVOKABLE QStringList sharedLayoutsNames();

signals:
    void centralLayoutsChanged();
    void currentLayoutChanged();
    void currentLayoutNameChanged();
    void launchersSignalsChanged();
    void layoutsChanged();
    void menuLayoutsChanged();

    void currentLayoutIsSwitching(QString layoutName);

private slots:
    void currentActivityChanged(const QString &id);
    void showInfoWindowChanged();
    void syncMultipleLayoutsToActivities(QString layoutForOrphans = QString());
    void unloadSharedLayout(SharedLayout *layout);

private:
    void addLayout(CentralLayout *layout);
    void cleanupOnStartup(QString path); //!remove deprecated or oldstyle config options
    void clearSharedLayoutsFromAssigned();
    void clearUnloadedContainmentsFromLinkedFile(QStringList containmentsIds, bool bypassChecks = false);
    void confirmDynamicSwitch();

    //! it is used just in order to provide translations for the presets
    void ghostForTranslatedPresets();

    void importLatteLayout(QString layoutPath);
    void importPreset(int presetNo, bool newInstanceIfPresent = false);
    void loadLatteLayout(QString layoutPath);
    void loadLayouts();
    void setMenuLayouts(QStringList layouts);
    void showInfoWindow(QString info, int duration, QStringList activities = {"0"});
    void updateCurrentLayoutNameInMultiEnvironment();

    bool layoutIsAssigned(QString layoutName);

    QString layoutPath(QString layoutName);

    QStringList validActivities(QStringList currentList);

private:
    bool m_multipleModeInitialized{false};

    QString m_currentLayoutNameInMultiEnvironment;
    QString m_shouldSwitchToLayout;

    QStringList m_layouts;
    QStringList m_menuLayouts;
    QStringList m_presetsPaths;
    QStringList m_sharedLayoutIds;

    QHash<const QString, QString> m_assignedLayouts;

    QTimer m_dynamicSwitchTimer;

    QPointer<Latte::SettingsDialog> m_latteSettingsDialog;

    Latte::Corona *m_corona{nullptr};
    Importer *m_importer{nullptr};
    LaunchersSignals *m_launchersSignals{nullptr};

    QList<CentralLayout *> m_centralLayouts;
    QList<SharedLayout *> m_sharedLayouts;

    KActivities::Controller *m_activitiesController;


    friend class SettingsDialog;
};

}

#endif // LAYOUTMANAGER_H
