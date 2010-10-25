#pragma once

#include "BaseScene.h"

namespace NScene
{
	class CTriBandsScene : public CBaseScene
	{
	public:
		CTriBandsScene() {};

		virtual void Init();
		virtual void Update();
		virtual void Draw();

	private:
	};
};
