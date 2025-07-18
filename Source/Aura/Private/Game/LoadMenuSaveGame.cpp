// Copyright Eduard Ciofu


#include "Game/LoadMenuSaveGame.h"

FSavedMap ULoadMenuSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	const FSavedMap* FoundMap = SavedMaps.FindByPredicate([InMapName](const FSavedMap& Map)
		{
			return Map.MapAssetName == InMapName;
		}
	);

	return FoundMap ? *FoundMap : FSavedMap();
}

bool ULoadMenuSaveGame::HasMap(const FString& InMapName) const
{
	return SavedMaps.ContainsByPredicate([InMapName](const FSavedMap& Map)
		{
			return Map.MapAssetName == InMapName;
		}
	);
}
