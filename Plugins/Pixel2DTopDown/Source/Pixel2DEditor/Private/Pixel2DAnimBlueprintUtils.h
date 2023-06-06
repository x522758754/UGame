//Polyart Studio 2021


#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphSchema_K2.h"
#include "Kismet2/BlueprintEditorUtils.h"

class PIXEL2DEDITORTD_API FPixel2DAnimBlueprintUtils
{
public:

	/**
	* Removes the supplied graph from the Blueprint.
	*
	* @param Blueprint			The blueprint containing the graph
	* @param GraphToRemove		The graph to remove.
	* @param Flags				Options to control the removal process
	*/
	static void RemoveGraph(UBlueprint* Blueprint, class UEdGraph* GraphToRemove, EGraphRemoveFlags::Type Flags = EGraphRemoveFlags::Default);

};