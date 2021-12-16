#pragma once

#include "core/Actor.h"


class Pitbull::Actor;

class Contact
{
public:
	Pitbull::Actor* FirstActor;
	Pitbull::Actor* SecondActor;

	Contact(Pitbull::Actor* FirstActor, Pitbull::Actor* SecondActor);
	~Contact() = default;
};

