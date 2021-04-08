#include <iostream>
#include "InputManager.h"

namespace snake {

	/* PUBLIC */
	InputManager& InputManager::getInstance() {
		static InputManager instance;
		return instance;
	}

	void InputManager::update() {

		m_oldBtnStates = m_btnStates;
		if (m_oldKeyStates.get() != nullptr && m_keyStates != nullptr)
			memcpy(m_oldKeyStates.get(), m_keyStates, m_numOKeys * sizeof(Uint8));

		SDL_PumpEvents();
		m_btnStates = SDL_GetMouseState(&m_mouseX, &m_mouseY);
	}

	bool InputManager::keyDown(int keyIndex) const {

		return currentKeyDown(keyIndex) && !oldKeyDown(keyIndex);
	}

	bool InputManager::keyStillDown(int keyIndex) const {

		return currentKeyDown(keyIndex) && oldKeyDown(keyIndex);
	}

	bool InputManager::keyUp(int keyIndex) const {

		return !currentKeyDown(keyIndex) && oldKeyDown(keyIndex);
	}

	bool InputManager::keyStillUp(int keyIndex) const {

		return !currentKeyDown(keyIndex) && !oldKeyDown(keyIndex);
	}

	bool InputManager::mouseDown(int btnIndex) const {

		return currentBtnDown(btnIndex) && !oldBtnDown(btnIndex);
	}

	bool InputManager::mouseStillDown(int btnIndex) const {

		return currentBtnDown(btnIndex) && oldBtnDown(btnIndex);;
	}

	bool InputManager::mouseUp(int btnIndex) const {

		return !currentBtnDown(btnIndex) && oldBtnDown(btnIndex);
	}

	bool InputManager::mouseStillUp(int btnIndex) const {

		return !currentBtnDown(btnIndex) && !oldBtnDown(btnIndex);
	}

	int InputManager::getMouseX() {
		return m_mouseX;
	}

	int InputManager::getMouseY() {
		return m_mouseY;
	}

	/* PRIVATE */
	InputManager::InputManager() :
		m_keyStates(SDL_GetKeyboardState(&m_numOKeys)),
		m_btnStates(SDL_GetMouseState(&m_mouseX, &m_mouseY)) {

		m_oldKeyStates = std::make_unique<Uint8[]>(m_numOKeys);

		m_oldBtnStates = m_btnStates;
	}

	bool InputManager::currentKeyDown(int keyIndex) const {
		return m_keyStates[keyIndex];
	}

	bool InputManager::oldKeyDown(int keyIndex) const {
		return m_oldKeyStates.get()[keyIndex];
	}

	bool InputManager::currentBtnDown(int button) const {

		//AND operation with the bitmask btnStates & the number representing the btn
		if (m_btnStates & SDL_BUTTON(button)) {
			return true;
		} else {
			return false;
		}
	}

	bool InputManager::oldBtnDown(int button) const {
		if (m_oldBtnStates & SDL_BUTTON(button)) {
			return true;
		} else {
			return false;
		}
	}
}