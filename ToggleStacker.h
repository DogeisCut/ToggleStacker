#pragma once

#include <Spore\BasicIncludes.h>

class ToggleStacker 
	: public ArgScript::ICommand
	, public App::IUpdatable
	, public DefaultRefCounted
{
public:
	enum class Mode {
		DEFAULT,
		STACK_ALL,
		STACK_NONE,
		CUSTOM
	};

	ToggleStacker();
	~ToggleStacker();

	void Update() override;

	void ParseLine(const ArgScript::Line& line) override;

	virtual int AddRef() override;
	virtual int Release() override;
	
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

private:
	Mode currentMode = Mode::DEFAULT;
};

