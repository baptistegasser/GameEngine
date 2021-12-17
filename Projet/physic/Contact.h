#pragma once

#include "core/Actor.h"


class Pitbull::Actor;

class Contact
{
public:
	Pitbull::Actor* FirstActor;
	Pitbull::Actor* SecondActor;
	enum class ContactStatus {
		Started,
		Lost
	} Status;

	Contact(Pitbull::Actor* FirstActor, Pitbull::Actor* SecondActor, ContactStatus Status = ContactStatus::Started);
	~Contact() = default;
};

