#pragma once

#include "Module.h"

class Engine : public Singleton<Engine>
{
public:
	Engine();
	~Engine();

	void 
		Update();

	void 
		SetModule(Module * pModule);
	Module * 
		GetModule();

	void 
		MapScreenCoordUnit(float & x, float &y);
	float 
		GetHeightAt(const Float3 & pos);

protected:
	Module * mModule;
};