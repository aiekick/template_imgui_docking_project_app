#pragma once
#include <interfaces/iapp.h>
#include <ezlibs/ezClass.hpp>
#include <ezlibs/ezSingleton.hpp>
#include <ezlibs/ezXmlConfig.hpp>

class Backend : public ez::xml::Config {
    DISABLE_DESTRUCTORS(Backend)
    IMPLEMENT_SINGLETON(Backend)

private:
    IApp& mr_app;

public:
    Backend(IApp& aIApp);
    bool init();
    void unit();
    void startFrame();
    void endFrame();
    void justDropFiles(const std::vector<std::string> aPaths);
    ez::xml::Nodes getXmlNodes(const std::string& vUserDatas) override;
    bool setFromXmlNodes(const ez::xml::Node& vNode, const ez::xml::Node& vParent, const std::string& vUserDatas) override;
};
