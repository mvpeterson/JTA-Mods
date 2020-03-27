#include "Pedestrian.h"
#include "dictionary.h"

Pedestrian::Pedestrian(Hash _modelHash)
{
	setModelHash(_modelHash);
	this->state_current = NOT_EXISTS;
}

Pedestrian::~Pedestrian() {}

void Pedestrian::setModelHash(Hash _modelHash)
{
	this->modelHash = _modelHash;
}
void Pedestrian::assignJointCodes()
{
	joint_int_codes[0] = m.find("SKEL_Head")->second;
	joint_int_codes[1] = m.find("SKEL_Neck_1")->second;
	joint_int_codes[2] = m.find("SKEL_R_Clavicle")->second;
	joint_int_codes[3] = m.find("SKEL_R_UpperArm")->second;
	joint_int_codes[4] = m.find("SKEL_R_Forearm")->second;
	joint_int_codes[5] = m.find("SKEL_R_Hand")->second;
	joint_int_codes[6] = m.find("SKEL_L_Clavicle")->second;
	joint_int_codes[7] = m.find("SKEL_L_UpperArm")->second;
	joint_int_codes[8] = m.find("SKEL_L_Forearm")->second;
	joint_int_codes[9] = m.find("SKEL_L_Hand")->second;
	joint_int_codes[10] = m.find("SKEL_Spine3")->second;
	joint_int_codes[11] = m.find("SKEL_Spine2")->second;
	joint_int_codes[12] = m.find("SKEL_Spine1")->second;
	joint_int_codes[13] = m.find("SKEL_Spine0")->second;
	joint_int_codes[14] = m.find("SKEL_Spine_Root")->second;
	joint_int_codes[15] = m.find("SKEL_R_Thigh")->second;
	joint_int_codes[16] = m.find("SKEL_R_Calf")->second;
	joint_int_codes[17] = m.find("SKEL_R_Foot")->second;
	joint_int_codes[18] = m.find("SKEL_L_Thigh")->second;
	joint_int_codes[19] = m.find("SKEL_L_Calf")->second;
	joint_int_codes[20] = m.find("SKEL_L_Foot")->second;
}

void Pedestrian::spawn(Vector3 pos)
{
	if (this->state_current == NOT_EXISTS)
	{
		STREAMING::REQUEST_MODEL(modelHash);
		while (!STREAMING::HAS_MODEL_LOADED(modelHash))
			WAIT(0);

		ped_id = PED::CREATE_PED(NULL, modelHash, pos.x, pos.y, pos.z, 0.0, false, false);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelHash);

		WAIT(50);

		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped_id, TRUE);

		WAIT(100);

		spawn_point = pos;

		this->state_current = SPAWNED;
	}
}

Vector3 Pedestrian::getCurrentCoord()
{
	return ENTITY::GET_ENTITY_COORDS(ped_id, true);
}

void Pedestrian::taskWalkToTarget(Vector3 target_point, float speed)
{
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_GO_TO_COORD_ANY_MEANS(0, target_point.x, target_point.y, target_point.z, speed, 0, 0, 786603, 0xbf800000);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(ped_id, seq);
	AI::CLEAR_SEQUENCE_TASK(&seq);

	this->state_current = TASK_WALK_TO_TARGET;
}

void Pedestrian::taskFollowToOffsetOfEntity(Ped ped_target, float offset, float speed)
{
	AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(this->ped_id, ped_target, offset, offset, offset, speed + 0.2, -1, 10, 1);
	this->state_current = TASK_FOLLOW;
}

void Pedestrian::taskUseMobilPhoneTimed(int time_ms)
{
	AI::TASK_USE_MOBILE_PHONE_TIMED(this->ped_id, time_ms);
	task_start_time = std::clock() / CLOCKS_PER_SEC;
	this->state_current = TASK_PHONE;
}

void Pedestrian::taskChatToPed(Ped target_ped)
{
	AI::TASK_CHAT_TO_PED(this->ped_id, target_ped, 16, 0.0, 0.0, 0.0, 0.0, 0.0);
	task_start_time = std::clock() / CLOCKS_PER_SEC;
	this->state_current = TASK_CHAT;
}

void Pedestrian::taskWanderInArea(float x, float y, float z, float radius, float minimalLength, float timeBetweenWalks)
{
	AI::TASK_WANDER_IN_AREA(this->ped_id, x, y, z, radius, minimalLength, timeBetweenWalks);
	task_start_time = std::clock() / CLOCKS_PER_SEC;
	this->state_current = TASK_WANDER_IN_AREA;
}

void Pedestrian::taskAimAtEntity(Entity entity, int duration, BOOL p3)
{
	AI::TASK_AIM_GUN_AT_ENTITY(ped_id, entity, duration, p3);
	task_start_time = std::clock() / CLOCKS_PER_SEC;
	this->state_current = TASK_AIM_AT_ENTITY;
}

void Pedestrian::taskGoToCoordWhileAimingAtCoord(float x, float y, float z, float aimAtX, float aimAtY, float aimAtZ, float moveSpeed)
{
	AI::TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD(ped_id, x, y, z, aimAtX, aimAtY, aimAtZ, moveSpeed,
		false, 2.0, 0.5, true, 0, 0, 0xC6EE6B4C);
	task_start_time = std::clock() / CLOCKS_PER_SEC;
	this->state_current = TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD;
}