#include "mainscene.hpp"
#include "game.hpp"

#include "utils/logger.hpp"


MainScene::MainScene(Game* _game)
	: Scene{ _game },
	m_attack{ new Icosaedro{ } },
	m_attackp{ new Multiicos{ } },
	m_attackf{ new Fireball{ } },
	m_player{ new Mage{ } },
	m_plane{ new Plane{ } },
	m_enemy{ singleton_memory::getAssynchronousPool( ) },
	m_lastTime{ 0.f },
	m_attackTime{ 0.f },
	mBlinkOpen{ true },
	mBlinkTimer{ 0.0f },
	mAttackpOpen{ true },
	mAttackpTimer{ 0.0f },
	mAttackfOpen{ true },
	mAttackfTimer{ 0.0f },
	m_surviveTime{ 60.f }
{
	srand(time(nullptr));
	actors[0] = m_plane;
	actors[3] = m_player;
	actors[4] = m_attack;
	actors[5] = m_attackp;
	actors[6] = m_attackf;
	m_attackp->inactivate( );
	m_attackf->inactivate( );

	m_light = {
		{ 1.f, 1.f, 1.f },
		{ .6f, .7f, .9f }
	};

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		Logger::error("failed to open audio");
	}

	this->backgroundMusic = Mix_LoadMUS("assets/MainTheme.mp3");
	

	if (!backgroundMusic)
	{
		Logger::error("failed to open music file");
	}

	Mix_PlayMusic(backgroundMusic, -1);
	SDL_Delay(20);
	Mix_VolumeMusic(30);
}


MainScene::~MainScene( )
{
	if(backgroundMusic)
		Mix_FreeMusic(backgroundMusic);
}


void MainScene::onKeyboardEvent(SDL_KeyboardEvent& _e)
{
	vec4 front{ 0.f, 0.f, 1.f, 0.f };
	keyboard_event_trigger::when(_e, keyboard_event_trigger::KeyDown, SDLK_w).then([&] {
		m_player->getComponent<RigidBodyComponent>( )->acceleration = front * 3.f;
		});
	keyboard_event_trigger::when(_e, keyboard_event_trigger::KeyDown, SDLK_a).then([&] {
		m_player->relativeRot = glm::rotate(
			m_player->relativeRot,
			glm::radians(10.f),
			vec3{ 0.f, 1.f, 0.f });
		});
	keyboard_event_trigger::when(_e, keyboard_event_trigger::KeyDown, SDLK_s).then([&] {
		m_player->getComponent<RigidBodyComponent>( )->acceleration = front * -3.f;
		});
	keyboard_event_trigger::when(_e, keyboard_event_trigger::KeyDown, SDLK_d).then([&] {
		m_player->relativeRot = glm::rotate(
			m_player->relativeRot,
			glm::radians(-10.f),
			vec3{ 0.f, 1.f, 0.f });
		});
	keyboard_event_trigger::when(_e, keyboard_event_trigger::KeyDown, SDLK_SPACE).then([&] {
		if (mAttackpOpen)
		{
			mAttackpOpen = false;
			m_attackTime = 4.f;
			m_attackp->activate( );
		}
		});

	keyboard_event_trigger::when(_e, keyboard_event_trigger::KeyDown, SDLK_e).then([&] {
		if (mBlinkOpen)
		{
			mat4 rot = glm::toMat4(m_player->relativeRot);
			//pos = rot * vec4{ pos, 0.f };
			vec3 dash = rot * vec4{ 0.f, 0.f, 2.f, 0.f };
			m_player->rootComponent( )->position += dash;
			mBlinkOpen = false;
		}
		});
	keyboard_event_trigger::when(_e, keyboard_event_trigger::KeyDown, SDLK_q).then([&] {
		if (mAttackfOpen)
		{
			mAttackfOpen = false;
			mAttackfTimer = 4.f;
			m_attackf->activate( );
			m_attackf->rootComponent( )->position = m_player->rootComponent( )->position;
			atackFDir = m_player->rootComponent( )->rotation;
		}
		});

}

