#pragma once

#include <frontend/panes/abstract/apane.h>

class Console : public APane {
    DISABLE_CONSTRUCTORS(Console)
    DISABLE_DESTRUCTORS(Console)
    IMPLEMENT_SHARED_SINGLETON(Console)
public:
    bool init() override;
    void unit() override;
    bool drawPanes(bool* apOpened, LayoutPaneUserDatas apUserDatas) override;
    ez::xml::Nodes getXmlNodes(const std::string& vUserDatas) override;
    bool setFromXmlNodes(const ez::xml::Node& vNode, const ez::xml::Node& vParent, const std::string& vUserDatas) override;
};
