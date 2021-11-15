#include <iostream>

#include "core/Actor.h"
#include "core/Component.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace Pitbull;

class InputController : public Component<InputController>
{
public:
	~InputController() override = default;
};

class Collider : public Component<Collider>
{
public:
	~Collider() override = default;
};

void main_impl()
{
	using namespace std;

	Actor ball;
	auto controller = new InputController{};
	auto collider = new Collider{};

	ball.AddComponent(controller);
	ball.AddComponent(new InputController{});
	ball.AddComponent(collider);

	auto& comp = ball.GetComponent<InputController>();
	auto AllControllers = ball.GetComponents<InputController>();

	cout << "Hello team A " << controller->GetID() << " " << InputController::GetIDStatic() << endl;
	cout << "Hello team A " << collider->GetID() << " " << Collider::GetIDStatic() << endl;
}

int main() {
	main_impl();
	_CrtDumpMemoryLeaks();
}