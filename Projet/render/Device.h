#pragma once

enum class CDS_MODE
{
	CDS_FENETRE,
	CDS_PLEIN_ECRAN
};

/// <summary>
/// Interface with render device
/// </summary>
class Device
{
public:
	virtual ~Device() = default;

	virtual void Present();
	virtual void PresentSpecific() = 0;
};
