#pragma once


namespace SonyDriverHelper {

	class api {
	public:
		static void Init();

		static void MouseMove(float x, float y);

		static bool GetKey(int id);
	};
} inline SonyDriverHelper::api* _driver;