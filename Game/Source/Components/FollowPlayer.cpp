#include "FollowPlayer.h"
#include "Player.h"


void FollowPlayer::OnUpdate()
{
	GetTransform().SetPosition2D(Player::transform().Position2D());
}


