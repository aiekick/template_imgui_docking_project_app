/* SPDX-License-Identifier: LGPL-2.1-or-later */
#include "frontend.h"
#include <app.h>

#include <algorithm>
#include <cinttypes>
#include <filesystem>
#include <string>

#include <GLFW/glfw3.h>

#include <frontend/panes/console.h>

#define MESSAGING_CODE_INFOS 0
#define MESSAGING_LABEL_INFOS "Infos"
#define MESSAGING_CODE_WARNINGS 1
#define MESSAGING_LABEL_WARNINGS "Warnings"
#define MESSAGING_CODE_ERRORS 2
#define MESSAGING_CODE_DEBUG 3
#define MESSAGING_LABEL_ERRORS "Errors"
#define MESSAGING_LABEL_DEBUG "Debug"

Frontend::Frontend(IApp &aIApp) : mr_app(aIApp) {
}

bool Frontend::init() {
    ImGui::CustomStyle::ResetCustomId();
    m_buildThemes();
    ImLayout::initSingleton();
    ImLayout::ref().init("Panes", "Default Layout");
    Console::initSingleton();
    ImLayout::ref().addPane(LayoutPaneInfos(Console::ref(), "Console").setMenu("Console").setDisposalSide("BOTTOM", 0.5f));
    Messaging::initSingleton();
    Messaging::ref().AddCategory(MESSAGING_CODE_INFOS, "Infos(s)", MESSAGING_LABEL_INFOS, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    Messaging::ref().AddCategory(MESSAGING_CODE_WARNINGS, "Warnings(s)", MESSAGING_LABEL_WARNINGS, ImVec4(0.8f, 0.8f, 0.0f, 1.0f));
    Messaging::ref().AddCategory(MESSAGING_CODE_ERRORS, "Errors(s)", MESSAGING_LABEL_ERRORS, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    Messaging::ref().AddCategory(MESSAGING_CODE_DEBUG, "Debug(s)", MESSAGING_LABEL_DEBUG, ImVec4(0.8f, 0.8f, 0.0f, 1.0f));
    Messaging::ref().SetImLayout(&ImLayout::ref());
    ez::Log::ref().setStandardLogMessageFunctor([](const int &vType, const std::string &vMessage) {
        Messaging::ref().AddMessage(vMessage, vType, false, {}, {});
    });
    Messaging::sMessagePaneId = Console::ref()->getFlag();
    return ImLayout::ref().initPanes();
}

void Frontend::unit() {
    ImLayout::ref().unitPanes();
    Console::unitSingleton();
    ImLayout::ref().unit();
    ImLayout::unitSingleton();
}

void Frontend::display() {
    ImGui::CustomStyle::ResetCustomId();
    m_rect = ImRect(ImVec2(), ImGui::GetIO().DisplaySize);
    m_actionsSystem.runImmediateActions();
    m_actionsSystem.executeFirstConditionalAction();
    m_drawMainMenuBar();
    m_drawMainStatusBar();

    if (ImLayout::ref().beginDockSpace(ImGuiDockNodeFlags_PassthruCentralNode)) {
        ImLayout::ref().endDockSpace();
    }
    ImLayout::ref().drawPanes();
    ImGuiThemeHelper::ref().Draw();
    m_drawDialogs();
    ImLayout::ref().initAfterFirstDisplay(m_rect.Max);
}

void Frontend::actionCloseApp() {
    if (mr_app.isAppClosingNeeded()) {
        return;
    }
    m_actionsSystem.clear();
    m_actionsSystem.pushBackConditonalAction([this]() {
        mr_app.closeApp();
        return true;
    });
}

void Frontend::m_actionCancel() {
    m_actionsSystem.clear();
    mr_app.appClosingNeeded(false);
}

void Frontend::m_drawMainMenuBar() {
    if (!ImGui::BeginMainMenuBar()) {
        return;
    }
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
            GLFWwindow *win = glfwGetCurrentContext();
            if (win) {
                glfwSetWindowShouldClose(win, GLFW_TRUE);
            }
        }
        ImGui::EndMenu();
    }

    ImLayout::ref().drawMenu(m_rect.Max);
    if (ImGui::BeginMenu("Tools")) {
        if (ImGui::BeginMenu("Styles")) {
            ImGuiThemeHelper::ref().DrawMenu();
            ImGui::Separator();
            ImGui::MenuItem("ImGui Demo", nullptr, &m_showFlags.imgui);
            ImGui::MenuItem("ImNodal Demo", nullptr, &m_showFlags.imnodal);
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About")) {
            m_showFlags.aboutDialog = true;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

void Frontend::m_drawMainStatusBar() {
    if (ImGui::BeginMainStatusBar()) {
        Messaging::ref().DrawStatusBar();
        ImGui::EndMainStatusBar();
    }
}

void Frontend::m_drawDialogs() {
    ImLayout::ref().drawDialogsAndPopups(m_rect);
    if (m_showFlags.imgui) {
        ImGui::ShowDemoWindow(&m_showFlags.imgui);
    }
    if (m_showFlags.imnodal) {
        ImNodal::ShowDemoWindow(&m_showFlags.imnodal);
    }
    m_drawAboutDialog();
}

void Frontend::m_drawAboutDialog() {
    if (m_showFlags.aboutDialog) {
        ImGui::OpenPopup("About");
        m_showFlags.aboutDialog = false;
    }
    if (ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        constexpr ImGuiTableFlags k_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit;
        if (ImGui::BeginTable("about_tbl", 2, k_flags)) {
            ImGui::TableSetupColumn("Component", ImGuiTableColumnFlags_WidthFixed, 130.0f);
            ImGui::TableSetupColumn("Build", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Dear ImGui");
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(IMGUI_VERSION);
            ImGui::EndTable();
        }
        ImGui::Separator();
        if (ImGui::ContrastedButton_For_Dialogs("Close", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
}

ez::xml::Nodes Frontend::getXmlNodes(const std::string &vUserDatas) {
    ez::xml::Node root;
    auto &xml_panes = root.addChild("panes");
    xml_panes.addChilds(Console::ref()->getXmlNodes(vUserDatas));
    root.addChilds(ImGuiThemeHelper::ref().getXmlNodes(vUserDatas));
    root.addChilds(ImLayout::ref().getXmlNodes(vUserDatas));
    root.addChild("places").setContent(ImGuiFileDialog::ref().SerializePlaces());
    return root.getChildren();
}

bool Frontend::setFromXmlNodes(const ez::xml::Node &vNode,
                            const ez::xml::Node &vParent,
                            const std::string &vUserDatas) {
    ImLayout::ref().setFromXmlNodes(vNode, vParent, vUserDatas);
    ImGuiThemeHelper::ref().setFromXmlNodes(vNode, vParent, vUserDatas);
    Console::ref()->setFromXmlNodes(vNode, vParent, vUserDatas);
    if (vNode.getName() == "places") {
        ImGuiFileDialog::ref().DeserializePlaces(vNode.getContent());
    }
    return true;
}
