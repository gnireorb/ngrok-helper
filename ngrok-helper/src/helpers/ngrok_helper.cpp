#include "HTTPRequest.hpp"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include <future>

#include "util/util.hpp"

#include "ngrok_helper.hpp"

using namespace rapidjson;

bool ngrok::init( )
{
#ifndef DEBUG
	if ( !util::file_exists( "ngrok.exe" ) )
	{
		MessageBox( NULL, "O arquivo 'ngrok.exe' eh inexistente, baixe-o em 'ngrok.com'.", "Error", MB_ICONERROR );
		exit( -1 );
	}

	if ( !util::file_exists( "settings.json" ) )
	{
		if ( !util::create_file( "settings.json" ) )
		{
			MessageBox( NULL, "Ocorreu uma falha ao criar o arquivo 'settings.json'.", "Error", MB_ICONERROR );
			return false;
		}

		if ( !util::file_exists( "settings.json" ) )
		{
			MessageBox( NULL, "O arquivo 'settings.json' eh inexistente.", "Error", MB_ICONERROR );
			return false;
		}

		if ( !util::write_to_file( "settings.json", "{\"last_port\":0,\"ngrok_region\":0}" ) )
		{
			MessageBox( NULL, "Ocorreu uma falha ao escrever em 'settings.json'.", "Error", MB_ICONERROR );
			return false;
		}
	}
#endif

	return true;
}

bool ngrok::create_tunnel( int port, int region )
{
	std::string commandline;

	if ( region >= 4 || region < 0 )
		return false;

	switch ( region )
	{
	case 0:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region sa";
		WinExec( commandline.c_str( ), SW_HIDE );
		break;
	case 1:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region us";
		WinExec( commandline.c_str( ), SW_HIDE );
		break;
	case 2:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region eu";
		WinExec( commandline.c_str( ), SW_HIDE );
		break;
	case 3:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region ap";
		WinExec( commandline.c_str( ), SW_HIDE );
		break;
	case 4:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region au";
		WinExec( commandline.c_str( ), SW_HIDE );
		break;
	}

	return true;
}

std::string ngrok::get_public_url( )
{
	std::string api_request;
	try
	{
		http::Request request( "http://127.0.0.1:4040/api/tunnels" );
		const http::Response response = request.send( "GET" );
		api_request = std::string( response.body.begin( ), response.body.end( ) );
	}
	catch ( const std::exception& e )
	{
		MessageBox( NULL, e.what( ), "Error", MB_ICONERROR );
	}
	const char* json = api_request.c_str( );
	Document d;
	d.Parse( json );
	Value const& tunnels = d[ "tunnels" ];
	for ( auto& tunnel_url : tunnels.GetArray( ) )
	{
		std::string public_url = tunnel_url[ "public_url" ].GetString( );
		public_url.erase( 0, 6 );

		return public_url;
	}
	return ("Failed to get IP");
}