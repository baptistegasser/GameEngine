#include "stdafx.h"
#include "Contact.h"

Contact::Contact(Pitbull::Actor* FirstActor, Pitbull::Actor* SecondActor, ContactStatus Status)
	: FirstActor(FirstActor)
	, SecondActor(SecondActor)
	, Status{ Status }
{
}
