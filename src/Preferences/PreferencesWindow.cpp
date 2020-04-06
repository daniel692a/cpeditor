/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

// Main Frame
#include "Preferences/PreferencesWindow.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QScreen>

PreferencesWindow::PreferencesWindow(QWidget *parent) : QMainWindow(parent)
{
    // set attributes
    hide();
    setWindowTitle("Preferences");

    // set size
    const double PREFERENCES_WINDOW_SIZE_RADIO = 0.6;
    auto screenSize = QApplication::screenAt(parent->pos())->geometry().size();
    resize(screenSize * PREFERENCES_WINDOW_SIZE_RADIO);

    // move to the center of the screen
    move(screenSize.width() * (1 - PREFERENCES_WINDOW_SIZE_RADIO) / 2,
         screenSize.height() * (1 - PREFERENCES_WINDOW_SIZE_RADIO) / 2);

    // setup UI
    splitter = new QSplitter();
    splitter->setChildrenCollapsible(false);
    setCentralWidget(splitter);

    leftWidget = new QWidget();
    splitter->addWidget(leftWidget);

    leftLayout = new QVBoxLayout(leftWidget);

    searchLayout = new QHBoxLayout();
    leftLayout->addLayout(searchLayout);

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search...");
    connect(searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateSearch(const QString &)));
    searchLayout->addWidget(searchEdit);

    homeButton = new QPushButton("Home");
    homeButton->setDisabled(true);
    connect(homeButton, &QPushButton::clicked, this, [this]() { switchToPage(homePage); });
    searchLayout->addWidget(homeButton);

    menuTree = new QTreeWidget();
    menuTree->setHeaderHidden(true);
    connect(menuTree, &QTreeWidget::itemActivated, [this](QTreeWidgetItem *item) { switchToPage(pageWidget[item]); });
    connect(menuTree, &QTreeWidget::itemClicked, [this](QTreeWidgetItem *item) { switchToPage(pageWidget[item]); });
    leftLayout->addWidget(menuTree);

    stackedWidget = new QStackedWidget();
    splitter->addWidget(stackedWidget);

    homePage = new PreferencesHomePage();
    connect(homePage, &PreferencesHomePage::requestPage,
            [this](const QString &path) { switchToPage(getPageWidget(path)); });
    stackedWidget->addWidget(homePage);

    splitter->setSizes({1, 3});

    exitShortcut = new QShortcut(QKeySequence::Cancel, this);
    connect(exitShortcut, SIGNAL(activated()), this, SLOT(close()));

    addPage("Edit", new PreferencesPageTemplate({"Tab Width", "Auto Indent", "Wrap Text", "Auto Complete Parentheses",
                                                 "Auto Remove Parentheses", "Replace Tabs"}));

    addPage("Language/General", new PreferencesPageTemplate({"Default Language"}));

    addPage("Language/Commands",
            new PreferencesPageTemplate({"C++/Compile Command", "C++/Run Arguments", "Java/Compile Command",
                                         "Java/Run Arguments", "Java/Run Command", "Python/Run Arguments",
                                         "Python/Run Command"}));

    addPage("Language/Code Template",
            new PreferencesPageTemplate({"C++/Template Path", "Java/Template Path", "Python/Template Path"}));

    addPage("Appearance", new PreferencesPageTemplate({"Editor Theme", "Font", "Transparency",
                                                       "Show Compile And Run Only", "Display EOLN In Diff"}));

    addPage("Key Bindings",
            new PreferencesPageTemplate({"Hotkey/Compile", "Hotkey/Run", "Hotkey/Compile Run", "Hotkey/Format",
                                         "Hotkey/Kill", "Hotkey/Change View Mode", "Hotkey/Snippets"}));
}

void PreferencesWindow::display()
{
    switchToPage(homePage, true);
    show();
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    activateWindow();
    raise();
}

void PreferencesWindow::updateSearch(const QString &text)
{
    for (int i = 0; i < menuTree->topLevelItemCount(); ++i)
    {
        updateSearch(menuTree->topLevelItem(i), text);
    }
}

