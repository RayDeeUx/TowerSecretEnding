#pragma once

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	bool calledAlready = false;

	const std::unordered_map<int, int> robtopToColon = {
		{5001, 116926133},
		{5002, 116926138},
		{5003, 116926140},
		{5004, 116926955},
	};

	static Manager* getSharedInstance() {
		if (!instance) {
			instance = new Manager();
		}
		return instance;
	}

};