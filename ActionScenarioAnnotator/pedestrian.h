#pragma once
#include <string>
#include <vector>
#include <map>
#include "natives.h"
#include <ctime>

#define NJOINTS 21

enum State
{
	NOT_EXISTS									=	0,
	SPAWNED										=	1,
	CHATTING									=	2,
	WANDERING_AROUND							=	3,
	TASK_WALK_TO_TARGET							=	4,
	TASK_FINISHED								=	5,
	TASK_FOLLOW									=	6,
	TASK_PHONE									=	7,
	TASK_CHAT									=	8,
	TASK_WANDER_IN_AREA							=	9,
	TASK_AIM_AT_ENTITY							=	10,
	TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD		=	11,
	STOPPED										=	12

};

class Pedestrian
{
public:
	Pedestrian(Hash _modelHash = 0x909D9E7F); //0x909D9E7F - child bankman
	~Pedestrian();
	void setModelHash(Hash _modelHash);
	void spawn(Vector3 pos);
	std::map< std::string, int > states; 
	
	Hash modelHash;
	Ped ped_id;

	int joint_int_codes[NJOINTS];

	Vector3 spawn_point;

	State state_current;

	clock_t task_start_time;

	Vector3 getCurrentCoord();

	void taskWalkToTarget(Vector3 target_point, float speed = 1.0);
	void taskFollowToOffsetOfEntity(Ped ped_target, float offset=0.5, float speed=1.2);
	void taskUseMobilPhoneTimed(int time_ms);
	void taskChatToPed(Ped target_ped);
	void taskWanderInArea(float x, float y, float z, float radius, float minimalLength, float timeBetweenWalks);
	void taskAimAtEntity(Entity entity, int duration=-1, BOOL p3=true);
	void taskGoToCoordWhileAimingAtCoord(float x, float y, float z, float aimAtX, float aimAtY, float aimAtZ, float moveSpeed);

private:
	void assignJointCodes();
};

