/*
 * This file is part of ezSqlite.
 *
 * Copyright (C) 2025 Stephane Cuillerdier (aka aiekick)
 *
 * ezSqlite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ezSqlite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with ezSqlite.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include <interfaces/iapp.h>
#include <ezlibs/ezApp.hpp>
#include <ezlibs/ezClass.hpp>
#include <ezlibs/ezXmlConfig.hpp>
#include <ezlibs/ezSingleton.hpp>

struct GLFWwindow;
class App : public ez::xml::Config, public IApp {
    DISABLE_CONSTRUCTORS(App)
    DISABLE_DESTRUCTORS(App)
public:
#ifdef __EMSCRIPTEN__
    static void render(void* vApp);
#endif

private:
    std::unique_ptr<ez::App> mp_app;
    GLFWwindow* mp_mainWindow{nullptr};
    const char* m_glslVersion{""};
    bool m_consoleVisiblity{false};
    bool m_appClosingNeeded{false};

public:
    bool init(int argc, char** argv);
    void run();
    void unit();
    void closeApp() override;
    bool isAppClosingNeeded() override;
    void appClosingNeeded(const bool& aFlag) override;
    void setConsoleVisibility(const bool& vFlag) override;
    void switchConsoleVisibility() override;
    bool getConsoleVisibility() override;
    void setAppTitle(const std::string& aTitle) override;
    ez::xml::Nodes getXmlNodes(const std::string& vUserDatas) override;
    bool setFromXmlNodes(const ez::xml::Node& vNode, const ez::xml::Node& vParent, const std::string& vUserDatas) override;

private:
    void m_render();
    bool m_initGlfw();
    void m_unitGlfw();
    bool m_initImgui();
    void m_unitImgui();
};