void MainScene::tickScene(float _deltaTime)
{
	m_lastTime += _deltaTime;
	vec3 playerPos = m_player->rootComponent( )->position;
	m_attack->rootComponent( )->position.x = .8f * glm::sin(m_lastTime * 6.f) + playerPos.x;
	m_attack->rootComponent( )->position.y = 0.1f + glm::sin(m_lastTime * 10.f) / 4.f;
	m_attack->rootComponent( )->position.z = .8f * glm::cos(m_lastTime * 6.f) + playerPos.z;

	if (m_attackTime > 0.f)
	{
		m_attackp->rootComponent( )->position.x = (.8f * glm::sin(m_lastTime * 5.f))
			* (m_attackTime - 4.f) + playerPos.x;
		m_attackp->rootComponent( )->position.y = 0.2f;
		m_attackp->rootComponent( )->position.z = (.8f * glm::cos(m_lastTime * 5.f))
			* (m_attackTime - 4.f) + playerPos.z;
		m_attackp->rootComponent( )->rotation = glm::rotate(quat{ vec3{ } },
			glm::radians(1800 * m_attackTime),
			glm::normalize(vec3{ 0.f, 1.f, 1.f }));
		m_attackTime = glm::clamp(m_attackTime - _deltaTime, 0.f, 4.f);
	} else if (m_attackTime == 0.f)
	{
		m_attackp->inactivate( );
	}

	if (mAttackfTimer > 0.f)
	{
		mat4 rot = glm::toMat4(atackFDir);
		vec3 dash = rot * vec4{ 0.f, 0.f, (1.f - mAttackfTimer / 4.f) * 0.5f, 0.f };
		m_attackf->rootComponent( )->position += dash;
		mAttackfTimer = glm::clamp(mAttackfTimer - _deltaTime, 0.f, 4.f);
	} else if (mAttackfTimer == 0.f)
	{
		m_attackf->inactivate( );
		mAttackfOpen = true;
	}

	Renderer::getCurrentShader( )->setFloatVec3("lightColor", m_light.color);
	while (m_enemy.size( ) <= 30) //Pelo menos 10 inimigos no mapa
	{
		auto enemy = object_ptr{ new Arac{ } };
		enemy->rootComponent( )->position.x = m_player->rootComponent( )->position.x
			+ 20.f * (1.f + (-1 + 2 * ((float) rand( ) / RAND_MAX)));
		enemy->rootComponent( )->position.z = m_player->rootComponent( )->position.z
			+ 20.f * (1.f + (-1 + 2 * ((float) rand( ) / RAND_MAX)));
		actors[10 + m_enemy.size( )] = enemy;
		m_enemy.push_back(enemy);
	}

	if (!mBlinkOpen)
	{
		mBlinkTimer += _deltaTime;
		if (mBlinkTimer > 5.0f)
		{
			mBlinkOpen = true;
			mBlinkTimer = 0.0f;
		}
	}

	if (!mAttackpOpen)
	{
		mAttackpTimer += _deltaTime;
		if (mAttackpTimer > 10.0f)
		{
			mAttackpOpen = true;
			mAttackpTimer = 0.0f;
		}
	}

	for (auto& i : m_enemy)
	{
		i->toMove = m_player->rootComponent( )->position;
		auto a = m_player->getComponent<AABBComponent>( ),
			b = i->getComponent<AABBComponent>( ),
			c = m_attack->getComponent<AABBComponent>( ),
			d = m_attackp->getComponent<AABBComponent>( ),
			e = m_attackf->getComponent<AABBComponent>( );

		if (c->intersect(*b) || d->intersect(*b) || e->intersect(*b))
		{
			if (e->intersect(*b))
			{
				m_attackf->inactivate( );
				mAttackfOpen = true;
			}

			if (i->GetHealth( ) <= 0.0f)
			{
				i->rootComponent( )->position.x = m_player->rootComponent( )->position.x
					+ 20.f * (1.f + (-1 + 2 * ((float) rand( ) / RAND_MAX)));
				i->rootComponent( )->position.z = m_player->rootComponent( )->position.x
					+ 20.f * (1.f + (-1 + 2 * ((float) rand( ) / RAND_MAX)));
			} else
			{
				vec3 dash = i->rootComponent( )->position - m_player->rootComponent( )->position;
				dash = glm::normalize(dash) * 0.5f;
				i->rootComponent( )->position.x += dash.x;
				i->rootComponent( )->position.z += dash.z;
				i->setHealth(i->GetHealth( ) - 30.f);
			}
		}

		if (a->intersect(*b))
		{
			Logger::log(m_player->GetHealth( ));
			if (m_player->GetHealth( ) <= 0.0f)
			{
				game->quit( );
			} else
			{
				vec3 dash = m_player->rootComponent( )->position - i->rootComponent( )->position;
				dash = glm::normalize(dash) * 0.8f;
				m_player->getComponent<RigidBodyComponent>( )->velocity = vec3{ };
				m_player->rootComponent( )->position.x += dash.x;
				m_player->rootComponent( )->position.z += dash.z;
				m_player->setHealth(m_player->GetHealth( ) - 15);
			}
		};
	}
}
