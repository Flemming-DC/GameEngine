#include "AttachToPlayer.h"
#include "Player.h"

void AttachToPlayer::OnStart()
{
	GetTransform().SetParent(&Player::transform());
}
