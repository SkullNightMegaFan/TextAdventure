#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>

RoomData CreateRoom(const std::string& inName, const std::string& inDescription, bool hasKey, bool canSleepHere)
{
	RoomData room = {};
	room.Name = inName;
	room.Description = inDescription;
	room.HasKey = hasKey;
	room.CanSleepHere = canSleepHere;

	return room;
}

void AddExitToRoom(RoomData& roomToEdit, const std::string& exitName, int TargetRoomIndex, bool isLocked)
{
	RoomExitData exit = {};
	exit.Name = exitName;
	exit.TargetRoomIndex = TargetRoomIndex;
	exit.Locked = isLocked;
	roomToEdit.Exits.push_back(exit);
}
void InitializeGame(PlayerState& playerState, WorldState& worldState)
{
	playerState.CurrentRoomIndex = 0;
	playerState.WantsToLook = true;
	playerState.HasKey = false;

	printf("Game Instructions:\n\nThe goal of the game is to explore the party and find an exit.\n");
	printf("The command list is provided below to help you navigate through this game.\n\nType help for a more detailed explanation of each command.\n");
	printf("Command List: actions,help,look, quit, go [location], get key, sleep.\n");

	//0
	RoomData room1 = CreateRoom("After much thought, you decide to go to a party.\n\nGo out there and have some fun", "The front steps to a Frat house, it looks deciptively small ,but you hear a lot of people inside.\n\n", false, false);
	AddExitToRoom(room1, "livingroom", 2, false);
	AddExitToRoom(room1, "leave", 5, false);
	worldState.Rooms.push_back(room1);
	//1
	RoomData room2 = CreateRoom("backroom", "There's more alcohol than you could ever drink in one night, you spot your friend from Calculus chugging a Corona\n", true, false);
	AddExitToRoom(room2, "fratbedroom", 4, true);
	AddExitToRoom(room2, "livingroom", 2, false);
	worldState.Rooms.push_back(room2);
	//2
	RoomData room3 = CreateRoom("livingroom", "While at first the room seems inviting you quickly notice it's a slob fest. All the one night stand couples are here\nYou decide to leave before you have a chance to see more", false, false);
	AddExitToRoom(room3, "entrance", 0, false );
	AddExitToRoom(room3, "backroom", 1, false);
	AddExitToRoom(room3, "basement", 3, false);
	worldState.Rooms.push_back(room3);
	//3
	RoomData room4 = CreateRoom("basement", "As you walk down the creaky steps you quickly find the dance floor.\n\n\n Everyone seems to be having a good time dancing and you even recogonize one of your favorite songs playing.", false, false);
	AddExitToRoom(room4, "livingroom", 2, false);
	worldState.Rooms.push_back(room4);
	//4
	RoomData room5 = CreateRoom("fratbedroom", "Surprisingly tidy, then again Chris is very organized.\n\n\nChris' bed looks very inviting right now, time to go to dreamland.\n\n", false, true);
	AddExitToRoom(room2, "backroom", 1, false);
	worldState.Rooms.push_back(room5);
	//5
	RoomData room6 = CreateRoom("leave", "I did not think you would actually notice that option!\n\n\n You did clear the game ,but if you want the full experience feel free to replay the game.\n\n", false, false);
	worldState.Rooms.push_back(room6);
	
	
	
}

void GetInput(PlayerState& playerState, const WorldState& worldState)
{
	///og set to false 
	playerState.WantsToLook = true;
	playerState.WantsToSleep = false;
	playerState.DesiredExit = "";
	playerState.DesiredPickUp = "";

	printf("What do you do?\n");
	printf("> ");
	TextAdventureCommand command = ParseAdventureCommand();
	if (command.Verb == "quit")
	{
		playerState.WantsToExit = true;
	}
	else if (command.Verb == "go")
	{
		playerState.DesiredExit = command.Parameter;
	}
	else if (command.Verb == "get")
	{
		playerState.DesiredPickUp = command.Parameter;
	}
	else if (command.Verb == "sleep")
	{
		playerState.WantsToSleep = true;
	}
	else if (command.Verb == "help")
	{
		printf("Command List: look, quit, go [place], get key, sleep.\n");
		printf("look: this function will give you the room description alongside which rooms you can enter.\n");
		printf("\nget key:This action enables the player to add a key to their inventory if they are in the correct room.\n");
		printf("\ngo: This action enables the player to go to another room listed in the exits.\n");
		printf("\nquit: This function allows you to exit the game.\n");
		printf("\nsleep: A potential win condition for this game, explore the party to find clues.\n");
		printf("\nhelp: You've already used this action ,but it will allow you to see the instructions at any time.\n");
	}
	else if (command.Verb == "actions")
	{
		printf("Command List: look, quit, go [place], get key, sleep.\n");
	}
	else if (command.Verb == "look")
		playerState.WantsToLook = true;
	else
	{
		printf("I don't understand\n");
		printf("\nTry typing help to find valid commands.");
	}
	printf("\n");

}

