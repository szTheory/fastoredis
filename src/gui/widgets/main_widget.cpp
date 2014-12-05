#include "gui/widgets/main_widget.h"

#include "gui/shortcuts.h"
#include "gui/main_tab_bar.h"
#include "gui/gui_factory.h"
#include "gui/widgets/query_widget.h"

namespace fastoredis
{
    MainWidget::MainWidget(QWidget *parent)
        : QTabWidget(parent)
    {
        MainTabBar *tab = new MainTabBar(this);
        VERIFY(connect(tab, SIGNAL(createdNewTab()), this, SLOT(createNewTab())));
        VERIFY(connect(tab, SIGNAL(reloadedTab()), this, SLOT(reloadeCurrentTab())));
        VERIFY(connect(tab, SIGNAL(duplicatedTab()), this, SLOT(duplicateCurrentTab())));
        VERIFY(connect(tab, SIGNAL(closedOtherTabs()), this, SLOT(closedOtherTabs())));
        VERIFY(connect(tab, SIGNAL(closedTab()), this, SLOT(closeCurrentTab())));
        VERIFY(connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int))));
        setTabBar(tab);
        setTabsClosable(true);
        setElideMode(Qt::ElideRight);
        setMovable(true);
        setDocumentMode(true);
    }

    void MainWidget::openConsole(IServerPtr server)
    {
        if(server){
            QueryWidget *queryWidget = new QueryWidget(server);
            addWidgetToTab(queryWidget, server->name());
        }
    }

    QueryWidget *MainWidget::currentWidget() const
    {
        return qobject_cast<QueryWidget *>(QTabWidget::currentWidget());
    }

    QueryWidget *MainWidget::widget(int index) const
    {
        return qobject_cast<QueryWidget *>(QTabWidget::widget(index));
    }

    void MainWidget::closeTab(int index)
    {
        QueryWidget * shw = widget(index);
        if(shw){
            removeTab(index);
            delete shw;
        }
    }

    void MainWidget::createNewTab()
    {
        int curIndex = currentIndex();
        QueryWidget * shw = widget(curIndex);
        if(shw){
            openNewTab(shw, tabText(curIndex), QString());
        }
    }

    void MainWidget::duplicateCurrentTab()
    {
        int curIndex = currentIndex();
        QueryWidget * shw = widget(curIndex);
        if(shw){
           openNewTab(shw, tabText(curIndex), shw->inputText());
        }
    }

    void MainWidget::reloadeCurrentTab()
    {
        int curIndex = currentIndex();
        QueryWidget *shw = widget(curIndex);
        if(shw){
            shw->reload();
        }
    }

    void MainWidget::closeCurrentTab()
    {
        int curIndex = currentIndex();
        closeTab(curIndex);
    }

    void MainWidget::closedOtherTabs()
    {
        int curIndex = currentIndex();
        tabBar()->moveTab(curIndex, 0);
        while (count() > 1) {
            closeTab(1);
        }
    }

    void MainWidget::addWidgetToTab(QWidget* wid, const QString& title)
    {
        addTab(wid, GuiFactory::instance().redisConnectionIcon(), title);
        setCurrentWidget(wid);
    }

    void MainWidget::openNewTab(QueryWidget* src, const QString& title, const QString& text)
    {
        QueryWidget *newWid = src->clone(text);
        DCHECK(newWid);
        addWidgetToTab(newWid, title);
    }

    void MainWidget::nextTab()
    {
        int index = currentIndex();
        int tabsCount = count();
        if (index == tabsCount - 1)
        {
            setCurrentIndex(0);
            return;
        }
        if (index >= 0 && index < tabsCount - 1)
        {
            setCurrentIndex(index + 1);
            return;
        }
    }

    void MainWidget::previousTab()
    {
        int index = currentIndex();
        if (index == 0)
        {
            setCurrentIndex(count() - 1);
            return;
        }

        if (index > 0)
        {
            setCurrentIndex(index - 1);
            return;
        }
    }

    /**
    * @brief Overrides QTabWidget::keyPressEvent() in order to intercept
    * tab close key shortcuts (Ctrl+F4 and Ctrl+W)
    */
    void MainWidget::keyPressEvent(QKeyEvent *keyEvent)
    {
        if (isCloseTabShortcut(keyEvent))
        {
            int index = currentIndex();
            closeTab(index);
            return;
        }

        if (isCloseTabShortcut(keyEvent)) {
            previousTab();
            return;
        }
        else if (isNextTabShortcut(keyEvent)) {
            nextTab();
            return;
        }
        else if (isNewTabShortcut(keyEvent)) {
            createNewTab();
            return;
        }
        else if (isDuplicateTabShortcut(keyEvent)) {
            duplicateCurrentTab();
            return;
        }
        else if (isSetFocusOnQueryLineShortcut(keyEvent)) {
            QueryWidget* wid = currentWidget();
            if(wid){
                wid->setFocus();
            }
            return;
        }

        QTabWidget::keyPressEvent(keyEvent);
    }
}

