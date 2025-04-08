// colon sent me this in private
// it takes too long to find the binding for DialogLayer::onClose() on macos
// --raydeeux
#pragma once

using namespace geode::prelude;

class CustomDialogCallback : public CCNode, public DialogDelegate {
public:
	std::function<void()> m_callback;
	virtual void dialogClosed(DialogLayer* p0) { if (m_callback) m_callback(); }
};