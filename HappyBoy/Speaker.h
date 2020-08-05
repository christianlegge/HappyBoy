#pragma once

#include <memory>

#include "APU.h";

class Speaker
{
public:
	Speaker(std::shared_ptr<APU> apu);

private:
	void Start();
	std::shared_ptr<APU> apu;
};

