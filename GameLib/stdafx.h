#pragma once

#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/array.hpp>

#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION		DIRECTINPUT_HEADER_VERSION
#include <dinput.h>

#include <xaudio2.h>

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dsound.lib" )

#ifdef _DEBUG
	#pragma comment( lib, "libogg_static_d.lib" )
	#pragma comment( lib, "libvorbis_static_d.lib" )
	#pragma comment( lib, "libvorbisfile_static_d.lib" )
#else
	#pragma comment( lib, "libogg_static.lib" )
	#pragma comment( lib, "libvorbis_static.lib" )
	#pragma comment( lib, "libvorbisfile_static.lib" )
#endif