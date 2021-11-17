#pragma once

// Physx resources cleaning
#define PX_RELEASE(res) if(res) { res->release(); res = nullptr; }