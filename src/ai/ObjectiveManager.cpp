#include "ObjectiveManager.hpp"

ObjectiveManager::ObjectiveManager()
{
	this->currentObjective = OBJECTIVE_ENTER_MUSEUM;
}
ObjectiveManager::~ObjectiveManager()
{
	
}
void ObjectiveManager::tick()
{
	switch(this->currentObjective)
	{
		case OBJECTIVE_ENTER_MUSEUM: // TODO: Delay
			return;
		case OBJECTIVE_LOCATE_EXHIBIT: // Locate the exhibit
			return;
		case OBJECTIVE_CLEAR_EXHIBIT: // Sweep around the nearby area
			return;
		case OBJECTIVE_COLLECT_EXHIBIT: // Sit near exhibit and collect, delay
			return;
		case OBJECTIVE_LEAVE_MUSEUM: // Leave the museum
			return;
		case OBJECTIVE_ATTACK_TURRET: // Attack turret
			return;
		case OBJECTIVE_ATTACK_PLAYER: // Attack player
			return;
	}
}
