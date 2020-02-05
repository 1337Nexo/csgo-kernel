#include "service.h"

// ICY

bool start_service( const std::string& driver_name, const std::string& driver_path )
{
	const SC_HANDLE manager = OpenSCManager( nullptr, nullptr, SC_MANAGER_CREATE_SERVICE );

	if ( !manager )
		return false;

	SC_HANDLE service_handle = CreateService( manager,
		driver_name.c_str( ),
		driver_name.c_str( ),
		SERVICE_START | SERVICE_STOP | DELETE, SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_IGNORE,
		driver_path.c_str( ), nullptr, nullptr, nullptr, nullptr, nullptr );

	if ( !service_handle )
	{
		service_handle = OpenService( manager, driver_name.c_str( ), SERVICE_START );
	}

	if ( !service_handle )
	{
		CloseServiceHandle( manager );
		return false;
	}

	bool result = StartService( service_handle, 0, nullptr );

	if ( !result )
		printf( "[*]: failed to start service, last_error=%d\n", GetLastError( ) );

	CloseServiceHandle( service_handle );
	CloseServiceHandle( manager );

	return result;
}

bool stop_service( const std::string& driver_name )
{
	const SC_HANDLE manager = OpenSCManager( nullptr, nullptr, SC_MANAGER_CREATE_SERVICE );

	if ( !manager )
		return false;

	const SC_HANDLE service_handle = OpenService( manager, driver_name.c_str( ), SERVICE_STOP | DELETE );

	if ( !service_handle )
	{
		CloseServiceHandle( manager );
		return false;
	}

	SERVICE_STATUS status = {0};
	bool result = ControlService( service_handle, SERVICE_CONTROL_STOP, &status );

	DeleteService( service_handle );
	CloseServiceHandle( service_handle );
	CloseServiceHandle( manager );

	return result;
}