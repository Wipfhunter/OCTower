/**
	Tower of Despair
	A big tower filled with riddle rooms.

	@author Sven2, Maikel
*/


public func Initialize()
{
	// Gather statistics.
	CreateObject(Statistics_RoomDuration);
	CreateObject(Statistics_RoomAttempts);
	CreateObject(Statistics_TowerRanking);
	return;
}

public func InitializePlayer(int plr)
{
	// Init the room data for this player from the saved data.
	InitPlayerRoomData(plr);
	// Join the player with its crew.
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
	crew->SetPosition(40 + RandomX(80), 350);
	crew->SetDir(DIR_Right);
	//crew->SetPosition(620, 294);
	
	// Add the player to the playing queue.
	AppendPlayerToQueue(plr);
	return;
}

public func RemovePlayer(int plr)
{
	if (GetPlayerCount(C4PT_User) <= 1)
		return GameOver();
	return;
}

public func OnGameOver()
{
	HideSettlementScoreInEvaluation(true);
	AddEvaluationData("$MsgEvaluationData$", 0);
}

public func InitMain()
{
	SetCurrentRoom(nil);
	for (var plr in GetPlayers())
		JoinPlayer(plr);
	return;
}

public func OnRoomEntranceEntered(object crew)
{
	// Only open room menu if entering the tower.
	if (GetCurrentRoom() == nil)
		RoomMenu->Create(crew->GetController());
	return;
}

public func OnRoomJokerCollected(object crew, object joker)
{
	CustomMessage("$MsgJokerCollected$", nil, NO_OWNER, 90, 70, 0xffffff, GUI_MenuDeco, {Source = joker->GetID()});
	return;
}

public func OnRoomJokerCompleted(object crew, id room)
{
	// Share the progress among all playing players.
	for (var for_plr in GetPlayers(C4PT_User))
	{
		// Add the joker of this room to the player's completion list.
		AddPlayerFoundJoker(for_plr, room);
		// Save the progress.
		SavePlayerRoomData(for_plr);
	}
	return;
}

public func OnRoomTabletCollected(object crew, object tablet)
{
	CustomMessage("$MsgTabletCollected$", nil, NO_OWNER, 90, 70, 0xffffff, GUI_MenuDeco, {Source = tablet->GetID()});
	return;
}

public func OnRoomTabletCompleted(object crew, id room)
{
	// Share the progress among all playing players.
	for (var for_plr in GetPlayers(C4PT_User))
	{
		// Add the tablet of this room to the player's completion list.
		AddPlayerFoundTablet(for_plr, room);
		// Save the progress.
		SavePlayerRoomData(for_plr);
	}
	return;
}

public func OnRoomCompleted(object crew, id room)
{
	var plr = crew->GetOwner();
	// Share the progress among all playing players.
	for (var for_plr in GetPlayers(C4PT_User))
	{
		// Add this room to the player's completion list.
		AddPlayerCompletedRoom(for_plr, room);
		// Remove a possible used joker for this room.
		RemovePlayerUsedJoker(for_plr, room);
		// Save the progress.
		SavePlayerRoomData(for_plr);
	}
	
	// Append player to the end of playing queue.	
	AppendPlayerToQueue(plr);
	
	// Get the next room and load that.
	var next_room = GetNextRoom(room);
	if (next_room)
		return next_room->LoadRoom();

	// Load main if no next room is found.		
	return LoadMain();
}