void PreferencesWindow::switchToPage(QWidget *page, bool force)
{
    // return if page is nullptr
    if (page == nullptr)
        return;

    // return if it's no need to switch
    if (stackedWidget->currentWidget() == page)
        return;

    // ask for saving changes or not if not force
    if (!force)
    {
        auto current = dynamic_cast<PreferencesPage *>(stackedWidget->currentWidget());
        if (current != nullptr && !current->aboutToExit())
            return;
    }

    // disable home button when it's already at home
    homeButton->setDisabled(page == homePage);

    // switch if everything is OK
    stackedWidget->setCurrentWidget(page);

    auto preferencesPage = dynamic_cast<PreferencesPage *>(page);
    if (preferencesPage != nullptr)
    {
        menuTree->setCurrentItem(pageTreeItem[preferencesPage]);
        preferencesPage->loadSettings();
    }
}

void PreferencesWindow::addPage(const QString &path, PreferencesPage *page, const QStringList &pageContent)
{
    auto parts = path.split('/');

    QTreeWidgetItem *current = getTopLevelItem(parts.front());

    // add if not exists
    if (current == nullptr)
    {
        current = new QTreeWidgetItem({parts.front()});
        menuTree->addTopLevelItem(current);
    }

    // get non-top-level items step by step
    for (int i = 1; i < parts.count(); ++i)
    {
        QTreeWidgetItem *nxt = getChild(current, parts[i]);

        // add if not exists
        if (nxt == nullptr)
        {
            nxt = new QTreeWidgetItem({parts[i]});
            current->addChild(nxt);
        }

        current = nxt;
    }

    page->setPath(path);
    content[current] = pageContent;
    pageWidget[current] = page;
    pageTreeItem[page] = current;
    stackedWidget->addWidget(page);
    connect(page, SIGNAL(settingsApplied(const QString &)), this, SIGNAL(settingsApplied(const QString &)));
}

void PreferencesWindow::addPage(const QString &path, PreferencesPageTemplate *page)
{
    addPage(path, page, page->content());
}

PreferencesPage *PreferencesWindow::getPageWidget(const QString &pagePath)
{
    auto parts = pagePath.split('/');

    QTreeWidgetItem *current = getTopLevelItem(parts.front());

    if (current == nullptr)
        return nullptr;

    // get the non-top-level items step by step
    for (int i = 1; i < parts.count(); ++i)
    {
        QTreeWidgetItem *nxt = getChild(current, parts[i]);
        if (nxt == nullptr)
            return nullptr;
        current = nxt;
    }

    return pageWidget[current];
}

void PreferencesWindow::closeEvent(QCloseEvent *event)
{
    auto current = dynamic_cast<PreferencesPage *>(stackedWidget->currentWidget());
    if (current != nullptr && !current->aboutToExit())
        event->ignore();
    else
        event->accept();
}

void PreferencesWindow::updateSearch(QTreeWidgetItem *item, const QString &text)
{
    if (item == nullptr)
        return;

    item->setExpanded(!text.isEmpty());

    if (text.isEmpty())
    {
        // If text is empty, set all items visible
        item->setHidden(false);
        for (int i = 0; i < item->childCount(); ++i)
            updateSearch(item->child(i), text);
        return;
    }

    bool shouldHide = true;

    if (item->childCount() == 0)
    {
        // check whether the leaf should be hidden or not
        if (pageWidget[item]->path().contains(text, Qt::CaseInsensitive)) // check whether the path contains text
        {
            shouldHide = false;
        }
        else
        {
            // check whether the content contains text
            for (auto s : content[item])
            {
                if (s.contains(text, Qt::CaseInsensitive))
                {
                    shouldHide = false;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < item->childCount(); ++i)
    {
        auto child = item->child(i);
        updateSearch(child, text);
        // If at least one child is not hidden, a non-leaf item is not hidden
        if (!child->isHidden())
            shouldHide = false;
    }

    item->setHidden(shouldHide);
}

QTreeWidgetItem *PreferencesWindow::getTopLevelItem(const QString &text)
{
    for (int i = 0; i < menuTree->topLevelItemCount(); ++i)
    {
        auto item = menuTree->topLevelItem(i);
        if (item->text(0) == text)
        {
            return item;
        }
    }
    return nullptr;
}

QTreeWidgetItem *PreferencesWindow::getChild(QTreeWidgetItem *item, const QString &text)
{
    for (int i = 0; i < item->childCount(); ++i)
    {
        auto child = item->child(i);
        if (child->text(0) == text)
        {
            return child;
        }
    }
    return nullptr;
}
