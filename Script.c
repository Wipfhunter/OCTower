/**
	Tower of Despair
	A big tower filled with riddle rooms.

	@author Sven2, Maikel
*/


public func Initialize()
{
	return;
}

public func InitializePlayer(int plr)
{
	InitPlayerRoomData(plr);
	JoinPlayer(plr);
	return;
}

public func JoinPlayer(int plr)
{
	// Get crew member or create new one.
	var crew = GetCrew(plr);
	if (!crew)
	{
		var crew = CreateObjectAbove(Clonk, 0, 0, plr);
		crew->MakeCrewMember(plr);
		SetCursor(plr, crew);
	}

	// Give clonk its maximum energy.
	crew->DoEnergy(crew.MaxEnergy / 1000);
	
	// Move clonk to position outside the castle.
	//crew->SetPosition(40 + RandomX(100), 350);
	crew->SetPosition(620, 294);
	return;
}

public func InitMain()
{
	for (var plr in GetPlayers())
		JoinPlayer(plr);
	return;
}

public func OnRoomEntranceEntered(object crew)
{
	RoomMenu->Create(crew->GetController());
	return;
}

public func OnRoomJokerCompleted(object crew, id room)
{
	var plr = crew->GetOwner();
	// Add the joker of this room to the player's completion list.
	AddPlayerFoundJoker(plr, room);
	// Save the progress.
	SavePlayerRoomData(plr);
	return;
}

public func OnRoomTabletCompleted(object crew, id room)
{
	var plr = crew->GetOwner();
	// Add the tablet of this room to the player's completion list.
	AddPlayerFoundTablet(plr, room);
	// Save the progress.
	SavePlayerRoomData(plr);
	return;
}

public func OnRoomCompleted(object crew, id room)
{
	var plr = crew->GetOwner();
	// Add this room to the player's completion list.
	AddPlayerCompletedRoom(plr, room);
	// Remove a possible used joker for this room.
	RemovePlayerUsedJoker(plr, room);
	// Save the progress.
	SavePlayerRoomData(plr);
	
	// Get the next room and load that.
	var next_room = GetNextRoom(room);
	if (next_room)
		return next_room->LoadRoom();

	// Load main if no next room is found.		
	return LoadMain();
}
