#pragma once
#include "discord_register.h"
#include "discord_rpc.h"
#include <Windows.h>
#include <iostream>

class Discord {
public:
    void initialize();
    void update();
}; inline Discord _discord;