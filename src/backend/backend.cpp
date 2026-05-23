#include "backend.h"

Backend::Backend(IApp& aIApp) : mr_app(aIApp) {
}

bool Backend::init() {
#ifdef NDEBUG
    setConsoleVisibility(false);
#endif
    return true;
}

void Backend::unit() {

}

void Backend::startFrame() {

}

void Backend::endFrame() {

}

void Backend::justDropFiles(const std::vector<std::string> aPaths) {
    
}

ez::xml::Nodes Backend::getXmlNodes(const std::string& vUserDatas) {
    ez::xml::Node node;
    return node.getChildren();
}

bool Backend::setFromXmlNodes(const ez::xml::Node& vNode, const ez::xml::Node& vParent, const std::string& vUserDatas) {
    return true; // continue explore childrens
}
