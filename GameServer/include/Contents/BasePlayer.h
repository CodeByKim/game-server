#pragma once
#include <NetworkLib.h>
#include "./Contents/Entity.h"

class Sector;

class BasePlayer : public Entity
{
public :
	BasePlayer();
	~BasePlayer();

	bool IsMove();
	garam::net::ClientInfo* GetClientInfo();
	virtual void OnSectorChanged(std::vector<Sector*>& leave, std::vector<Sector*>& enter);

protected:	
	garam::net::ClientInfo* mClient;

private:
	//void ProcessLeaveSector(std::vector<Sector*>& leaveSectors);
	//void ProcessNewEnterSector(std::vector<Sector*>& enterSectors);	

	bool mIsMoving;
};

