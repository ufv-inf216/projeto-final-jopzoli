/*
*
*/

#pragma once

#include "../scene.hpp"
#include "actor/myactors/arac.hpp"
#include "actor/myactors/mage.hpp"
#include "actor/myactors/icosaedro.hpp"
#include "actor/myactors/multiicos.hpp"
#include "actor/myactors/plane.hpp"
#include "actor/myactors/fireball.hpp"

#include "SDL_mixer.h"

class MainScene
	: public Scene
{
public:
	OBJIMPL( );

	MainScene(Game* _game);

	~MainScene( );

	void onKeyboardEvent(SDL_KeyboardEvent& _e) override;

	void tickScene(float _deltaTime) override;

private:
	object_ptr<Mage> m_player;
	object_ptr<Icosaedro> m_attack;
	object_ptr<Fireball> m_attackf;
	object_ptr<Multiicos> m_attackp;
	std::pmr::vector<object_ptr<Arac>> m_enemy;
	object_ptr<Plane> m_plane;
	lightSource m_light;
	float m_lastTime;
	float m_attackTime;
	float m_surviveTime;
	
	bool mBlinkOpen;
	float mBlinkTimer;

	float mAttackpTimer;
	bool mAttackpOpen;

	float mAttackfTimer;
	bool mAttackfOpen;
	quat atackFDir;

	Mix_Music* backgroundMusic = nullptr;
};
OBJDECL(MainScene, Scene);
