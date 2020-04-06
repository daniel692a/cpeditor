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

/*
 * This file is auto generated by tools/genSettings.sh
 */

#ifndef SETTINGSINFO_HPP
#define SETTINGSINFO_HPP

#include <QByteArray>
#include <QFont>
#include <QRect>
#include <QString>
#include <QVariant>

struct SettingInfo
{
    QString desc, type, ui;
    QStringList old;
    QVariant def;
    QVariant param;

    QString name() const
    {
        return desc.toLower().replace('+', 'p').replace(' ', '_');
    }
};

const SettingInfo settingInfo[] = {
    {"Tab Width", "int", "", {"tab_stop"}, 4, QList<QVariant>{1, 16}},
    {"Time Limit", "int", "", {}, 5000},
    {"Geometry", "QRect", "", {}, QRect()},
    {"Font", "QFont", "", {}, QFont("monospace", 9)},
    {"Default Language", "QString", "QComboBox", {}, "C++", QStringList{"C++", "Java", "Python"}},
    {"Clang Format/Path", "QString", "PathItem", {"clang_format_binary"}, "clang-format", 0},
    {"Clang Format/Style", "QString", "", {"clang_format_style"}, "BasedOnStyle: Google"},
    {"C++/Template Path", "QString", "PathItem", {"template_cpp"}, "", 1},
    {"C++/Compile Command", "QString", "", {"compile_cpp"}, "g++ -Wall"},
    {"C++/Run Arguments", "QString", "", {"runtime_cpp"}, ""},
    {"Java/Template Path", "QString", "PathItem", {"template_java"}, "", 2},
    {"Java/Compile Command", "QString", "", {"compile_java"}, "javac"},
    {"Java/Run Arguments", "QString", "", {"runtime_java"}, "java"},
    {"Java/Run Command", "QString", "", {"run_java"}, "java"},
    {"Python/Template Path", "QString", "PathItem", {"template_python"}, "", 3},
    {"Python/Run Arguments", "QString", "", {"runtime_python"}, ""},
    {"Python/Run Command", "QString", "", {"run_python"}, "python"},
    {"Editor Theme",
     "QString",
     "QComboBox",
     {},
     "Light",
     QStringList{"Light", "Drakula", "Monkai", "Solarised", "Solarised Dark"}},
    {"Auto Complete Parentheses", "bool", "", {"auto_parenthesis"}, true},
    {"Auto Remove Parentheses", "bool", "", {"auto_remove_parentheses"}, true},
    {"Auto Indent", "bool", "", {}, true},
    {"Auto Save", "bool", "", {"autosave"}, false},
    {"Wrap Text", "bool", "", {}, false},
    {"Beta", "bool", "", {}, false},
    {"Replace Tabs", "bool", "", {}, false},
    {"Save Tests", "bool", "", {}, false},
    {"Maximized Window", "bool", "", {"win_max"}, false},
    {"Check Update", "bool", "", {"update_start_check"}, true},
    {"Auto Format", "bool", "", {"format_on_save"}, false},
    {"Transparency", "int", "QSlider", {}, 100, QList<QVariant>{60, 100}},
    {"View Mode", "QString", "", {}, "split"},
    {"Splitter Size", "QByteArray", "", {"splitter_sizes"}, QByteArray()},
    {"Right Splitter Size", "QByteArray", "", {"right_splitter_sizes"}, QByteArray()},
    {"Competitive Companion/Enable", "bool", "", {"competitive_use"}, true},
    {"Competitive Companion/Connection Port", "int", "", {"connection_port"}, 10045, QList<QVariant>{1024, 49151}},
    {"Competitive Companion/Open New Tab", "bool", "", {"companion_new_tab"}, true},
    {"Hotkey/Enable", "bool", "", {"hotkey_use"}, false},
    {"Hotkey/Format", "QString", "ShortcutItem", {"hotkey_format"}, ""},
    {"Hotkey/Kill", "QString", "ShortcutItem", {"hotkey_kill"}, ""},
    {"Hotkey/Compile Run", "QString", "ShortcutItem", {"hotkey_compile_run"}, ""},
    {"Hotkey/Run", "QString", "ShortcutItem", {"hotkey_run"}, ""},
    {"Hotkey/Compile", "QString", "ShortcutItem", {"hotkey_compile"}, ""},
    {"Hotkey/Change View Mode", "QString", "ShortcutItem", {"hotkey_mode_toggle"}, ""},
    {"Hotkey/Snippets", "QString", "ShortcutItem", {"hotkey_snippets"}, ""},
    {"Hot Exit/Enable", "bool", "", {"use_hot_exit"}, true},
    {"Hot Exit/Tab Count", "int", "", {"number_of_tabs"}, 0},
    {"Hot Exit/Current Index", "int", "", {"current_index"}, -1},
    {"Hot Exit/Load From File", "bool", "", {"hot_exit_load_from_file"}, false},
    {"CF/Path", "QString", "PathItem", {"cf_path"}, "cf", 0},
    {"Save Path", "QString", "", {}, ""},
    {"Show Compile And Run Only", "bool", "", {"compile_and_run_only"}, false},
    {"Display EOLN In Diff", "bool", "", {}, false},
    {"Save Faster", "bool", "", {}, false},
};

inline SettingInfo findSetting(const QString &desc)
{
    for (const SettingInfo &si : settingInfo)
        if (si.desc == desc)
            return si;
    return SettingInfo();
}

#endif // SETTINGSINFO_HPP