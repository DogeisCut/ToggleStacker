#pragma once

#include <Spore\BasicIncludes.h>

class ToggleStacker 
	: public ArgScript::ICommand
	, public App::IUpdatable
	, public DefaultRefCounted
{
public:
	ToggleStacker();
	~ToggleStacker();

	void Update() override;

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;

	virtual int AddRef() override;
	virtual int Release() override;

	int partcount;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

