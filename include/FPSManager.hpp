#pragma once

namespace Engine {
	class FPSManager
	{
	public:
		int FPS = 0;

		FPSManager();
		~FPSManager();

		void Calc();
		void Limit(double fps);
	private:
		double startTime = 0.f;

		double lastTime1 = glfwGetTime();
		double lastTime2 = glfwGetTime() + 0.25f;
		double lastTime3 = glfwGetTime() + 0.5f;
		double lastTime4 = glfwGetTime() + 0.75f;
		int frameCount1 = 0;
		int frameCount2 = 0;
		int frameCount3 = 0;
		int frameCount4 = 0;
	};

	void FPSManager::Calc() {
		double currentTime = glfwGetTime();
		this->frameCount1++;
		if (currentTime - this->lastTime1 >= 1.0) {
			this->FPS = frameCount1;
			this->frameCount1 = 0;
			this->lastTime1 = currentTime;
		}

		this->frameCount2++;
		if (currentTime - this->lastTime2 >= 1.0) {
			this->FPS = this->frameCount2;
			this->frameCount2 = 0;
			this->lastTime2 = currentTime;
		}

		this->frameCount3++;
		if (currentTime - this->lastTime3 >= 1.0) {
			this->FPS = this->frameCount3;
			this->frameCount3 = 0;
			this->lastTime3 = currentTime;
		}

		this->frameCount4++;
		if (currentTime - this->lastTime4 >= 1.0) {
			this->FPS = this->frameCount4;
			this->frameCount4 = 0;
			this->lastTime4 = currentTime;
		}

	}

	void FPSManager::Limit(double fps) {
		float endTime = glfwGetTime();
		while (glfwGetTime() < endTime + (1000 / fps - (endTime * 1000 - startTime * 1000)) / 1000)
		{

		}
		startTime = glfwGetTime();
	}

	FPSManager::FPSManager()
	{
	}

	FPSManager::~FPSManager()
	{
	}


}