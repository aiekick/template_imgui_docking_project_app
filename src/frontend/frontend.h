/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once
#include <string>
#include <vector>
#include <imguipack.h>
#include <interfaces/iapp.h>
#include <ezlibs/ezClass.hpp>
#include <ezlibs/ezSingleton.hpp>
#include <ezlibs/ezXmlConfig.hpp>
#include <ezlibs/ezActions.hpp>

class Frontend : public ez::xml::Config {
    DISABLE_DESTRUCTORS(Frontend)
    IMPLEMENT_SINGLETON(Frontend)

private:
    ImRect m_rect{};
    struct ImGuiDemoFlags {
        bool imgui{false};
        bool imnodal{false};
        bool aboutDialog{false};
    } m_showFlags;
    ez::Actions m_actionsSystem;
    IApp& mr_app;

public:
    Frontend() = default;
    Frontend(IApp& aIApp);
    bool init();
    void unit();
    void display();
    void actionCloseApp();

    ez::xml::Nodes getXmlNodes(const std::string& vUserDatas) override;
    bool setFromXmlNodes(const ez::xml::Node& vNode, const ez::xml::Node& vParent, const std::string& vUserDatas) override;

private:
    bool m_buildThemes();
    void m_drawMainMenuBar();
    void m_drawMainStatusBar();
    void m_drawDialogs();
    void m_drawAboutDialog();
    void m_actionCancel();
};
