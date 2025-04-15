#include "stdafx.h"
#include "ToggleStacker.h"
#include <Spore/App/cPropManager.h>
#include <Spore\Properties.h>
#include <unordered_map>

using namespace UTFWin;

enum class Mode {
	DEFAULT,
	STACK_ALL,
	STACK_NONE,
	STACK_NO_BASE,
	CUSTOM,
};

Mode currentMode = Mode::DEFAULT;

ToggleStacker::ToggleStacker()
{
	App::AddUpdateFunction(this);
}

ToggleStacker::~ToggleStacker()
{
}

void ToggleStacker::Update()
{
	if (!Editor.IsMode(Editors::Mode::BuildMode) || Editor.GetEditorModel() == nullptr) return;

	for each (EditorRigblockPtr part in Editor.GetEditorModel()->mRigblocks)
	{
		part->mModelTypesNotToInteractWith.clear();
		part->mModelTypesToInteractWith.clear();

		switch (currentMode)
		{
			case Mode::DEFAULT:
				{
					PropertyListPtr propList = part->mpPropList;
					vector<ResourceKey> notInteractNames;
					App::Property::GetArrayKey(propList.get(), 0x044DCD0A, notInteractNames);
					for each (ResourceKey name in notInteractNames) {
						part->mModelTypesNotToInteractWith.push_back(name);
					}
					vector<ResourceKey> interactNames;
					App::Property::GetArrayKey(propList.get(), 0x6FFDED92, interactNames);
					for each (ResourceKey name in interactNames) {
						part->mModelTypesToInteractWith.push_back(name);
					}
					break;
				}
			case Mode::STACK_ALL:
				break;
			case Mode::STACK_NONE:
				{
					static const vector<ResourceKey> notInteractKeys = {
						// Creature parts
						{id("nose"), 0x0, id("creature")},
						{id("eye"), 0x0, id("creature")},
						{id("antenna"), 0x0, id("creature")},
						{id("ear"), 0x0, id("creature")},
						{id("detail"), 0x0, id("creature")},
						{id("fin"), 0x0, id("creature")},
						{id("wing"), 0x0, id("creature")},
						{id("weapon"), 0x0, id("creature")},
						{id("foot"), 0x0, id("creature")},
						{id("grasper"), 0x0, id("creature")},
						{id("mouth"), 0x0, id("creature")},

						// Flora parts
						{id("cap"), 0x0, id("plant")},
						{id("fruit"), 0x0, id("plant")},
						{id("leaf"), 0x0, id("plant")},

						// Outfitter parts
						{id("shoulderpad"), 0x0, id("tribal")},
						{id("hat"), 0x0, id("tribal")},
						{id("mask"), 0x0, id("tribal")},
						{id("collar"), 0x0, id("tribal")},
						{id("feather"), 0x0, id("tribal")},
						{id("horn"), 0x0, id("tribal")},
						{id("symbol"), 0x0, id("tribal")},

						// Vehicle parts
						{id("wing"), 0x0, id("vehicle")},
						{id("window"), 0x0, id("vehicle")},
						{id("wheel"), 0x0, id("vehicle")},
						{id("weapon"), 0x0, id("vehicle")},
						{id("ski"), 0x0, id("vehicle")},
						{id("sail"), 0x0, id("vehicle")},
						{id("ringpop"), 0x0, id("vehicle")},
						{id("propeller"), 0x0, id("vehicle")},
						{id("paddle"), 0x0, id("vehicle")},
						{id("medallion"), 0x0, id("vehicle")},
						{id("light"), 0x0, id("vehicle")},
						{id("jetengine"), 0x0, id("vehicle")},
						{id("flag"), 0x0, id("vehicle")},
						{id("detail"), 0x0, id("vehicle")},
						{id("Cultural"), 0x0, id("vehicle")},
						{id("balloon"), 0x0, id("vehicle")},

						// Building parts
						{id("window"), 0x0, id("building")},
						{id("detail"), 0x0, id("building")},
						{id("stair"), 0x0, id("building")},
						{id("floor"), 0x0, id("building")},
						{id("door"), 0x0, id("building")},
						{id("chimney"), 0x0, id("building")},
						{id("balcony"), 0x0, id("building")}
					};

					for (const ResourceKey& key : notInteractKeys)
					{
						part->mModelTypesNotToInteractWith.push_back(key);
					}
					break;
				}
			case Mode::CUSTOM:
				break;
			default:
				break;
		}
	}
}


void ToggleStacker::ParseLine(const ArgScript::Line& line)
{
	if (currentMode == Mode::DEFAULT)
	{
		currentMode = Mode::STACK_ALL;
		App::ConsolePrintF("ToggleStacker activated, all parts will now stack.");

	} 
	else 
	{
		currentMode = Mode::DEFAULT;
		App::ConsolePrintF("ToggleStacker deactivated, default behavior restored.");
	}
}

const char* ToggleStacker::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Toggles the ability to stack any part on each other.";
	}
	else 
	{
		return "ToggleStacker: Toggles the ability to stack any part on each other, or disable stacking alltogether, on specific parts or all of them.";
	}
}

int ToggleStacker::AddRef()
{
	return DefaultRefCounted::AddRef();
}

int ToggleStacker::Release()
{
	return DefaultRefCounted::Release();
}