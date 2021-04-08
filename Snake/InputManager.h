#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <memory>
#include <vector>

namespace snake {

	class InputManager {
	public:
		static InputManager& getInstance();
		void update();

		//Keyboard
		bool keyDown(int keyIndex) const;
		bool keyStillDown(int keyIndex) const;
		bool keyUp(int keyIndex) const;
		bool keyStillUp(int keyIndex) const;

		//Mouse
		bool mouseDown(int btnIndex) const;
		bool mouseStillDown(int btnIndex) const;
		bool mouseUp(int btnIndex) const;
		bool mouseStillUp(int btnIndex) const;

		int getMouseX();
		int getMouseY();

		//Deleting functions that could interfere with Singleton Pattern
		InputManager(InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		void operator=(const InputManager&) = delete;

	private:
		//Keyboard
		int m_numOKeys;
		const Uint8* m_keyStates = nullptr;
		std::unique_ptr<Uint8[]> m_oldKeyStates = nullptr;

		//Mouse
		int m_mouseX;
		int m_mouseY;
		Uint8 m_btnStates = 0;
		Uint8 m_oldBtnStates = 0;

		InputManager(); //Constructor only accessible from GetInstance()
		bool currentKeyDown(int keyIndex) const;
		bool oldKeyDown(int keyIndex) const;
		bool currentBtnDown(int button) const;
		bool oldBtnDown(int button) const;
	};
}

#endif