void RenderGame(const PlayerState& playerState, const WorldState& worldState)
{
	if (playerState.WantsToLook)
	{
		RoomData currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
		printf("===========================\n");
		printf("Location %s\n", currRoom.Name.c_str());
		printf("%s\n\n", currRoom.Description.c_str());

		if (currRoom.HasKey)
		{
			printf("I'm pretty sure my friend lives at this frat house.\nMaybe he has a key");
		}
		printf("\nEXITS:\n");

		

		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i)
		{
			printf("%s\n", currRoom.Exits[i].Name.c_str());
		}

		printf("\n");

		if (playerState.HasKey)
		{
			printf("Inventory:\n");
			printf("Bedroom Key\n");
		}
		printf("\n");
	}
	


}

void UpdateGame(PlayerState& playerState, WorldState& worldState)
{
	

	RoomData& currRoom = worldState.Rooms[playerState.CurrentRoomIndex];
	//////////////////////////////////
	
	if (playerState.DesiredExit != "")
	{
		bool foundExit = false;

		for (unsigned int i = 0; i < currRoom.Exits.size(); ++i)
		{
			if (playerState.DesiredExit == currRoom.Exits[i].Name)
			{
				foundExit = true;

				if (!currRoom.Exits[i].Locked)
				{
					playerState.CurrentRoomIndex = currRoom.Exits[i].TargetRoomIndex;
					playerState.WantsToLook = true;
				}
				else
				{
					if (playerState.HasKey)
					{
						playerState.CurrentRoomIndex = currRoom.Exits[i].TargetRoomIndex;
						playerState.WantsToLook = true;
					}
					else
					{
						printf("Looks like this door is locked.\n Maybe you know someone that has the key.\n");
					}
				}

				{
					if (playerState.HasKey)
					{
						playerState.CurrentRoomIndex = currRoom.Exits[i].TargetRoomIndex;
						playerState.WantsToLook = true;
					}


				}


			}

		}
		if (!foundExit)
		{
			printf("Hmmmm I can't find an exit called %s\n.", playerState.DesiredExit.c_str());
		}

	}
	else if (playerState.DesiredPickUp != "")
	{
		if (playerState.DesiredPickUp == "key")
		{
			if (currRoom.HasKey)
			{
				printf("Hey Chris, this party is getting kind of loud, mind if I sleep in your room for the night?\n");
				printf("Chris: No problem man, my room is a right over there.\n\n");
				playerState.HasKey = true;
				currRoom.HasKey = false;

			}
			else
			{
				printf("Doesn't look like anybody here has the key to the bedroom,\n\nKeep looking");

			}
		}
		else
		{
			printf("I'm feeling a bit tired, I should look for a KEY");
		}
	}
	else if (playerState.WantsToSleep)
	{
		if (currRoom.CanSleepHere)
		{
			printf("It's surprisingly comfortable, you feel yourself slowly drift to slee.\n");
			printf("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ\n");
			playerState.WantsToExit = true;
		}
		else
		{
			printf("There's a time and a place for everything, and it's definitely not time to sleep on the floor.\n\nMaybe one of your friends will allow you to stay the night\n");
		}
	}
	if (playerState.CurrentRoomIndex = 6)
	{
		printf("\nI did not think you would actually notice that option!\n\nYou did clear the game,but if you want the full experience feel free to replay the game.\n\n");
		playerState.WantsToExit = true;
	}
}

void CleanupGame(PlayerState& playerState, WorldState& worldState)
{
	printf("Quitting...\n\n");
}