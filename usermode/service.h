#pragma once

#include <windows.h>
#include <string>

// ICY

bool start_service( const std::string& driver_name, const std::string& driver_path );
bool stop_service( const std::string& driver_name );